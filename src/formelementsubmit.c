/*
 * Copyright (C) 2015-2016 Andrea Zagli <azagli@libero.it>
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
#include "formelementsubmit.h"

static void zak_form_cgi_form_element_submit_class_init (ZakFormCgiFormElementSubmitClass *class);
static void zak_form_cgi_form_element_submit_init (ZakFormCgiFormElementSubmit *zak_form_cgi_form_element_submit);

static gchar *zak_form_cgi_form_element_submit_render (ZakFormCgiFormElement *element);

static void zak_form_cgi_form_element_submit_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec);
static void zak_form_cgi_form_element_submit_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec);

static void zak_form_cgi_form_element_submit_dispose (GObject *gobject);
static void zak_form_cgi_form_element_submit_finalize (GObject *gobject);

#define ZAK_FORM_CGI_FORM_ELEMENT_SUBMIT_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_FORM_CGI_TYPE_FORM_ELEMENT_SUBMIT, ZakFormCgiFormElementSubmitPrivate))

typedef struct _ZakFormCgiFormElementSubmitPrivate ZakFormCgiFormElementSubmitPrivate;
struct _ZakFormCgiFormElementSubmitPrivate
	{
		gpointer empty;
	};

G_DEFINE_TYPE (ZakFormCgiFormElementSubmit, zak_form_cgi_form_element_submit, ZAK_FORM_CGI_TYPE_FORM_ELEMENT)

static void
zak_form_cgi_form_element_submit_class_init (ZakFormCgiFormElementSubmitClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	ZakFormCgiFormElementClass *elem_class = ZAK_FORM_CGI_FORM_ELEMENT_CLASS (klass);

	object_class->set_property = zak_form_cgi_form_element_submit_set_property;
	object_class->get_property = zak_form_cgi_form_element_submit_get_property;
	object_class->dispose = zak_form_cgi_form_element_submit_dispose;
	object_class->finalize = zak_form_cgi_form_element_submit_finalize;

	elem_class->render = zak_form_cgi_form_element_submit_render;

	g_type_class_add_private (object_class, sizeof (ZakFormCgiFormElementSubmitPrivate));
}

static void
zak_form_cgi_form_element_submit_init (ZakFormCgiFormElementSubmit *zak_form_cgi_form_element_submit)
{
	ZakFormCgiFormElementSubmitPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_SUBMIT_GET_PRIVATE (zak_form_cgi_form_element_submit);

}

/**
 * zak_form_cgi_form_element_submit_new:
 *
 * Returns: the newly created #ZakFormCgiFormElementSubmit object.
 */
ZakFormCgiFormElement
*zak_form_cgi_form_element_submit_new ()
{
	ZakFormCgiFormElementSubmit *zak_form_cgi_form_element_submit;

	zak_form_cgi_form_element_submit = ZAK_FORM_CGI_FORM_ELEMENT_SUBMIT (g_object_new (zak_form_cgi_form_element_submit_get_type (), NULL));

	return ZAK_FORM_CGI_FORM_ELEMENT (zak_form_cgi_form_element_submit);
}

/**
 * zak_form_cgi_form_element_submit_new_attrs:
 * @id:
 * @...:
 *
 * Returns: the newly created #ZakFormCgiFormElementSubmit object.
 */
ZakFormCgiFormElement
*zak_form_cgi_form_element_submit_new_attrs (const gchar *id,
										...)
{
	va_list ap;

	ZakFormCgiFormElement *zak_form_cgi_form_element_submit;

	zak_form_cgi_form_element_submit = zak_form_cgi_form_element_submit_new ();

	va_start (ap, id);

	ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_submit_parent_class)->construct (zak_form_cgi_form_element_submit,
																					  id,
																					  zak_cgi_commons_valist_to_ghashtable (ap));

	return zak_form_cgi_form_element_submit;
}

gboolean
zak_form_cgi_form_element_submit_xml_parsing (ZakFormElement *element, xmlNodePtr xmlnode)
{
	gboolean ret;

	gchar *id;

	GHashTable *ht_attrs;

	xmlNode *cur;

	id = NULL;

	ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_submit_parent_class)->xml_parsing (element, xmlnode);

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
			ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_submit_parent_class)->construct (ZAK_FORM_CGI_FORM_ELEMENT (element),
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
*zak_form_cgi_form_element_submit_render (ZakFormCgiFormElement *element)
{
	gchar *ret;

	GHashTable *ht_attrs;
	gchar *attr_class;
	GString *str_attr_class;
	gchar *form_control;

	ZakFormCgiFormElementClass *klass;

	klass = (ZakFormCgiFormElementClass *)g_type_class_peek_parent (ZAK_FORM_CGI_FORM_ELEMENT_SUBMIT_GET_CLASS (ZAK_FORM_CGI_FORM_ELEMENT_SUBMIT (element)));

	ht_attrs = klass->get_ht_attrs (element);

	attr_class = g_hash_table_lookup (ht_attrs, "class");
	if (attr_class != NULL)
		{
			str_attr_class = g_string_new (attr_class);
			form_control = g_strstr_len (str_attr_class->str, -1, "form-control");
			if (form_control != NULL)
				{
					/* no form-control for submit button */
					g_string_erase (str_attr_class, form_control - str_attr_class->str, 12);
				}

			if (g_strstr_len (attr_class, -1, "btn btn-default") == NULL)
				{
					g_hash_table_insert (ht_attrs, "class", g_strdup_printf ("%s btn btn-default", str_attr_class->str));
				}

			g_free (attr_class);
			g_string_free (str_attr_class, TRUE);
		}
	else
		{
			g_hash_table_replace (ht_attrs, g_strdup ("class"), g_strdup ("btn btn-default"));
		}

	ret = zak_cgi_tag_submit_ht (zak_form_cgi_form_element_get_id (element), ht_attrs);

	return ret;
}

/* PRIVATE */
static void
zak_form_cgi_form_element_submit_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakFormCgiFormElementSubmit *zak_form_cgi_form_element_submit = (ZakFormCgiFormElementSubmit *)object;
	ZakFormCgiFormElementSubmitPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_SUBMIT_GET_PRIVATE (zak_form_cgi_form_element_submit);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_cgi_form_element_submit_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakFormCgiFormElementSubmit *zak_form_cgi_form_element_submit = (ZakFormCgiFormElementSubmit *)object;
	ZakFormCgiFormElementSubmitPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_SUBMIT_GET_PRIVATE (zak_form_cgi_form_element_submit);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_cgi_form_element_submit_dispose (GObject *gobject)
{
	ZakFormCgiFormElementSubmit *zak_form_cgi_form_element_submit = (ZakFormCgiFormElementSubmit *)gobject;
	ZakFormCgiFormElementSubmitPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_SUBMIT_GET_PRIVATE (zak_form_cgi_form_element_submit);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_cgi_form_element_submit_finalize (GObject *gobject)
{
	ZakFormCgiFormElementSubmit *zak_form_cgi_form_element_submit = (ZakFormCgiFormElementSubmit *)gobject;
	ZakFormCgiFormElementSubmitPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_SUBMIT_GET_PRIVATE (zak_form_cgi_form_element_submit);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}
