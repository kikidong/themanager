/*
 * html_path_input_class_info.c
 *
 * 	这个文件用来接受课程的输入
 *
 *      Copyright 2011 薇菜工作室
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

void SoupServer_path_input_class_info(SoupServer *server, SoupMessage *msg,
		const char *path, GHashTable *query, SoupClientContext *client,
		gpointer user_data)
{
	const gchar * classname;

	//接收用户的输入
	classname = g_hash_table_lookup(query,"classname");

	if(classname  && g_utf8_strlen(classname,512))
	{
		g_debug("课程名:%s",classname);

		gchar * sql = g_strdup_printf("select * from lesson where name='%s'",classname);

		if( g_sql_connect_run_query(dbclient,sql,-1))
		{
			//已经存在这样的表格了
			if(g_sql_connect_use_result(dbclient))
			{
				g_free(sql);
				//返回已经存在的页面
				return SoupServer_path_static_file(server,msg,"/classnameexist.html",query,client,user_data);
			}

			//把课程存入数据库
			gchar * sql = g_strdup_printf("insert into lesson (name) VALUES ('%s')",classname);

			if( g_sql_connect_run_query(dbclient,sql,-1))
			{
				g_free(sql);

				g_debug("已经存入数据库");

				return SoupServer_path_static_file(server,msg,"/class.html",query,client,user_data);
			}
			g_free(sql);
		}
		g_free(sql);
	}
	return SoupServer_path_404(server,msg,path,query,client,user_data);
}
