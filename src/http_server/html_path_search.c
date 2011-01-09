/*
 * html_path_search.c

 * 		进阶使用 -- 执行 SQL 语句
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


void SoupServer_path_search(SoupServer *server, SoupMessage *msg,
		const char *path, GHashTable *query, SoupClientContext *client,
		gpointer user_data)
{
	GSQLResult * result;
	gchar * sql=NULL;


	HtmlNode * html = htmlnode_new(NULL,"html",NULL);

	htmlnode_new_head(html,"http-equiv=\"content-type\"","content=\"text/html;charset=utf-8\"",NULL);

	HtmlNode * body = htmlnode_new_body(html,"background=\"eg_bg_06.gif\"",0);

	//首先是一个输入框
	HtmlNode * form = htmlnode_new_form(body,"GET","search.html",0);

	HtmlNode * table = htmlnode_new_table(form,"width=\"100%\"",0);

	HtmlNode * table_tr = htmlnode_new(table,"tr",0);

	HtmlNode * p = htmlnode_new(htmlnode_new(table_tr,"td",0),"p",0);

	htmlnode_new_text(p,"Select ");
	htmlnode_new(p,"input","type=\"text\"","name=\"sql\"","size=\"100%\"",0);

	htmlnode_new(p,"input","type=\"submit\"","name=\"执行\"",0);

	HtmlNode * body_div = htmlnode_new(body,"div",0);

//	首先获得对于的SQL语句
	if(query)
		sql = g_hash_table_lookup(query,"sql");

	if(sql)
		sql =  soup_uri_decode(sql);

	if(sql) //执行 SQL 语句
	{
		gchar * fsql = g_strdup_printf("select %s",sql);
		g_free(sql);
		sql = fsql;
	}else goto outputpage;



	if(!g_sql_connect_run_query(dbclient,sql,-1))
	{
		//输出 SQL 的错误信息
		htmlnode_new_text_printf(htmlnode_new(body_div,"p",0),"%s","Sql 语句错误");

	}else if((result = g_sql_connect_use_result(dbclient)))
	{
		HtmlNode * body_div_table = htmlnode_new(body_div,"table",0);

		HtmlNode * tr = htmlnode_new(body_div_table,"tr",0);


		int i;

		//表头

		for(i=0;i < result->colum->len ; i++)
		{
			htmlnode_new_text_printf(htmlnode_new(tr,"td",0),"%s",g_sql_result_colum_name(result,i));
		}

		tr = htmlnode_new(body_div_table,"tr",0);

		//内容

		while(g_sql_result_fetch_row(result))
		{
			for(i=0;i < result->colum->len ; i++)
			{
				htmlnode_new_text_printf(htmlnode_new(tr,"td",0),"%s",g_sql_result_colum(result,i));
			}

			tr = htmlnode_new(body_div_table,"tr",0);
		}
	}

outputpage:
	soup_message_set_status(msg,SOUP_STATUS_OK);
	soup_message_headers_set_encoding(msg->response_headers,SOUP_ENCODING_CONTENT_LENGTH);

	htmlnode_to_plane_text_and_free(html,
			(htmlnode_appender) soup_message_body_appender, msg->response_body);

	soup_message_body_complete(msg->response_body);
}
