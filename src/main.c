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
#include <libsoup/soup.h>
#include <sqlite3.h>

#include "global.h"
#include "http_server.h"

int main(int argc, char* argv[])
{
	g_type_init();


	//寻找并打开配置文件

	gkeyfile = g_key_file_new();


	//打开数据库

	sqlite3_open(PACKAGE_NAME ".sqlite",&db);

	start_server();
	/*主循环*/
	GMainLoop * loop = g_main_loop_new(g_main_context_default(),0);
	g_main_loop_run(loop);
	return 0;
}

sqlite3	* db;
GKeyFile * gkeyfile = NULL;
