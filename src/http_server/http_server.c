/*
 * http_server.c -- HTTP 服务器实现
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
#include <libnotify/notify.h>


static void SoupServer_path_root(SoupServer *server, SoupMessage *msg,
		const char *path, GHashTable *query, SoupClientContext *client,
		gpointer user_data);

static SoupServer * server;

int start_server(guint port)
{
	GError * err = NULL;

	while (!(server = soup_server_new(SOUP_SERVER_ASYNC_CONTEXT,
			g_main_context_get_thread_default(), "port", port,
			SOUP_SERVER_SERVER_HEADER, PACKAGE_STRING " simple http server", NULL)))
	{
		g_warning(_("server failt to start at port %d, will use random port!"),port);
		port = 0;
	}

	g_debug(_("server started at port %u"),soup_server_get_port(server));

	soup_server_add_handler(server,"/favicon.ico",SoupServer_path_static_file,NULL,NULL);

	soup_server_add_handler(server,"/",SoupServer_path_root,NULL,NULL);

	soup_server_add_handler(server,"/dform.js",SoupServer_path_dformjs,0,0);

	soup_server_add_handler(server,"/input_class_info",SoupServer_path_input_class_info,0,0);
	soup_server_add_handler(server,"/input_student_info",SoupServer_path_input_student_info,0,0);

	soup_server_add_handler(server,"/stulist",SoupServer_path_stulist,NULL,NULL);

	soup_server_run_async(server);

	return soup_server_get_port(server);
}

static void SoupServer_path_root(SoupServer *server, SoupMessage *msg,
		const char *path, GHashTable *query, SoupClientContext *client,
		gpointer user_data)
{
	if(g_strcmp0(path,"/")==0)
	{
		return SoupServer_path_static_file(server,msg,"/index.html",query,client,user_data);
	}

	return SoupServer_path_static_file(server,msg,path,query,client,user_data);
}

void soup_message_body_appender(const gchar * txt, SoupMessageBody * body)
{
	soup_message_body_append(body,SOUP_MEMORY_COPY,txt,strlen(txt));
}
