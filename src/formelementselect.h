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

#ifndef __ZAK_FORM_CGI_FORM_ELEMENT_SELECT_H__
#define __ZAK_FORM_CGI_FORM_ELEMENT_SELECT_H__


#include <glib-object.h>

#include <libxml/tree.h>

#include "formelement.h"


G_BEGIN_DECLS


#define ZAK_FORM_CGI_TYPE_FORM_ELEMENT_SELECT                 (zak_form_cgi_form_element_select_get_type ())
#define ZAK_FORM_CGI_FORM_ELEMENT_SELECT(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), ZAK_FORM_CGI_TYPE_FORM_ELEMENT_SELECT, ZakFormCgiFormElementSelect))
#define ZAK_FORM_CGI_FORM_ELEMENT_SELECT_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), ZAK_FORM_CGI_TYPE_FORM_ELEMENT_SELECT, ZakFormCgiFormElementSelectClass))
#define ZAK_FORM_CGI_IS_FORM_ELEMENT_SELECT(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ZAK_FORM_CGI_TYPE_FORM_ELEMENT_SELECT))
#define ZAK_FORM_CGI_IS_FORM_ELEMENT_SELECT_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), ZAK_FORM_CGI_TYPE_FORM_ELEMENT_SELECT))
#define ZAK_FORM_CGI_FORM_ELEMENT_SELECT_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), ZAK_FORM_CGI_TYPE_FORM_ELEMENT_SELECT, ZakFormCgiFormElementSelectClass))

typedef struct _ZakFormCgiFormElementSelect ZakFormCgiFormElementSelect;
typedef struct _ZakFormCgiFormElementSelectClass ZakFormCgiFormElementSelectClass;

struct _ZakFormCgiFormElementSelect
	{
		ZakFormCgiFormElement parent_instance;
	};

struct _ZakFormCgiFormElementSelectClass
	{
		ZakFormCgiFormElementClass parent_class;
	};


GType zak_form_cgi_form_element_select_get_type (void);

ZakFormCgiFormElement *zak_form_cgi_form_element_select_new (void);
ZakFormCgiFormElement *zak_form_cgi_form_element_select_new_attrs (const gchar *id, ...);

void zak_form_cgi_form_element_select_add_option (ZakFormCgiFormElementSelect *element, const gchar *value, const gchar *content, ...);

void zak_form_cgi_form_element_select_xml_parsing (ZakFormElement *element, xmlNodePtr xmlnode);


G_END_DECLS


#endif /* __ZAK_FORM_CGI_FORM_ELEMENT_SELECT_H__ */
