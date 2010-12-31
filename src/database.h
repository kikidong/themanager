
#ifndef DATABASE_H_
#define DATABASE_H_

#include <libxml/tree.h>

#define DB_NAME	"database.xml"

typedef struct _DataBase DataBase;

G_GNUC_INTERNAL gchar *database_checking_existence(gchar * argv_0);
G_GNUC_INTERNAL gchar * database_making_new(char * argv_0);

G_GNUC_INTERNAL DataBase * database_open(gchar * filename);
G_GNUC_INTERNAL void database_save(DataBase * db);
G_GNUC_INTERNAL void database_close(DataBase * db);

G_GNUC_INTERNAL xmlNodePtr database_getrootnode(DataBase * db);
G_GNUC_INTERNAL xmlNodePtr database_clearnodes(DataBase * db);


#endif /* DATABASE_H_ */
