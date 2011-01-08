/*
 * main.c
 *
 *  Created on: 2011-1-8
 *      Author: cai
 */
#include <glib.h>
#include <glib-object.h>
#include <libsoup/soup.h>
#include "global.h"
#include "http_server.h"

int main(int argc, char* argv[])
{
	g_type_init();


	//寻找并打开配置文件

	gkeyfile = g_key_file_new();

	start_server();
	/*主循环*/
	GMainLoop * loop = g_main_loop_new(g_main_context_default(),0);
	g_main_loop_run(loop);
	return 0;
}

GKeyFile * gkeyfile = NULL;
