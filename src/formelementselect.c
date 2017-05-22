/*
 * Copyright (C) 2015-2017 Andrea Zagli <azagli@libero.it>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <libzakcgi/libzakcgi.h>
#include "formelementselect.h"

static void zak_form_cgi_form_element_select_class_init (ZakFormCgiFormElementSelectClass *class);
static void zak_form_cgi_form_element_select_init (ZakFormCgiFormElementSelect *zak_form_cgi_form_element_select);

static gchar *zak_form_cgi_form_element_select_render (ZakFormCgiFormElement *element);

static void zak_form_cgi_form_element_select_set_property (GObject *object,
                                                           guint property_id,
                                                           const GValue *value,
                                                           GParamSpec *pspec);
static void zak_form_cgi_form_element_select_get_property (GObject *object,
                                                           guint property_id,
                                                           GValue *value,
                                                           GParamSpec *pspec);

static void zak_form_cgi_form_element_select_dispose (GObject *gobject);
static void zak_form_cgi_form_element_select_finalize (GObject *gobject);

#define ZAK_FORM_CGI_FORM_ELEMENT_SELECT_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_FORM_CGI_TYPE_FORM_ELEMENT_SELECT, ZakFormCgiFormElementSelectPrivate))

typedef struct _ZakFormCgiFormElementSelectPrivate ZakFormCgiFormElementSelectPrivate;
struct _ZakFormCgiFormElementSelectPrivate
	{
		GHashTable *ht_options;
	};

G_DEFINE_TYPE (ZakFormCgiFormElementSelect, zak_form_cgi_form_element_select, ZAK_FORM_CGI_TYPE_FORM_ELEMENT)

static void
zak_form_cgi_form_element_select_class_init (ZakFormCgiFormElementSelectClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	ZakFormCgiFormElementClass *elem_class = ZAK_FORM_CGI_FORM_ELEMENT_CLASS (klass);

	object_class->set_property = zak_form_cgi_form_element_select_set_property;
	object_class->get_property = zak_form_cgi_form_element_select_get_property;
	object_class->dispose = zak_form_cgi_form_element_select_dispose;
	object_class->finalize = zak_form_cgi_form_element_select_finalize;

	elem_class->render = zak_form_cgi_form_element_select_render;

	g_type_class_add_private (object_class, sizeof (ZakFormCgiFormElementSelectPrivate));
}

static void
zak_form_cgi_form_element_select_init (ZakFormCgiFormElementSelect *zak_form_cgi_form_element_select)
{
	ZakFormCgiFormElementSelectPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_SELECT_GET_PRIVATE (zak_form_cgi_form_element_select);

	priv->ht_options = g_hash_table_new (g_str_hash, g_str_equal);
}

/**
 * zak_form_cgi_form_element_select_new:
 *
 * Returns: the newly created #ZakFormCgiFormElementSelect object.
 */
ZakFormCgiFormElement
*zak_form_cgi_form_element_select_new ()
{
	ZakFormCgiFormElementSelect *zak_form_cgi_form_element_select;

	zak_form_cgi_form_element_select = ZAK_FORM_CGI_FORM_ELEMENT_SELECT (g_object_new (zak_form_cgi_form_element_select_get_type (), NULL));

	return ZAK_FORM_CGI_FORM_ELEMENT (zak_form_cgi_form_element_select);
}

/**
 * zak_form_cgi_form_element_select_new_attrs:
 * @id:
 * @...:
 *
 * Returns: the newly created #ZakFormCgiFormElementSelect object.
 */
ZakFormCgiFormElement
*zak_form_cgi_form_element_select_new_attrs (const gchar *id,
										...)
{
	va_list ap;

	ZakFormCgiFormElement *zak_form_cgi_form_element_select;

	zak_form_cgi_form_element_select = zak_form_cgi_form_element_select_new ();

	va_start (ap, id);

	ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_select_parent_class)->construct (zak_form_cgi_form_element_select,
		                      id,
		                      zak_cgi_commons_valist_to_ghashtable (ap));

	return zak_form_cgi_form_element_select;
}

/**
 * zak_form_cgi_form_element_select_add_option:
 * @element:
 * @value:
 * @content:
 * @...:
 *
 */
void
zak_form_cgi_form_element_select_add_option (ZakFormCgiFormElementSelect *element,
                                             const gchar *value,
                                             const gchar *content,
                                             ...)
{
	va_list ap;
	GHashTable *ht_attrs;

	ZakFormCgiFormElementSelectPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_SELECT_GET_PRIVATE (element);

	va_start (ap, content);
	ht_attrs = zak_cgi_commons_valist_to_ghashtable (ap);
	if (ht_attrs == NULL)
		{
			g_hash_table_new (g_str_hash, g_str_equal);
		}
	g_hash_table_replace (ht_attrs, "zak-cgi-content", g_strdup (content));

	g_hash_table_replace (priv->ht_options, g_strdup (value), ht_attrs);
}

gboolean
zak_form_cgi_form_element_select_xml_parsing (ZakFormElement *element, xmlNodePtr xmlnode)
{
	gboolean ret;

	gchar *id;

	GHashTable *ht_attrs;

	xmlNode *cur;

	id = NULL;

	ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_select_parent_class)->xml_parsing (element, xmlnode);

	ht_attrs = g_hash_table_new (g_str_hash, g_str_equal);

	cur = xmlnode->children;
	while (cur != NULL)
		{
			if (xmlStrcmp (cur->name, (const xmlChar *)"id") == 0)
				{
					id = (gchar *)xmlNodeGetContent (cur);
				}
			else if (xmlStrcmp (cur->name, (const xmlChar *)"label") == 0)
				{
					zak_form_cgi_form_element_set_label (ZAK_FORM_CGI_FORM_ELEMENT (element), (gchar *)xmlNodeGetContent (cur), NULL);
				}
			else if (xmlStrcmp (cur->name, (const xmlChar *)"option") == 0)
				{
					zak_form_cgi_form_element_select_add_option (ZAK_FORM_CGI_FORM_ELEMENT_SELECT (element), (gchar *)xmlGetProp (cur, (xmlChar *)"id"), (gchar *)xmlNodeGetContent (cur), NULL);
				}
			else if (xmlStrcmp (cur->name, (const xmlChar *)"text") == 0)
				{
				}
			else
				{
					g_hash_table_replace (ht_attrs, g_strdup (cur->name), (gchar *)xmlNodeGetContent (cur));
				}

			cur = cur->next;
		}

	if (id != NULL)
		{
			ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_select_parent_class)->construct (ZAK_FORM_CGI_FORM_ELEMENT (element),
				                      id,
				                      ht_attrs);
			ret = TRUE;
		}
	else
		{
			ret = FALSE;
		}

	return ret;
}

static gchar
*zak_form_cgi_form_element_select_render (ZakFormCgiFormElement *element)
{
	gchar *ret;

	GHashTableIter iter;
	gpointer key;
	gpointer value;
	GString *options;
	GHashTable *ht_options_attrs;

	GHashTable *ht_attrs;

	gchar *gval;

	ZakFormCgiFormElementClass *klass;

	ZakFormCgiFormElementSelectPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_SELECT_GET_PRIVATE (element);

	klass = (ZakFormCgiFormElementClass *)g_type_class_peek_parent (ZAK_FORM_CGI_FORM_ELEMENT_SELECT_GET_CLASS (ZAK_FORM_CGI_FORM_ELEMENT_SELECT (element)));

	gval = zak_form_element_get_value (ZAK_FORM_ELEMENT (element));

	/* list options */
	options = g_string_new ("\n");
	g_hash_table_iter_init (&iter, priv->ht_options);
	while (g_hash_table_iter_next (&iter, &key, &value))
		{
			ht_options_attrs = (GHashTable *)value;
			g_hash_table_replace (ht_options_attrs, "value", g_strdup ((gchar *)key));

			if (gval != NULL)
				{
					if (g_strcmp0 (gval, (gchar *)key) == 0)
						{
							g_hash_table_replace (ht_options_attrs, "selected", g_strdup ("selected"));
						}
				}

			g_string_append (options,
			                 zak_cgi_tag_tag_ht ("option", NULL, ht_options_attrs));
			g_string_append (options, "\n");
		}

	ht_attrs = klass->get_ht_attrs (element);

	if (!zak_form_element_get_editable (ZAK_FORM_ELEMENT (element)))
		{
			g_hash_table_insert (ht_attrs, (gpointer)"disabled", (gpointer)"disabled");
		}

	g_hash_table_replace (ht_attrs, "zak-cgi-content", g_strdup (options->str));
	ret = zak_cgi_tag_tag_ht ("select", zak_form_cgi_form_element_get_id (element), ht_attrs);

	g_string_free (options, TRUE);

	return ret;
}

/* PRIVATE */
static void
zak_form_cgi_form_element_select_set_property (GObject *object,
                                               guint property_id,
                                               const GValue *value,
                                               GParamSpec *pspec)
{
	ZakFormCgiFormElementSelect *zak_form_cgi_form_element_select = (ZakFormCgiFormElementSelect *)object;
	ZakFormCgiFormElementSelectPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_SELECT_GET_PRIVATE (zak_form_cgi_form_element_select);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_cgi_form_element_select_get_property (GObject *object,
                                               guint property_id,
                                               GValue *value,
                                               GParamSpec *pspec)
{
	ZakFormCgiFormElementSelect *zak_form_cgi_form_element_select = (ZakFormCgiFormElementSelect *)object;
	ZakFormCgiFormElementSelectPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_SELECT_GET_PRIVATE (zak_form_cgi_form_element_select);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_cgi_form_element_select_dispose (GObject *gobject)
{
	ZakFormCgiFormElementSelect *zak_form_cgi_form_element_select = (ZakFormCgiFormElementSelect *)gobject;
	ZakFormCgiFormElementSelectPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_SELECT_GET_PRIVATE (zak_form_cgi_form_element_select);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_cgi_form_element_select_finalize (GObject *gobject)
{
	ZakFormCgiFormElementSelect *zak_form_cgi_form_element_select = (ZakFormCgiFormElementSelect *)gobject;
	ZakFormCgiFormElementSelectPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_SELECT_GET_PRIVATE (zak_form_cgi_form_element_select);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}
