/*
 * html_path_input_student_info.c
 *
 *  Created on: 2011-1-10
 *      Author: cai
 */

/*
 * html_path_dformjs.c
 *
 * 		 动态创建显示用的 JS 脚本
 *
 *      Copyright 2010 薇菜工作室
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <unistd.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <libsoup/soup.h>
#include "http_server.h"
#include "global.h"
#include "htmlnode.h"
#include "html_paths.h"

void SoupServer_path_input_student_info(SoupServer *server, SoupMessage *msg,
		const char *path, GHashTable *query, SoupClientContext *client,
		gpointer user_data)
{

}
