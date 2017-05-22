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

#include <libzakutils/libzakutils.h>

#include <libzakcgi/libzakcgi.h>
#include "formelementcheck.h"

static void zak_form_cgi_form_element_check_class_init (ZakFormCgiFormElementCheckClass *class);
static void zak_form_cgi_form_element_check_init (ZakFormCgiFormElementCheck *zak_form_cgi_form_element_check);

static gchar *zak_form_cgi_form_element_check_render (ZakFormCgiFormElement *element);

static void zak_form_cgi_form_element_check_set_property (GObject *object,
                                                          guint property_id,
                                                          const GValue *value,
                                                          GParamSpec *pspec);
static void zak_form_cgi_form_element_check_get_property (GObject *object,
                                                          guint property_id,
                                                          GValue *value,
                                                          GParamSpec *pspec);

static void zak_form_cgi_form_element_check_dispose (GObject *gobject);
static void zak_form_cgi_form_element_check_finalize (GObject *gobject);

#define ZAK_FORM_CGI_FORM_ELEMENT_CHECK_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_FORM_CGI_TYPE_FORM_ELEMENT_CHECK, ZakFormCgiFormElementCheckPrivate))

typedef struct _ZakFormCgiFormElementCheckPrivate ZakFormCgiFormElementCheckPrivate;
struct _ZakFormCgiFormElementCheckPrivate
	{
		gchar *text;
		gboolean in_line;
	};

G_DEFINE_TYPE (ZakFormCgiFormElementCheck, zak_form_cgi_form_element_check, ZAK_FORM_CGI_TYPE_FORM_ELEMENT)

static void
zak_form_cgi_form_element_check_class_init (ZakFormCgiFormElementCheckClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	ZakFormCgiFormElementClass *elem_class = ZAK_FORM_CGI_FORM_ELEMENT_CLASS (klass);

	object_class->set_property = zak_form_cgi_form_element_check_set_property;
	object_class->get_property = zak_form_cgi_form_element_check_get_property;
	object_class->dispose = zak_form_cgi_form_element_check_dispose;
	object_class->finalize = zak_form_cgi_form_element_check_finalize;

	elem_class->render = zak_form_cgi_form_element_check_render;

	g_type_class_add_private (object_class, sizeof (ZakFormCgiFormElementCheckPrivate));
}

static void
zak_form_cgi_form_element_check_init (ZakFormCgiFormElementCheck *zak_form_cgi_form_element_check)
{
	ZakFormCgiFormElementCheckPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_CHECK_GET_PRIVATE (zak_form_cgi_form_element_check);

	priv->text = g_strdup ("");
	priv->in_line = TRUE;
}

/**
 * zak_form_cgi_form_element_check_new:
 *
 * Returns: the newly created #ZakFormCgiFormElementCheck object.
 */
ZakFormCgiFormElement
*zak_form_cgi_form_element_check_new ()
{
	ZakFormCgiFormElementCheck *zak_form_cgi_form_element_check;

	zak_form_cgi_form_element_check = ZAK_FORM_CGI_FORM_ELEMENT_CHECK (g_object_new (zak_form_cgi_form_element_check_get_type (), NULL));

	return ZAK_FORM_CGI_FORM_ELEMENT (zak_form_cgi_form_element_check);
}

/**
 * zak_form_cgi_form_element_check_new_attrs:
 * @id:
 * @...:
 *
 * Returns: the newly created #ZakFormCgiFormElementCheck object.
 */
ZakFormCgiFormElement
*zak_form_cgi_form_element_check_new_attrs (const gchar *id,
                                            ...)
{
	va_list ap;

	GHashTable *ht_attrs;

	ZakFormCgiFormElement *zak_form_cgi_form_element_check;

	zak_form_cgi_form_element_check = zak_form_cgi_form_element_check_new ();

	va_start (ap, id);

	ht_attrs = zak_cgi_commons_valist_to_ghashtable (ap);
	g_hash_table_replace (ht_attrs, "type", "checkbox");

	ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_check_parent_class)->construct (zak_form_cgi_form_element_check,
		                     id,
		                     ht_attrs);

	return zak_form_cgi_form_element_check;
}

gchar
*zak_form_cgi_form_element_check_get_text (ZakFormCgiFormElementCheck *element)
{
	ZakFormCgiFormElementCheckPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_CHECK_GET_PRIVATE (element);

	return g_strdup (priv->text);
}

void
zak_form_cgi_form_element_check_set_text (ZakFormCgiFormElementCheck *element, const gchar *text)
{
	ZakFormCgiFormElementCheckPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_CHECK_GET_PRIVATE (element);

	g_free (priv->text);

	if (text == NULL)
		{
			priv->text = g_strdup ("");
		}
	else
		{
			priv->text = g_strdup (text);
		}
}

gboolean
zak_form_cgi_form_element_check_get_in_line (ZakFormCgiFormElementCheck *element)
{
	ZakFormCgiFormElementCheckPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_CHECK_GET_PRIVATE (element);

	return priv->in_line;
}

void
zak_form_cgi_form_element_check_set_in_line (ZakFormCgiFormElementCheck *element, gboolean in_line)
{
	ZakFormCgiFormElementCheckPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_CHECK_GET_PRIVATE (element);

	priv->in_line = in_line;
}

gboolean
zak_form_cgi_form_element_check_xml_parsing (ZakFormElement *element, xmlNodePtr xmlnode)
{
	gboolean ret;

	gchar *id;

	GHashTable *ht_attrs;

	xmlNode *cur;

	id = NULL;

	ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_check_parent_class)->xml_parsing (element, xmlnode);

	ht_attrs = g_hash_table_new (g_str_hash, g_str_equal);
	g_hash_table_replace (ht_attrs, "type", "checkbox");

	cur = xmlnode->children;
	while (cur != NULL)
		{
			if (xmlStrEqual (cur->name, (const xmlChar *)"id"))
				{
					id = (gchar *)xmlNodeGetContent (cur);
				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"label"))
				{
					zak_form_cgi_form_element_set_label (ZAK_FORM_CGI_FORM_ELEMENT (element), (gchar *)xmlNodeGetContent (cur), NULL);
				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"text"))
				{

				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"zak-cgi-text"))
				{
					zak_form_cgi_form_element_check_set_text (ZAK_FORM_CGI_FORM_ELEMENT_CHECK (element), (gchar *)xmlNodeGetContent (cur));
				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"zak-cgi-inline"))
				{
					zak_form_cgi_form_element_check_set_in_line (ZAK_FORM_CGI_FORM_ELEMENT_CHECK (element),
					                                             (gboolean)xmlStrEqual (xmlNodeGetContent (cur), (const xmlChar *)"TRUE"));
				}
			else
				{
					g_hash_table_replace (ht_attrs, g_strdup (cur->name), (gchar *)xmlNodeGetContent (cur));
				}

			cur = cur->next;
		}

	if (id != NULL)
		{
			ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_check_parent_class)->construct (ZAK_FORM_CGI_FORM_ELEMENT (element),
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
*zak_form_cgi_form_element_check_render (ZakFormCgiFormElement *element)
{
	gchar *ret;
	GString *str;

	GHashTable *ht_attrs;
	GHashTable *ht_label_attrs;

	gchar *value;
	gchar *attr_class;

	ZakFormCgiFormElementClass *klass;

	ZakFormCgiFormElementCheckPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_CHECK_GET_PRIVATE (element);

	klass = (ZakFormCgiFormElementClass *)g_type_class_peek_parent (ZAK_FORM_CGI_FORM_ELEMENT_CHECK_GET_CLASS (ZAK_FORM_CGI_FORM_ELEMENT_CHECK (element)));

	ht_attrs = klass->get_ht_attrs (element);
	ht_label_attrs = klass->get_ht_label_attrs (element);

	if (ht_label_attrs != NULL)
		{
			str = g_string_new ("<br/>\n");
		}
	else
		{
			str = g_string_new ("");
		}

	value = zak_form_element_get_value (ZAK_FORM_ELEMENT (element));
	if (value != NULL
		&& zak_utils_string_to_boolean (value))
		{
			g_hash_table_insert (ht_attrs, (gpointer)"checked", (gpointer)"checked");
		}

	attr_class = g_hash_table_lookup (ht_attrs, "class");
	if (attr_class != NULL)
		{
			g_hash_table_insert (ht_attrs, "class", g_strjoinv ("", g_strsplit (attr_class, "form-control", -1)));
			g_free (attr_class);
		}

	g_string_append_printf (str, "\n%s<label%s>\n%s %s</label>%s",
	                        priv->in_line ? "" : "<div class=\"checkbox\">\n",
	                        priv->in_line ? " class=\"checkbox-inline\"" : "",
	                        zak_cgi_tag_tag_ht ("input", zak_form_cgi_form_element_get_id (element), ht_attrs),
	                        zak_form_cgi_form_element_check_get_text (ZAK_FORM_CGI_FORM_ELEMENT_CHECK (element)),
	                        priv->in_line ? "" : "\n</div><br/>");

	if (ht_label_attrs != NULL)
		{
			g_string_append (str, "<br/><br/>\n");
		}

	ret = g_strdup (str->str);
	g_string_free (str, TRUE);

	return ret;
}

/* PRIVATE */
static void
zak_form_cgi_form_element_check_set_property (GObject *object,
                                              guint property_id,
                                              const GValue *value,
                                              GParamSpec *pspec)
{
	ZakFormCgiFormElementCheck *zak_form_cgi_form_element_check = (ZakFormCgiFormElementCheck *)object;
	ZakFormCgiFormElementCheckPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_CHECK_GET_PRIVATE (zak_form_cgi_form_element_check);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_cgi_form_element_check_get_property (GObject *object,
                                              guint property_id,
                                              GValue *value,
                                              GParamSpec *pspec)
{
	ZakFormCgiFormElementCheck *zak_form_cgi_form_element_check = (ZakFormCgiFormElementCheck *)object;
	ZakFormCgiFormElementCheckPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_CHECK_GET_PRIVATE (zak_form_cgi_form_element_check);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_cgi_form_element_check_dispose (GObject *gobject)
{
	ZakFormCgiFormElementCheck *zak_form_cgi_form_element_check = (ZakFormCgiFormElementCheck *)gobject;
	ZakFormCgiFormElementCheckPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_CHECK_GET_PRIVATE (zak_form_cgi_form_element_check);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_cgi_form_element_check_finalize (GObject *gobject)
{
	ZakFormCgiFormElementCheck *zak_form_cgi_form_element_check = (ZakFormCgiFormElementCheck *)gobject;
	ZakFormCgiFormElementCheckPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_CHECK_GET_PRIVATE (zak_form_cgi_form_element_check);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}
