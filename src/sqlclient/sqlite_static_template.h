/*
 * ksql_static_template.h
 *
 *      Copyright 2009-2010 薇菜工作室
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

#ifndef SQLITE_STATIC_TEMPLATE_H_
#define SQLITE_STATIC_TEMPLATE_H_


static const char * create_sql[] =
{
//				--
//				-- 表的结构 `student`
//				--
		"CREATE TABLE IF NOT EXISTS `student` ("
		"  `nIndex` INTEGER PRIMARY KEY AUTOINCREMENT," //COMMENT \'key,自动增长\',"
		"	`ID`	TEXT	DEFAULT NULL,"	//学号
		"	`name`	TEXT ,"				//姓名
		"	`mj`		TEXT,"				//专业
		"	`class`	TEXT "				//班级
		") ",

//				--
//				-- 表的结构 `lesson info`
//				--
		"CREATE TABLE IF NOT EXISTS `lesson` ("
		"  `nIndex` INTEGER PRIMARY KEY AUTOINCREMENT,"	//ID
		"	`name`	TEXT ,"						//课程名
		"	`score`	TEXT ,"						//成绩
		"	`stuid`	TEXT	"						//课程所有人的学号 NULL 是课程模板
		") ",
};


#endif /* KSQL_STATIC_TEMPLATE_H_ */
