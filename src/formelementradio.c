/*
 * Copyright (C) 2017 Andrea Zagli <azagli@libero.it>
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
#include "formelementradio.h"

static void zak_form_cgi_form_element_radio_class_init (ZakFormCgiFormElementRadioClass *class);
static void zak_form_cgi_form_element_radio_init (ZakFormCgiFormElementRadio *zak_form_cgi_form_element_radio);

static gchar *zak_form_cgi_form_element_radio_render (ZakFormCgiFormElement *element);

static void zak_form_cgi_form_element_radio_set_property (GObject *object,
                                                          guint property_id,
                                                          const GValue *value,
                                                          GParamSpec *pspec);
static void zak_form_cgi_form_element_radio_get_property (GObject *object,
                                                          guint property_id,
                                                          GValue *value,
                                                          GParamSpec *pspec);

static void zak_form_cgi_form_element_radio_dispose (GObject *gobject);
static void zak_form_cgi_form_element_radio_finalize (GObject *gobject);

#define ZAK_FORM_CGI_FORM_ELEMENT_RADIO_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_FORM_CGI_TYPE_FORM_ELEMENT_RADIO, ZakFormCgiFormElementRadioPrivate))

typedef struct _ZakFormCgiFormElementRadioPrivate ZakFormCgiFormElementRadioPrivate;
struct _ZakFormCgiFormElementRadioPrivate
	{
		gboolean in_line;
		GHashTable *ht_options;
	};

G_DEFINE_TYPE (ZakFormCgiFormElementRadio, zak_form_cgi_form_element_radio, ZAK_FORM_CGI_TYPE_FORM_ELEMENT)

static void
zak_form_cgi_form_element_radio_class_init (ZakFormCgiFormElementRadioClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	ZakFormCgiFormElementClass *elem_class = ZAK_FORM_CGI_FORM_ELEMENT_CLASS (klass);

	object_class->set_property = zak_form_cgi_form_element_radio_set_property;
	object_class->get_property = zak_form_cgi_form_element_radio_get_property;
	object_class->dispose = zak_form_cgi_form_element_radio_dispose;
	object_class->finalize = zak_form_cgi_form_element_radio_finalize;

	elem_class->render = zak_form_cgi_form_element_radio_render;

	g_type_class_add_private (object_class, sizeof (ZakFormCgiFormElementRadioPrivate));
}

static void
zak_form_cgi_form_element_radio_init (ZakFormCgiFormElementRadio *zak_form_cgi_form_element_radio)
{
	ZakFormCgiFormElementRadioPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_RADIO_GET_PRIVATE (zak_form_cgi_form_element_radio);

	priv->in_line = TRUE;
	priv->ht_options = g_hash_table_new (g_str_hash, g_str_equal);
}

/**
 * zak_form_cgi_form_element_radio_new:
 *
 * Returns: the newly created #ZakFormCgiFormElementRadio object.
 */
ZakFormCgiFormElement
*zak_form_cgi_form_element_radio_new ()
{
	ZakFormCgiFormElementRadio *zak_form_cgi_form_element_radio;

	zak_form_cgi_form_element_radio = ZAK_FORM_CGI_FORM_ELEMENT_RADIO (g_object_new (zak_form_cgi_form_element_radio_get_type (), NULL));

	return ZAK_FORM_CGI_FORM_ELEMENT (zak_form_cgi_form_element_radio);
}

/**
 * zak_form_cgi_form_element_radio_new_attrs:
 * @id:
 * @...:
 *
 * Returns: the newly created #ZakFormCgiFormElementRadio object.
 */
ZakFormCgiFormElement
*zak_form_cgi_form_element_radio_new_attrs (const gchar *id,
                                            ...)
{
	va_list ap;

	GHashTable *ht_attrs;

	ZakFormCgiFormElement *zak_form_cgi_form_element_radio;

	zak_form_cgi_form_element_radio = zak_form_cgi_form_element_radio_new ();

	va_start (ap, id);

	ht_attrs = zak_cgi_commons_valist_to_ghashtable (ap);
	g_hash_table_replace (ht_attrs, "type", "radio");

	ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_radio_parent_class)->construct (zak_form_cgi_form_element_radio,
		                     id,
		                     ht_attrs);

	return zak_form_cgi_form_element_radio;
}

gboolean
zak_form_cgi_form_element_radio_get_in_line (ZakFormCgiFormElementRadio *element)
{
	ZakFormCgiFormElementRadioPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_RADIO_GET_PRIVATE (element);

	return priv->in_line;
}

void
zak_form_cgi_form_element_radio_set_in_line (ZakFormCgiFormElementRadio *element, gboolean in_line)
{
	ZakFormCgiFormElementRadioPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_RADIO_GET_PRIVATE (element);

	priv->in_line = in_line;
}

gboolean
zak_form_cgi_form_element_radio_xml_parsing (ZakFormElement *element, xmlNodePtr xmlnode)
{
	gboolean ret;

	gchar *id;

	GHashTable *ht_attrs;

	xmlNode *cur;

	ZakFormCgiFormElementRadioPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_RADIO_GET_PRIVATE (element);

	id = NULL;

	ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_radio_parent_class)->xml_parsing (element, xmlnode);

	ht_attrs = g_hash_table_new (g_str_hash, g_str_equal);
	g_hash_table_replace (ht_attrs, "type", "radio");

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
			else if (xmlStrEqual (cur->name, (const xmlChar *)"zak-cgi-inline"))
				{
					zak_form_cgi_form_element_radio_set_in_line (ZAK_FORM_CGI_FORM_ELEMENT_RADIO (element),
					                                             (gboolean)xmlStrEqual (xmlNodeGetContent (cur), (const xmlChar *)"TRUE"));
				}
			else if (xmlStrEqual (cur->name, (const xmlChar *)"zak-cgi-options"))
				{
					xmlNode *xnode;

					xnode = cur->children;
					while (xnode != NULL)
						{
							if (xmlStrcmp (xnode->name, (const xmlChar *)"option") == 0)
								{
									g_hash_table_insert (priv->ht_options,
									                     (gpointer)xmlGetProp (xnode, "value"),
									                     (gpointer)xmlGetProp (xnode, "content"));
								}

							xnode = xnode->next;
						}
				}
			else
				{
					g_hash_table_replace (ht_attrs, g_strdup (cur->name), (gchar *)xmlNodeGetContent (cur));
				}

			cur = cur->next;
		}

	if (id != NULL)
		{
			ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_radio_parent_class)->construct (ZAK_FORM_CGI_FORM_ELEMENT (element),
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
*zak_form_cgi_form_element_radio_render (ZakFormCgiFormElement *element)
{
	gchar *ret_value;
	GString *ret;

	GHashTableIter iter;
	gpointer key;
	gpointer value;

	GHashTable *ht_attrs;
	GHashTable *ht_attrs_option;

	gchar *element_value;

	guint new_id;

	ZakFormCgiFormElementClass *klass;

	ZakFormCgiFormElementRadioPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_RADIO_GET_PRIVATE (element);

	ret = g_string_new ("");

	klass = (ZakFormCgiFormElementClass *)g_type_class_peek_parent (ZAK_FORM_CGI_FORM_ELEMENT_RADIO_GET_CLASS (ZAK_FORM_CGI_FORM_ELEMENT_RADIO (element)));

	ht_attrs = klass->get_ht_attrs (element);

	element_value = zak_form_element_get_value (ZAK_FORM_ELEMENT (element));

	new_id = 0;

	g_hash_table_iter_init (&iter, priv->ht_options);
	while (g_hash_table_iter_next (&iter, &key, &value))
		{
			gchar *attr_class;

			ht_attrs_option = g_hash_table_new (g_str_hash, g_str_equal);
			zak_utils_ghashtable_copy (ht_attrs, ht_attrs_option);

			g_hash_table_insert (ht_attrs_option, (gpointer)"value", (gpointer)g_strdup (key));

			if (element_value != NULL
			    && g_strcmp0 (element_value, key) == 0)
				{
					g_hash_table_insert (ht_attrs_option, (gpointer)"checked", (gpointer)"checked");
				}

			attr_class = g_hash_table_lookup (ht_attrs_option, "class");
			if (attr_class != NULL)
				{
					g_hash_table_insert (ht_attrs_option, "class", g_strjoinv ("", g_strsplit (attr_class, "form-control", -1)));
					g_free (attr_class);
				}

			g_string_append_printf (ret, "\n%s<label%s>\n%s %s</label>%s",
			                        priv->in_line ? "" : "<div class=\"radio\">\n",
			                        priv->in_line ? " class=\"radio-inline\"" : "",
			                        zak_cgi_tag_tag_ht ("input",
			                                            g_strdup_printf ("%s_%d",
			                                                              zak_form_cgi_form_element_get_id (element),
			                                                             ++new_id),
			                                            ht_attrs_option),
			                        value,
			                        priv->in_line ? "" : "\n</div><br/>");

			g_hash_table_unref (ht_attrs_option);
		}

	ret_value = g_strdup (ret->str);
	g_string_free (ret, TRUE);

	return ret_value;
}

/* PRIVATE */
static void
zak_form_cgi_form_element_radio_set_property (GObject *object,
                                              guint property_id,
                                              const GValue *value,
                                              GParamSpec *pspec)
{
	ZakFormCgiFormElementRadio *zak_form_cgi_form_element_radio = (ZakFormCgiFormElementRadio *)object;
	ZakFormCgiFormElementRadioPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_RADIO_GET_PRIVATE (zak_form_cgi_form_element_radio);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_cgi_form_element_radio_get_property (GObject *object,
                                              guint property_id,
                                              GValue *value,
                                              GParamSpec *pspec)
{
	ZakFormCgiFormElementRadio *zak_form_cgi_form_element_radio = (ZakFormCgiFormElementRadio *)object;
	ZakFormCgiFormElementRadioPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_RADIO_GET_PRIVATE (zak_form_cgi_form_element_radio);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_cgi_form_element_radio_dispose (GObject *gobject)
{
	ZakFormCgiFormElementRadio *zak_form_cgi_form_element_radio = (ZakFormCgiFormElementRadio *)gobject;
	ZakFormCgiFormElementRadioPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_RADIO_GET_PRIVATE (zak_form_cgi_form_element_radio);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_cgi_form_element_radio_finalize (GObject *gobject)
{
	ZakFormCgiFormElementRadio *zak_form_cgi_form_element_radio = (ZakFormCgiFormElementRadio *)gobject;
	ZakFormCgiFormElementRadioPrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_RADIO_GET_PRIVATE (zak_form_cgi_form_element_radio);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}
