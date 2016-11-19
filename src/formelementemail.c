/*
 * Copyright (C) 2016 Andrea Zagli <azagli@libero.it>
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
#include "formelementemail.h"

static void zak_form_cgi_form_element_email_class_init (ZakFormCgiFormElementEmailClass *class);
static void zak_form_cgi_form_element_email_init (ZakFormCgiFormElementEmail *zak_form_cgi_form_element_email);

static gchar *zak_form_cgi_form_element_email_render (ZakFormCgiFormElement *element);

static void zak_form_cgi_form_element_email_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec);
static void zak_form_cgi_form_element_email_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec);

static void zak_form_cgi_form_element_email_dispose (GObject *gobject);
static void zak_form_cgi_form_element_email_finalize (GObject *gobject);

#define ZAK_FORM_CGI_FORM_ELEMENT_EMAIL_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_FORM_CGI_TYPE_FORM_ELEMENT_EMAIL, ZakFormCgiFormElementEmailPrivate))

typedef struct _ZakFormCgiFormElementEmailPrivate ZakFormCgiFormElementEmailPrivate;
struct _ZakFormCgiFormElementEmailPrivate
	{
		gpointer empty;
	};

G_DEFINE_TYPE (ZakFormCgiFormElementEmail, zak_form_cgi_form_element_email, ZAK_FORM_CGI_TYPE_FORM_ELEMENT)

static void
zak_form_cgi_form_element_email_class_init (ZakFormCgiFormElementEmailClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	ZakFormCgiFormElementClass *elem_class = ZAK_FORM_CGI_FORM_ELEMENT_CLASS (klass);

	object_class->set_property = zak_form_cgi_form_element_email_set_property;
	object_class->get_property = zak_form_cgi_form_element_email_get_property;
	object_class->dispose = zak_form_cgi_form_element_email_dispose;
	object_class->finalize = zak_form_cgi_form_element_email_finalize;

	elem_class->render = zak_form_cgi_form_element_email_render;

	g_type_class_add_private (object_class, sizeof (ZakFormCgiFormElementEmailPrivate));
}

static void
zak_form_cgi_form_element_email_init (ZakFormCgiFormElementEmail *zak_form_cgi_form_element_email)
{
	ZakFormCgiFormElementEmailPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_EMAIL_GET_PRIVATE (zak_form_cgi_form_element_email);

}

/**
 * zak_form_cgi_form_element_email_new:
 *
 * Returns: the newly created #ZakFormCgiFormElementEmail object.
 */
ZakFormCgiFormElement
*zak_form_cgi_form_element_email_new ()
{
	ZakFormCgiFormElementEmail *zak_form_cgi_form_element_email;

	zak_form_cgi_form_element_email = ZAK_FORM_CGI_FORM_ELEMENT_EMAIL (g_object_new (zak_form_cgi_form_element_email_get_type (), NULL));

	return ZAK_FORM_CGI_FORM_ELEMENT (zak_form_cgi_form_element_email);
}

/**
 * zak_form_cgi_form_element_email_new_attrs:
 * @id:
 * @...:
 *
 * Returns: the newly created #ZakFormCgiFormElementEmail object.
 */
ZakFormCgiFormElement
*zak_form_cgi_form_element_email_new_attrs (const gchar *id,
										  ...)
{
	va_list ap;

	GHashTable *ht_attrs;

	ZakFormCgiFormElement *zak_form_cgi_form_element_email;

	zak_form_cgi_form_element_email = zak_form_cgi_form_element_email_new ();

	va_start (ap, id);

	ht_attrs = zak_cgi_commons_valist_to_ghashtable (ap);
	g_hash_table_replace (ht_attrs, "type", "email");

	ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_email_parent_class)->construct (zak_form_cgi_form_element_email,
																						id,
																						ht_attrs);

	return zak_form_cgi_form_element_email;
}

gboolean
zak_form_cgi_form_element_email_xml_parsing (ZakFormElement *element, xmlNodePtr xmlnode)
{
	gboolean ret;

	gchar *id;

	GHashTable *ht_attrs;

	xmlNode *cur;

	id = NULL;

	ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_email_parent_class)->xml_parsing (element, xmlnode);

	ht_attrs = g_hash_table_new (g_str_hash, g_str_equal);
	g_hash_table_replace (ht_attrs, "type", "email");

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
			ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_email_parent_class)->construct (ZAK_FORM_CGI_FORM_ELEMENT (element),
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
*zak_form_cgi_form_element_email_render (ZakFormCgiFormElement *element)
{
	gchar *ret;

	GHashTable *ht_attrs;

	gchar *value;

	ZakFormCgiFormElementClass *klass;

	klass = (ZakFormCgiFormElementClass *)g_type_class_peek_parent (ZAK_FORM_CGI_FORM_ELEMENT_EMAIL_GET_CLASS (ZAK_FORM_CGI_FORM_ELEMENT_EMAIL (element)));

	ht_attrs = klass->get_ht_attrs (element);

	value = zak_form_element_get_value (ZAK_FORM_ELEMENT (element));
	if (value != NULL)
		{
			g_hash_table_insert (ht_attrs, (gpointer)"value", (gpointer)g_strdup (value));
		}

	ret = zak_cgi_tag_tag_ht ("input", zak_form_cgi_form_element_get_id (element), ht_attrs);

	return ret;
}

/* PRIVATE */
static void
zak_form_cgi_form_element_email_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakFormCgiFormElementEmail *zak_form_cgi_form_element_email = (ZakFormCgiFormElementEmail *)object;
	ZakFormCgiFormElementEmailPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_EMAIL_GET_PRIVATE (zak_form_cgi_form_element_email);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_cgi_form_element_email_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakFormCgiFormElementEmail *zak_form_cgi_form_element_email = (ZakFormCgiFormElementEmail *)object;
	ZakFormCgiFormElementEmailPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_EMAIL_GET_PRIVATE (zak_form_cgi_form_element_email);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_cgi_form_element_email_dispose (GObject *gobject)
{
	ZakFormCgiFormElementEmail *zak_form_cgi_form_element_email = (ZakFormCgiFormElementEmail *)gobject;
	ZakFormCgiFormElementEmailPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_EMAIL_GET_PRIVATE (zak_form_cgi_form_element_email);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_cgi_form_element_email_finalize (GObject *gobject)
{
	ZakFormCgiFormElementEmail *zak_form_cgi_form_element_email = (ZakFormCgiFormElementEmail *)gobject;
	ZakFormCgiFormElementEmailPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_EMAIL_GET_PRIVATE (zak_form_cgi_form_element_email);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}
