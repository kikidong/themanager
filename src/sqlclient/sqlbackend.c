/*
 * sqlbackend.c
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


#include <glib/gi18n.h>
#include "gsqlconnect.h"
#ifdef WITH_MYSQL
#include "gsqlconnect_mysql.h"
#endif
#include "gsqlconnect_sqlite3.h"
#include "global.h"

GSQLConnect *  sqlconnect_new()
{
	return g_object_new(G_TYPE_SQL_CONNNECT_SQLITE,0);
}
