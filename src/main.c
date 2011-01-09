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
#include <glib/gi18n.h>
#include <libnotify/notify.h>

#include "sqlclient/gsqlconnect.h"
#include "sqlclient/gsqlresult.h"

#include "global.h"
#include "http_server.h"

static void myLog(const gchar *log_domain, GLogLevelFlags log_level,
		const gchar *message, gpointer user_data);


static struct timeval	start_time;


int main(int argc, char* argv[])
{
	gettimeofday(&start_time,0);

	g_type_init();
	notify_init(PACKAGE_NAME);

	g_set_application_name("学生成绩管理系统");

	g_log_set_default_handler(myLog,NULL);


	//寻找并打开配置文件

	gkeyfile = g_key_file_new();

	//创建数据库访问对象
	dbclient = sqlconnect_new();
	//设置各种参数

	g_object_set(dbclient, "file", PACKAGE_NAME ".sqlite", NULL);

	//打开数据库
	g_sql_connect_real_connect(dbclient,0);

	//开启服务
	start_server(8000);
	/*主循环*/
	GMainLoop * loop = g_main_loop_new(g_main_context_default(),0);
	g_main_loop_run(loop);
	return 0;
}

GSQLConnect	* dbclient;
GKeyFile * gkeyfile = NULL;

void myLog(const gchar *log_domain, GLogLevelFlags log_level,
		const gchar *message, gpointer user_data)
{
	gchar * message_hdr;
	const gchar *prg_name = g_get_prgname();

	if (!prg_name)
		prg_name = "process";

	const char * level;
	FILE * logfd;

	if (user_data)
		logfd = user_data;
	else
		logfd = stdout;

	switch (log_level & G_LOG_LEVEL_MASK)
	{
		case G_LOG_LEVEL_ERROR:
			level = _("ERROR");
			break;
		case G_LOG_LEVEL_CRITICAL:
			level = _("CRITICAL");
			break;
		case G_LOG_LEVEL_WARNING:
			level = _("WARNING");
			break;
		case G_LOG_LEVEL_MESSAGE:
			level = _("Message");
			break;
		case G_LOG_LEVEL_INFO:
			level = _("INFO");
			break;
		case G_LOG_LEVEL_DEBUG:
			level = _("DEBUG");
			break;
		default:
			level = "LOG";
			break;
	}

	if(log_level & G_LOG_FLAG_FATAL)
	{
		if (user_data)
			logfd = user_data;
		else
			logfd = stderr;
	}

	struct timeval tv;

	gettimeofday(&tv,0);

	if( tv.tv_usec < start_time.tv_usec)
	{
		tv.tv_usec = 1000000 + tv.tv_usec - start_time.tv_usec;
		tv.tv_sec -= start_time.tv_sec+1;
	}
	else
	{
		tv.tv_sec -= start_time.tv_sec;
		tv.tv_usec -= start_time.tv_usec;
	}

	message_hdr = g_strdup_printf("(%s) [%06li.%04li] **%s** : ",
			prg_name,tv.tv_sec,tv.tv_usec / 100,level);

	NotifyNotification * notify = notify_notification_new(message_hdr,message,NULL,NULL);

	notify_notification_show(notify,0);

	g_object_unref(notify);
	g_free(message_hdr);

	fprintf(logfd,"[%06li.%04li](%s:%lu) **%s** : %s\n",
			tv.tv_sec,tv.tv_usec / 100,
			prg_name,(gulong)getpid(),
			level,message);

	if(user_data)
		fflush(user_data);
}
