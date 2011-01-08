/*
 * html_path_stulist.c  --
 *
 *
 * 		动态创建学生列表。
 *
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
 *
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


void SoupServer_path_stulist(SoupServer *server, SoupMessage *msg,
		const char *path, GHashTable *query, SoupClientContext *client,
		gpointer user_data)
{

	if(!g_sql_connect_run_query(dbclient,"SELECT * from student",-1));
	{
		soup_message_set_status(msg,SOUP_STATUS_BAD_REQUEST);
		return ;
	}

	HtmlNode * html = htmlnode_new(NULL,"html",NULL);

	htmlnode_new_head(html,"http-equiv=\"content-type\"","content=\"text/html;charset=utf-8\"",NULL);




	GSQLResult * result = g_sql_connect_use_result(dbclient);

	if(result) //数据库有内容
	{
		//循环获得每一行
		while(g_sql_result_get_row(result))
		{
			//构建页面

			const gchar * ID = g_sql_result_colum_by_name(result,"ID");



		}
	}


	soup_message_set_status(msg,SOUP_STATUS_OK);

	htmlnode_to_plane_text_and_free(html,
			(htmlnode_appender) soup_message_body_appender, msg->response_body);

	soup_message_body_complete(msg->response_body);
}
