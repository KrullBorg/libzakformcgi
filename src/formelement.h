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

#ifndef __ZAK_FORM_CGI_FORM_ELEMENT_H__
#define __ZAK_FORM_CGI_FORM_ELEMENT_H__


#include <glib-object.h>

#include <libzakform/libzakform.h>


G_BEGIN_DECLS


#define ZAK_FORM_CGI_TYPE_FORM_ELEMENT zak_form_cgi_form_element_get_type ()
G_DECLARE_DERIVABLE_TYPE (ZakFormCgiFormElement, zak_form_cgi_form_element, ZAK_FORM_CGI, FORM_ELEMENT, ZakFormElement)

struct _ZakFormCgiFormElementClass
	{
		ZakFormElementClass parent_class;

		void (*construct) (ZakFormCgiFormElement *element, const gchar *id, GHashTable *ht_attrs);
		GHashTable *(*get_ht_attrs) (ZakFormCgiFormElement *element);

		void (*xml_parsing) (ZakFormElement *element, xmlNode *xmlnode);

		gchar *(*render) (ZakFormCgiFormElement *element);
	};


gchar *zak_form_cgi_form_element_get_id (ZakFormCgiFormElement *element);

void zak_form_cgi_form_element_set_label (ZakFormCgiFormElement *element, const gchar *label, ...);

void zak_form_cgi_form_element_bind (ZakFormCgiFormElement *element, const gchar *value);

gchar *zak_form_cgi_form_element_render (ZakFormCgiFormElement *element);


G_END_DECLS


#endif /* __ZAK_FORM_CGI_FORM_ELEMENT_H__ */
