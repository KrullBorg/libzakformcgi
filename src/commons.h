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

#ifndef __ZAK_FORM_CGI_COMMONS_H__
#define __ZAK_FORM_CGI_COMMONS_H__


#include <libzakcgi/libzakcgi.h>
#include <libzakform/libzakform.h>


void zak_form_cgi_session_set_from_form (ZakCgiSession *session, ZakFormForm *form);
void zak_form_cgi_session_fill_form (ZakCgiSession *session, ZakFormForm *form);


#endif /* __ZAK_FORM_CGI_COMMONS_H__ */
