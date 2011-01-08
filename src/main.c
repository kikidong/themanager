/*
 * main.c
 *
 *  Created on: 2011-1-8
 *      Author: cai
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <glib.h>
#include <glib-object.h>
#include <libnotify/notify.h>

#include "sqlclient/gsqlconnect.h"
#include "sqlclient/gsqlresult.h"

#include "global.h"
#include "http_server.h"

int main(int argc, char* argv[])
{
	g_type_init();
	notify_init(PACKAGE_NAME);

	//寻找并打开配置文件

	gkeyfile = g_key_file_new();


	//创建数据库访问对象
	dbclient = sqlconnect_new();
	//设置各种参数

	g_object_set(dbclient, "file", PACKAGE_NAME ".sqlite", NULL);

	//打开数据库
	g_sql_connect_real_connect(dbclient,0);


	//检查数据库内容

	start_server(8000);
	/*主循环*/
	GMainLoop * loop = g_main_loop_new(g_main_context_default(),0);
	g_main_loop_run(loop);
	return 0;
}

GSQLConnect	* dbclient;
GKeyFile * gkeyfile = NULL;
