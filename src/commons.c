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

#include "commons.h"

#include <libzakcgi/libzakcgi.h>
#include <libzakformini/libzakformini.h>

static ZakFormIniProvider
*zak_form_cgi_session_get_form_ini_provider (ZakCgiSession *session)
{
    return zak_form_ini_provider_new_from_gkeyfile (zak_cgi_session_get_gkeyfile (session),
												    g_file_get_path (zak_cgi_session_get_gfile (session)));
}

/**
 * zak_form_cgi_session_set_from_form:
 * @session:
 * @form:
 *
 */
void
zak_form_cgi_session_set_from_form (ZakCgiSession *session, ZakFormForm *form)
{
	zak_form_form_insert (ZAK_FORM_FORM (form), ZAK_FORM_IPROVIDER (zak_form_cgi_session_get_form_ini_provider (session)));
}

/**
 * zak_form_cgi_session_fill_form:
 * @session:
 * @form:
 *
 */
void
zak_form_cgi_session_fill_form (ZakCgiSession *session, ZakFormForm *form)
{
	zak_form_form_load (ZAK_FORM_FORM (form), ZAK_FORM_IPROVIDER (zak_form_cgi_session_get_form_ini_provider (session)));
}
