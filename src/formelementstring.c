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
#include "formelementstring.h"

static void zak_form_cgi_form_element_string_class_init (ZakFormCgiFormElementStringClass *class);
static void zak_form_cgi_form_element_string_init (ZakFormCgiFormElementString *zak_form_cgi_form_element_string);

static gchar *zak_form_cgi_form_element_string_render (ZakFormCgiFormElement *element);

static gboolean zak_form_cgi_form_element_string_is_valid (ZakFormCgiFormElement *element);

static void zak_form_cgi_form_element_string_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec);
static void zak_form_cgi_form_element_string_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec);

static void zak_form_cgi_form_element_string_dispose (GObject *gobject);
static void zak_form_cgi_form_element_string_finalize (GObject *gobject);

#define ZAK_FORM_CGI_FORM_ELEMENT_STRING_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_FORM_CGI_TYPE_FORM_ELEMENT_STRING, ZakFormCgiFormElementStringPrivate))

typedef struct _ZakFormCgiFormElementStringPrivate ZakFormCgiFormElementStringPrivate;
struct _ZakFormCgiFormElementStringPrivate
	{
		gchar *str;
	};

G_DEFINE_TYPE (ZakFormCgiFormElementString, zak_form_cgi_form_element_string, ZAK_FORM_CGI_TYPE_FORM_ELEMENT)

static void
zak_form_cgi_form_element_string_class_init (ZakFormCgiFormElementStringClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	ZakFormCgiFormElementClass *elem_class = ZAK_FORM_CGI_FORM_ELEMENT_CLASS (klass);

	object_class->set_property = zak_form_cgi_form_element_string_set_property;
	object_class->get_property = zak_form_cgi_form_element_string_get_property;
	object_class->dispose = zak_form_cgi_form_element_string_dispose;
	object_class->finalize = zak_form_cgi_form_element_string_finalize;

	elem_class->render = zak_form_cgi_form_element_string_render;
	//elem_class->is_valid = zak_form_cgi_form_element_string_is_valid;

	g_type_class_add_private (object_class, sizeof (ZakFormCgiFormElementStringPrivate));
}

static void
zak_form_cgi_form_element_string_init (ZakFormCgiFormElementString *zak_form_cgi_form_element_string)
{
	ZakFormCgiFormElementStringPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_STRING_GET_PRIVATE (zak_form_cgi_form_element_string);

	priv->str = NULL;
}

/**
 * zak_form_cgi_form_element_string_new:
 *
 * Returns: the newly created #ZakFormCgiFormElementString object.
 */
ZakFormCgiFormElement
*zak_form_cgi_form_element_string_new ()
{
	ZakFormCgiFormElementString *zak_form_cgi_form_element_string;

	zak_form_cgi_form_element_string = ZAK_FORM_CGI_FORM_ELEMENT_STRING (g_object_new (zak_form_cgi_form_element_string_get_type (), NULL));

	return ZAK_FORM_CGI_FORM_ELEMENT (zak_form_cgi_form_element_string);
}

/**
 * zak_form_cgi_form_element_string_new_attrs:
 * @str:
 *
 * Returns: the newly created #ZakFormCgiFormElementString object.
 */
ZakFormCgiFormElement
*zak_form_cgi_form_element_string_new_attrs (const gchar *str)
{
	ZakFormCgiFormElement *zak_form_cgi_form_element_string;

	zak_form_cgi_form_element_string = zak_form_cgi_form_element_string_new ();

	ZakFormCgiFormElementStringPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_STRING_GET_PRIVATE (zak_form_cgi_form_element_string);

	priv->str = g_strdup (str);

	return zak_form_cgi_form_element_string;
}

gboolean
zak_form_cgi_form_element_string_xml_parsing (ZakFormElement *element, xmlNodePtr xmlnode)
{
	gboolean ret;

	xmlNode *cur;

	ZakFormCgiFormElementStringPrivate *priv;

	ret = FALSE;

	ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_string_parent_class)->xml_parsing (element, xmlnode);

	cur = xmlnode->children;
	while (cur != NULL)
		{
			if (xmlStrcmp (cur->name, (const xmlChar *)"string") == 0)
				{
					priv = ZAK_FORM_CGI_FORM_ELEMENT_STRING_GET_PRIVATE (ZAK_FORM_CGI_FORM_ELEMENT_STRING (element));
					priv->str = g_strdup ((gchar *)xmlNodeGetContent (cur));
					ret = TRUE;
					break;
				}

			cur = cur->next;
		}

	return ret;
}

static gchar
*zak_form_cgi_form_element_string_render (ZakFormCgiFormElement *element)
{
	gchar *ret;

	ZakFormCgiFormElementStringPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_STRING_GET_PRIVATE (element);

	ret = g_strdup (priv->str);

	return ret;
}

static gboolean
zak_form_cgi_form_element_string_check_value (const gchar *validation_regex, GValue *value)
{
	return TRUE;
}

static gboolean
zak_form_cgi_form_element_string_is_valid (ZakFormCgiFormElement *element)
{
	return TRUE;
}

/* PRIVATE */
static void
zak_form_cgi_form_element_string_set_property (GObject *object,
                   guint property_id,
                   const GValue *value,
                   GParamSpec *pspec)
{
	ZakFormCgiFormElementString *zak_form_cgi_form_element_string = (ZakFormCgiFormElementString *)object;
	ZakFormCgiFormElementStringPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_STRING_GET_PRIVATE (zak_form_cgi_form_element_string);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_cgi_form_element_string_get_property (GObject *object,
                   guint property_id,
                   GValue *value,
                   GParamSpec *pspec)
{
	ZakFormCgiFormElementString *zak_form_cgi_form_element_string = (ZakFormCgiFormElementString *)object;
	ZakFormCgiFormElementStringPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_STRING_GET_PRIVATE (zak_form_cgi_form_element_string);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_cgi_form_element_string_dispose (GObject *gobject)
{
	ZakFormCgiFormElementString *zak_form_cgi_form_element_string = (ZakFormCgiFormElementString *)gobject;
	ZakFormCgiFormElementStringPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_STRING_GET_PRIVATE (zak_form_cgi_form_element_string);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_cgi_form_element_string_finalize (GObject *gobject)
{
	ZakFormCgiFormElementString *zak_form_cgi_form_element_string = (ZakFormCgiFormElementString *)gobject;
	ZakFormCgiFormElementStringPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_STRING_GET_PRIVATE (zak_form_cgi_form_element_string);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}
