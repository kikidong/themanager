

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>
#include <gio/gio.h>

#include <libxml/xmlreader.h>
#include <libxml/xmlsave.h>

#include "database.h"

struct _DataBase{
	gchar * filename;
	xmlDocPtr	xmldoc;
	xmlNodePtr	rootnode;
};


//程序目录，或者家目录看看是否存在,存在并且可写的就返回
gchar *database_checking_existence(gchar * argv_0)
{
	gchar	*  binpath,*dbname;

	binpath = g_path_get_dirname(argv_0);
	g_assert(g_file_test(binpath,G_FILE_TEST_IS_DIR));

	//check 程序目录下是否有数据库,有就是和数据库和程序保存在一个目录
	dbname = g_build_filename(binpath,DB_NAME,NULL);
	g_free(binpath);

	if(g_file_test(dbname,G_FILE_TEST_EXISTS))
	{
		//check 是否可写
		if (g_access(dbname, R_OK | W_OK) == 0)
		{
			// ok , check done, db 就是和程序放在一起的
			return dbname;
		}
	}

	g_free(dbname);

	//检查家目录是否有
	const gchar * home = g_get_home_dir();

	dbname = g_build_filename(home,".themanager",DB_NAME,NULL);

	if(g_file_test(dbname,G_FILE_TEST_EXISTS))
	{
		//check 是否可写
		if (g_access(dbname, R_OK | W_OK) == 0)
		{
			// ok , check done, db 就是放在家目录的
			return dbname;
		}
	}
	g_free(dbname);

	return NULL;
}

//check 程序目录是否可写，否者
//如果不可写，就是保存到家目录
gchar * database_making_new(char * argv_0)
{
	FILE * db;
	gchar	* binpath,*dbname;

	binpath = g_path_get_dirname(argv_0);
	g_assert(g_file_test(binpath,G_FILE_TEST_IS_DIR));

	dbname = g_build_filename(binpath,DB_NAME,NULL);

	g_free(binpath);

	db = fopen(dbname,"wr");

	if(db)
	{
		fclose(db);
		return dbname;
	}

	g_free(dbname);

	const gchar	* home = g_get_home_dir();

	gchar * dbdir = g_build_filename(home,".themanager",NULL);

	if (!g_file_test(dbdir, G_FILE_TEST_IS_DIR | G_FILE_TEST_EXISTS))
	{
		g_mkdir(dbdir,0777);
	}

	g_free(dbdir);

	dbname = g_build_filename(home,".themanager",DB_NAME,NULL);

	db = fopen(dbname,"wr");

	if(db)
	{
		fclose(db);
		return dbname;
	}
}

DataBase * database_open(gchar * filename)
{
	g_assert(g_access(filename,W_OK|R_OK)==0);

	DataBase * db = g_new0(DataBase,1);

	db->filename = g_strdup(filename);

	//数据库其实是 XML 格式文件
	db->xmldoc = xmlReadFile(filename,NULL,0);

	if(!db->xmldoc) //空白数据库
	{
		db->xmldoc = xmlNewDoc("1.0");

		db->rootnode = xmlNewNode(NULL,"database");

		xmlNewProp(db->rootnode, "version", PACKAGE_VERSION);

		xmlDocSetRootElement(db->xmldoc,db->rootnode);
		xmlSaveFormatFileEnc(filename, db->xmldoc, "UTF-8", 1);
		return NULL;
	}
	db->rootnode = xmlDocGetRootElement(db->xmldoc);
	return db;
}

void database_save(DataBase * db)
{
	xmlSaveFormatFileEnc(db->filename, db->xmldoc, "UTF-8", 1);
}

void database_close(DataBase * db)
{
	database_save(db);
	xmlFreeDoc(db->xmldoc);
	xmlCleanupParser();
	g_free(db->filename);
	g_free(db);
}

xmlNodePtr database_getrootnode(DataBase * db)
{
	return db->rootnode;
}

xmlNodePtr database_clearnodes(DataBase * db)
{
	xmlFreeNode(db->rootnode);

	db->rootnode = xmlNewNode(NULL,"database");

	xmlNewProp(db->rootnode, "version", PACKAGE_VERSION);

	xmlDocSetRootElement(db->xmldoc,db->rootnode);

	return db->rootnode;
}
