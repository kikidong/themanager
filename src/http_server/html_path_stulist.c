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

	if(!g_sql_connect_run_query(dbclient,"SELECT * from student",-1))
	{
		soup_message_set_status(msg,SOUP_STATUS_BAD_REQUEST);
		return ;
	}

	HtmlNode * html = htmlnode_new(NULL,"html",NULL);

	htmlnode_new_head(html,"http-equiv=\"content-type\"","content=\"text/html;charset=utf-8\"",NULL);


	GSQLResult * result = g_sql_connect_use_result(dbclient);

	HtmlNode * body = htmlnode_new_body(html,"background=\"eg_bg_06.gif\"",0);

	HtmlNode * body_div = htmlnode_new(body,"div",0);

	HtmlNode * body_div_table = htmlnode_new(body_div,"table",0);

	HtmlNode * tr = htmlnode_new(body_div_table,"tr",0);

	htmlnode_new_text_printf(htmlnode_new(tr,"td",0),"%s","专业");
	htmlnode_new_text_printf(htmlnode_new(tr,"td",0),"%s","班级");
	htmlnode_new_text_printf(htmlnode_new(tr,"td",0),"%s","学号");
	htmlnode_new_text_printf(htmlnode_new(tr,"td",0),"%s","姓名");

	htmlnode_new(body_div,"br",0);

	if(result) //数据库有内容
	{
		g_object_ref(result);
		tr = htmlnode_new(body_div_table,"tr",0);

		//循环获得每一行
		while(g_sql_result_fetch_row(result))
		{
			//构建页面

			const gchar * ID = g_sql_result_colum_by_name(result,"ID");

			const gchar * name = g_sql_result_colum_by_name(result,"name");

			const gchar * mj = g_sql_result_colum_by_name(result,"mj");

			const gchar * class = g_sql_result_colum_by_name(result,"class");


			htmlnode_new_text_printf(htmlnode_new(tr,"td",0),"%s",mj);
			htmlnode_new_text_printf(htmlnode_new(tr,"td",0),"%s",class);
			htmlnode_new_text_printf(htmlnode_new(tr,"td",0),"%s",ID);
			htmlnode_new_text_printf(htmlnode_new(tr,"td",0),"%s",name);


			//有成绩嘛就显示成绩

			gchar * sql ;
			GSQLResult * res;

			sql = g_strdup_printf("select * from lesson where stuid='%s'",ID);

			g_sql_connect_run_query(dbclient,sql,-1);

			if((res =g_sql_connect_use_result(dbclient)))
			{
				while(g_sql_result_fetch_row(res))
				{
					htmlnode_new_text_printf(htmlnode_new(tr,"td",0),"科目:%s	分数:%s",
							g_sql_result_colum_by_name(res,"name"),g_sql_result_colum_by_name(res,"score"));
				}
				g_object_unref(res);
			}

			g_free(sql);

			tr = htmlnode_new(body_div_table,"tr",0);
		}


		g_object_unref(result);
	}


	soup_message_set_status(msg,SOUP_STATUS_OK);
	soup_message_headers_set_encoding(msg->response_headers,SOUP_ENCODING_CONTENT_LENGTH);

	htmlnode_to_plane_text_and_free(html,
			(htmlnode_appender) soup_message_body_appender, msg->response_body);

	soup_message_body_complete(msg->response_body);
}
