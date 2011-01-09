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

char pre_option[] = "var items = 1;"
"function AddItem() {"
"   if (!document.getElementById) return;"
""
"   div = document.getElementById(\"items\");"
"   button = document.getElementById(\"inputwriteroot\");"
"   items++;"
""
"   newitem = \"<b ><select name=\\\"lessname\\\">\";"
"";
/*
char * option = "   newitem += \"<option></option>\";"
"   newitem += \"<option></option>\";";
*/
char post_option[]  = "   newitem += \"</select> </b> \";"
""
"	newitem += \"<b>\";"
"   newitem += \"<input type=\\\"text\\\" name=\\\"score\" + items + \"\\\"/>\";"
"   newitem += \"</b>\";"
"   newitem += \"<br />\";"
""
"   newnode = document.createElement(\"span\");"
"   newnode.innerHTML = newitem;"
"   div.insertBefore(newnode,button);"
"} ";


void SoupServer_path_dformjs(SoupServer *server, SoupMessage *msg,
		const char *path, GHashTable *query, SoupClientContext *client,
		gpointer user_data)
{
	if(!g_sql_connect_run_query(dbclient,"select * from lesson where stuid is null",-1))
	{
		return SoupServer_path_404(server,msg,path,query,client,user_data);
	}

	GSQLResult * result = g_sql_connect_use_result(dbclient);


	soup_message_body_append(msg->response_body,SOUP_MEMORY_STATIC,pre_option,sizeof(pre_option)-1);


	while (g_sql_result_fetch_row(result))
	{
		soup_message_body_append(msg->response_body,SOUP_MEMORY_STATIC,"   newitem +=\" <option>",sizeof("   newitem +=\" <option>")-1);

		soup_message_body_append(msg->response_body,SOUP_MEMORY_COPY,g_sql_result_colum_by_name(result,"name"),strlen(g_sql_result_colum_by_name(result,"name")));

		soup_message_body_append(msg->response_body,SOUP_MEMORY_STATIC,"</option>\";\n",sizeof("</option>\";\n")-1);

	}

	soup_message_body_append(msg->response_body,SOUP_MEMORY_STATIC,post_option,sizeof(post_option)-1);

	g_object_unref(result);

	soup_message_set_status(msg,SOUP_STATUS_OK);
	soup_message_headers_set_encoding(msg->response_headers,SOUP_ENCODING_CONTENT_LENGTH);
	soup_message_headers_set_content_type(msg->response_headers,"text/javascript",0);

	soup_message_body_complete(msg->response_body);
}
