/*
 * html_paths.h -- 定义各个路径处理函数
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

#ifndef HTML_PATHS_H_
#define HTML_PATHS_H_

#include <glib.h>
#include <libsoup/soup.h>

G_BEGIN_DECLS

void soup_message_body_appender(const gchar * txt, SoupMessageBody * body) G_GNUC_INTERNAL;

void SoupServer_path_root_icon(SoupServer *server, SoupMessage *msg,
		const char *path, GHashTable *query, SoupClientContext *client,
		gpointer user_data) G_GNUC_INTERNAL;

void SoupServer_path_404(SoupServer *server, SoupMessage *msg,const char *path,
		GHashTable *query, SoupClientContext *client,gpointer user_data)G_GNUC_INTERNAL;

void SoupServer_path_static_file(SoupServer *server, SoupMessage *msg,
		const char *path, GHashTable *query, SoupClientContext *client,
		gpointer user_data) G_GNUC_INTERNAL;

void SoupServer_path_stulist(SoupServer *server, SoupMessage *msg,
		const char *path, GHashTable *query, SoupClientContext *client,
		gpointer user_data) G_GNUC_INTERNAL;

void SoupServer_path_input_student_info(SoupServer *server, SoupMessage *msg,
		const char *path, GHashTable *query, SoupClientContext *client,
		gpointer user_data) G_GNUC_INTERNAL;

void SoupServer_path_input_class_info(SoupServer *server, SoupMessage *msg,
		const char *path, GHashTable *query, SoupClientContext *client,
		gpointer user_data) G_GNUC_INTERNAL;

void SoupServer_path_dformjs(SoupServer *server, SoupMessage *msg,
		const char *path, GHashTable *query, SoupClientContext *client,
		gpointer user_data) G_GNUC_INTERNAL;


G_END_DECLS

#endif /* HTML_PATHS_H_ */
