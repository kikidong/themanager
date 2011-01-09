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
	GList * keys,*values;
	char* ID,*name,*class,*mj;
	int item=0;

	ID = g_hash_table_lookup(query,"stuid");
	name = g_hash_table_lookup(query,"name");
	class = g_hash_table_lookup(query,"major");
	mj = g_hash_table_lookup(query,"class");

	if( !ID )
	{
		return SoupServer_path_404(server,msg,"/input.html",query,client,user_data);
	}

	name = soup_uri_decode(name);
	ID = soup_uri_decode(ID);
	class = soup_uri_decode(class);
	mj = soup_uri_decode(mj);

	//保存到数据库里

	gchar * sql = g_strdup_printf("SELECT ID from student where ID='%s'",ID);

	//如果没有结果就保存结果，如果有就更新嘛

	g_sql_connect_run_query(dbclient,sql,-1);

	if (!g_sql_connect_use_result(dbclient)) //查无此人
	{
		gchar * sql;
		sql = g_strdup_printf("insert into student (ID) values ('%s') ", ID);
		g_sql_connect_run_query(dbclient,sql,-1);
		g_free(sql);
	}

	g_free(sql);

//		如果有名字，就更新名字，如果有班级就更新班级 etc
	if (name && strlen(name)) {
	sql = g_strdup_printf("update student set name='%s' where ID='%s'",
			name, ID);
	g_sql_connect_run_query(dbclient,sql,-1);
	g_free(sql);
	}

	if (class && strlen(class)) {
	sql = g_strdup_printf("update student set class='%s' where ID='%s'",
			class, ID);
	g_sql_connect_run_query(dbclient,sql,-1);
	g_free(sql);
	}

	if (mj && strlen(mj)) {
	sql = g_strdup_printf("update student set mj='%s' where ID='%s'",
			mj, ID);
	g_sql_connect_run_query(dbclient,sql,-1);
	g_free(sql);
	}

	g_free(name);
	g_free(class);
	g_free(mj);


	//现在到了更新成绩的时候啦
	item++;

	char lessonname[80];
	char scorename[80];

	sprintf(lessonname,"lessname%d",item);
	sprintf(scorename,"score%d",item);

	gchar * score,*lesson;

	while( (lesson =  g_hash_table_lookup(query,lessonname) ) && ( score = g_hash_table_lookup(query,scorename)))
	{
		//查询是否添加了这门课，呵呵
		lesson = soup_uri_decode(lesson);
		score = soup_uri_decode(score);

		sql = g_strdup_printf("select * from lesson where stuid is null and name='%s'",lesson);

		g_debug("%s",sql);

		g_sql_connect_run_query(dbclient,sql,-1);

		if(g_sql_connect_use_result(dbclient))
		{
			gchar * sql ;

			sql = g_strdup_printf("select * from lesson where stuid='%s' and name='%s'",ID,lesson);
			g_sql_connect_run_query(dbclient,sql,-1);
			g_free(sql);

			if(g_sql_connect_use_result(dbclient)) //已经有此课成绩了，更新
			{
				sql = g_strdup_printf("update lesson set score='%s' where stuid='%s' and name='%s'",score,ID,lesson);
				g_sql_connect_run_query(dbclient,sql,-1);
				g_free(sql);

			}else //添加
			{
				sql = g_strdup_printf("insert into lesson (name,score,stuid) values ('%s','%s','%s')",lesson,score,ID);
				g_sql_connect_run_query(dbclient,sql,-1);
				g_free(sql);
			}
		}

		g_free(sql);

		item ++;
		sprintf(lessonname,"lessname%d",item);
		sprintf(scorename,"score%d",item);
	}

	g_free(ID);

	//继续输入
	return SoupServer_path_static_file(server,msg,"/input.html",query,client,user_data);
}
