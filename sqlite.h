#ifndef _SQLITE_
#define _SQLITE_

#define PKG_TABLE "packages"
#define DB_PATH "./db.sqlite"
#define COLUMNS "name, version, descr, license, btime, itime"

extern int create_table(char *db_path);
extern int add_pkg_to_db(char *db_path, pkg_t *pkg);
extern int del_pkg_from_db(char *db_path, pkg_t *pkg);
extern pkg_t* get_pkg_from_db(char *db_path, pkg_t *pkg);
extern char* create_query(pkg_t *pkg);
#endif 

