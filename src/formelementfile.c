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

#include <glib/gi18n-lib.h>

#include <libzakcgi/libzakcgi.h>
#include "formelementfile.h"

static void zak_form_cgi_form_element_file_class_init (ZakFormCgiFormElementFileClass *class);
static void zak_form_cgi_form_element_file_init (ZakFormCgiFormElementFile *zak_form_cgi_form_element_file);

static gchar *zak_form_cgi_form_element_file_render (ZakFormCgiFormElement *element);

static void zak_form_cgi_form_element_file_set_property (GObject *object,
                                                         guint property_id,
                                                         const GValue *value,
                                                         GParamSpec *pspec);
static void zak_form_cgi_form_element_file_get_property (GObject *object,
                                                         guint property_id,
                                                         GValue *value,
                                                         GParamSpec *pspec);

static void zak_form_cgi_form_element_file_dispose (GObject *gobject);
static void zak_form_cgi_form_element_file_finalize (GObject *gobject);

static gboolean zak_form_cgi_form_element_file_after_validating (ZakFormElement *element, GPtrArray *ar_messages);

#define ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ZAK_FORM_CGI_TYPE_FORM_ELEMENT_FILE, ZakFormCgiFormElementFilePrivate))

typedef struct _ZakFormCgiFormElementFilePrivate ZakFormCgiFormElementFilePrivate;
struct _ZakFormCgiFormElementFilePrivate
	{
		guint size_min;
		guint size_max;
		gchar *mime_type;
		gchar *mime_type_message;
	};

G_DEFINE_TYPE (ZakFormCgiFormElementFile, zak_form_cgi_form_element_file, ZAK_FORM_CGI_TYPE_FORM_ELEMENT)

static void
zak_form_cgi_form_element_file_class_init (ZakFormCgiFormElementFileClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	ZakFormElementClass *elem_class = ZAK_FORM_ELEMENT_CLASS (klass);
	ZakFormCgiFormElementClass *cgi_elem_class = ZAK_FORM_CGI_FORM_ELEMENT_CLASS (klass);

	object_class->set_property = zak_form_cgi_form_element_file_set_property;
	object_class->get_property = zak_form_cgi_form_element_file_get_property;
	object_class->dispose = zak_form_cgi_form_element_file_dispose;
	object_class->finalize = zak_form_cgi_form_element_file_finalize;

	elem_class->xml_parsing = zak_form_cgi_form_element_file_xml_parsing;

	cgi_elem_class->render = zak_form_cgi_form_element_file_render;

	g_type_class_add_private (object_class, sizeof (ZakFormCgiFormElementFilePrivate));
}

static void
zak_form_cgi_form_element_file_init (ZakFormCgiFormElementFile *zak_form_cgi_form_element_file)
{
	ZakFormCgiFormElementFilePrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE (zak_form_cgi_form_element_file);

	priv->size_min = 0;
	priv->size_max = 0;
	priv->mime_type = NULL;
	priv->mime_type_message = NULL;
}

/**
 * zak_form_cgi_form_element_file_new:
 *
 * Returns: the newly created #ZakFormCgiFormElementFile object.
 */
ZakFormCgiFormElement
*zak_form_cgi_form_element_file_new ()
{
	ZakFormCgiFormElementFile *zak_form_cgi_form_element_file;

	zak_form_cgi_form_element_file = ZAK_FORM_CGI_FORM_ELEMENT_FILE (g_object_new (zak_form_cgi_form_element_file_get_type (), NULL));

	g_signal_connect (G_OBJECT (zak_form_cgi_form_element_file), "after-validating",
	                  G_CALLBACK (zak_form_cgi_form_element_file_after_validating), NULL);

	return ZAK_FORM_CGI_FORM_ELEMENT (zak_form_cgi_form_element_file);
}

/**
 * zak_form_cgi_form_element_file_new_attrs:
 * @id:
 * @...:
 *
 * Returns: the newly created #ZakFormCgiFormElementFile object.
 */
ZakFormCgiFormElement
*zak_form_cgi_form_element_file_new_attrs (const gchar *id,
                                           ...)
{
	va_list ap;

	ZakFormCgiFormElement *zak_form_cgi_form_element_file;

	zak_form_cgi_form_element_file = zak_form_cgi_form_element_file_new ();

	va_start (ap, id);

	ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_file_parent_class)->construct (zak_form_cgi_form_element_file,
		                    id,
		                    zak_cgi_commons_valist_to_ghashtable (ap));

	return zak_form_cgi_form_element_file;
}

void
zak_form_cgi_form_element_file_xml_parsing (ZakFormElement *element, xmlNodePtr xmlnode)
{
	gchar *id;

	GHashTable *ht_attrs;

	xmlNode *cur;

	id = NULL;

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
			else if (xmlStrcmp (cur->name, (const xmlChar *)"zak-cgi-size-min") == 0)
				{
					zak_form_cgi_form_element_file_set_size_min (element, strtol ((gchar *)xmlNodeGetContent (cur), NULL, 10));
				}
			else if (xmlStrcmp (cur->name, (const xmlChar *)"zak-cgi-size-max") == 0)
				{
					zak_form_cgi_form_element_file_set_size_max (element, strtol ((gchar *)xmlNodeGetContent (cur), NULL, 10));
				}
			else if (xmlStrcmp (cur->name, (const xmlChar *)"zak-cgi-mime-type") == 0)
				{
					zak_form_cgi_form_element_file_set_mime_type (element, (gchar *)xmlNodeGetContent (cur));
					zak_form_cgi_form_element_file_set_mime_type_message (element, (gchar *)xmlGetProp (cur, (const xmlChar *)"message"));
				}
			else
				{
					g_hash_table_replace (ht_attrs, g_strdup ((gchar *)cur->name), (gchar *)xmlNodeGetContent (cur));
				}

			cur = cur->next;
		}

	if (id != NULL)
		{
			ZAK_FORM_CGI_FORM_ELEMENT_CLASS (zak_form_cgi_form_element_file_parent_class)->construct (ZAK_FORM_CGI_FORM_ELEMENT (element),
				                    id,
				                    ht_attrs);
		}
}

void
zak_form_cgi_form_element_file_set_size_min (ZakFormElement *element, guint min)
{
	ZakFormCgiFormElementFilePrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE (element);

	priv->size_min = min;
}

guint
zak_form_cgi_form_element_file_get_size_min (ZakFormElement *element)
{
	ZakFormCgiFormElementFilePrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE (element);

	return priv->size_min;
}

void
zak_form_cgi_form_element_file_set_size_max (ZakFormElement *element, guint max)
{
	ZakFormCgiFormElementFilePrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE (element);

	priv->size_max = max;
}

guint
zak_form_cgi_form_element_file_get_size_max (ZakFormElement *element)
{
	ZakFormCgiFormElementFilePrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE (element);

	return priv->size_max;
}

void
zak_form_cgi_form_element_file_set_size_range (ZakFormElement *element, guint min, guint max)
{
	zak_form_cgi_form_element_file_set_size_min (element, min);
	zak_form_cgi_form_element_file_set_size_max (element, max);
}

void
zak_form_cgi_form_element_file_set_mime_type (ZakFormElement *element, const gchar *mime_type)
{
	ZakFormCgiFormElementFilePrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE (element);

	if (priv->mime_type != NULL)
		{
			g_free (priv->mime_type);
			priv->mime_type = NULL;
		}

	if (mime_type != NULL)
		{
			priv->mime_type = g_strdup (mime_type);
		}
}

gchar
*zak_form_cgi_form_element_file_get_mime_type (ZakFormElement *element)
{
	ZakFormCgiFormElementFilePrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE (element);

	if (priv->mime_type == NULL)
		{
			return NULL;
		}
	else
		{
			return g_strdup (priv->mime_type);
		}
}

void
zak_form_cgi_form_element_file_set_mime_type_message (ZakFormElement *element, const gchar *mime_type_message)
{
	ZakFormCgiFormElementFilePrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE (element);

	if (priv->mime_type_message != NULL)
		{
			g_free (priv->mime_type_message);
			priv->mime_type_message = NULL;
		}

	if (mime_type_message != NULL)
		{
			priv->mime_type_message = g_strdup (mime_type_message);
		}
}

gchar
*zak_form_cgi_form_element_file_get_mime_type_message (ZakFormElement *element)
{
	ZakFormCgiFormElementFilePrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE (element);

	if (priv->mime_type_message == NULL)
		{
			return NULL;
		}
	else
		{
			return g_strdup (priv->mime_type_message);
		}
}

static gchar
*zak_form_cgi_form_element_file_render (ZakFormCgiFormElement *element)
{
	gchar *ret;

	GHashTable *ht_attrs;

	gchar *value;

	ZakFormCgiFormElementClass *klass;

	klass = (ZakFormCgiFormElementClass *)g_type_class_peek_parent (ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_CLASS (ZAK_FORM_CGI_FORM_ELEMENT_FILE (element)));

	ht_attrs = klass->get_ht_attrs (element);

	value = zak_form_element_get_value (ZAK_FORM_ELEMENT (element));
	if (value != NULL)
		{
			g_hash_table_insert (ht_attrs, (gpointer)"value", (gpointer)g_strdup (value));
		}

	ret = zak_cgi_tag_file_ht (zak_form_cgi_form_element_get_id (element), ht_attrs);

	return ret;
}

/* PRIVATE */
static void
zak_form_cgi_form_element_file_set_property (GObject *object,
                                             guint property_id,
                                             const GValue *value,
                                             GParamSpec *pspec)
{
	ZakFormCgiFormElementFile *zak_form_cgi_form_element_file = (ZakFormCgiFormElementFile *)object;
	ZakFormCgiFormElementFilePrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE (zak_form_cgi_form_element_file);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_cgi_form_element_file_get_property (GObject *object,
                                             guint property_id,
                                             GValue *value,
                                             GParamSpec *pspec)
{
	ZakFormCgiFormElementFile *zak_form_cgi_form_element_file = (ZakFormCgiFormElementFile *)object;
	ZakFormCgiFormElementFilePrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE (zak_form_cgi_form_element_file);

	switch (property_id)
		{
			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_form_cgi_form_element_file_dispose (GObject *gobject)
{
	ZakFormCgiFormElementFile *zak_form_cgi_form_element_file = (ZakFormCgiFormElementFile *)gobject;
	ZakFormCgiFormElementFilePrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE (zak_form_cgi_form_element_file);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->dispose (gobject);
}

static void
zak_form_cgi_form_element_file_finalize (GObject *gobject)
{
	ZakFormCgiFormElementFile *zak_form_cgi_form_element_file = (ZakFormCgiFormElementFile *)gobject;
	ZakFormCgiFormElementFilePrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE (zak_form_cgi_form_element_file);



	GObjectClass *parent_class = g_type_class_peek_parent (G_OBJECT_GET_CLASS (gobject));
	parent_class->finalize (gobject);
}

static gboolean
zak_form_cgi_form_element_file_after_validating (ZakFormElement *element, GPtrArray *ar_messages)
{
	gboolean ret;

	ZakCgiFile *cgi_file;

	ZakFormCgiFormElementFilePrivate *priv = ZAK_FORM_CGI_FORM_ELEMENT_FILE_GET_PRIVATE (element);

	ret = FALSE;

	if (priv->size_min > 0
	    || priv->size_max > 0
	    || priv->mime_type != NULL)
		{
			cgi_file = (ZakCgiFile *)g_value_get_boxed (zak_form_element_get_value_gvalue (element));
			if (cgi_file != NULL
			    && cgi_file->content != NULL
			    && cgi_file->name != NULL
			    && g_strcmp0 (cgi_file->name, "") != 0)
				{
					if (priv->size_min > 0
						&& cgi_file->size < priv->size_min)
						{
							g_ptr_array_add (ar_messages, g_strdup_printf (_("The size of «%s» must be greater than %d."),
							                                               zak_form_element_get_long_name (element),
							                                               priv->size_min));
							ret = TRUE;
						}
					if (priv->size_max > 0
					    && cgi_file->size > priv->size_max)
						{
							g_ptr_array_add (ar_messages, g_strdup_printf (_("The size of «%s» cannot be greater than %d."),
							                                               zak_form_element_get_long_name (element),
							                                               priv->size_max));
							ret = TRUE;
						}
					if (priv->mime_type != NULL)
						{
							gchar **splitted;
							guint l;
							guint i;
							gboolean found;
							gchar *mime;

							found = FALSE;

							mime = g_content_type_get_mime_type (cgi_file->content_type);

							if (mime == NULL)
								{
									mime = g_strdup (cgi_file->content_type);
								}

							splitted = g_strsplit (priv->mime_type, "|", -1);
							l = g_strv_length (splitted);
							for (i = 0; i < l; i++)
								{
									if (g_strcmp0 (mime, splitted[i]) == 0)
										{
											found = TRUE;
											break;
										}
								}
							g_strfreev (splitted);

							if (mime != NULL)
								{
									g_free (mime);
								}

							if (!found)
								{
									g_ptr_array_add (ar_messages, g_strdup_printf (_("The file for field «%s» must be of type «%s»."),
									                                               zak_form_element_get_long_name (element),
									                                               priv->mime_type_message != NULL ? priv->mime_type_message : priv->mime_type));
									ret = TRUE;
								}
						}
				}
			else if (priv->size_min > 0)
				{
					g_ptr_array_add (ar_messages, g_strdup_printf (_("The field «%s» cannot be empty."),
					                                               zak_form_element_get_long_name (element)));
					ret = TRUE;
				}
		}

	return ret;
}
