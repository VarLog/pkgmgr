/* Work with sqlite db */

#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h> 

#include <errno.h>

#include "main.h"
#include "sqlite.h"
#include "utils.h"

#define PKG_TABLE "packages"
#define DB_PATH "./db.sqlite"
#define COLUMNS "name, version, descr, license, btime, itime"  // Not used  

/* 
 * Function to create a sql table 
 */
int create_table(char *db_path) {
  /* Checking argument*/
  if(!db_path)
    return 1;
  
  sqlite3 *db;
  char *error_msg = 0, *query;
  int rc;
  
  /* Create query */
  query = "CREATE TABLE " PKG_TABLE " ("COLUMNS")";
  /* Opening database */
  rc = sqlite3_open(db_path, &db);
  if(rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }  

  /* Execution query */
  rc = sqlite3_exec(db, query, NULL, NULL, &error_msg);
  if(rc) {
    fprintf(stderr, "SQL error: %s\n", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(db);
    return 1;
  }  
  return 0;
}

/*
 * Function to insert an item in the table
 */
int add_pkg_to_db(char *db_path, pkg_t *pkg) {
  /* Checking argument*/
  if(!db_path)
    return 1; 
  
  sqlite3 *db;
  char *error_msg = 0; 
  //char *query = (char*)xcalloc(500, sizeof(char));
  char query[500];
  int rc;
  
  /* Create query */
  sprintf(query, "INSERT INTO " PKG_TABLE " ("COLUMNS") VALUES "
          "('%s', '%s', '%s', '%s', '%s', '%s')", pkg->name, pkg->version, 
          pkg->descr, pkg->license, pkg->btime, pkg->itime);

  /* Opening database */ 
  rc = sqlite3_open(db_path, &db);
  if(rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  } 
  
  /* Execution query */
  rc = sqlite3_exec(db, query, NULL, NULL, &error_msg);
  if(rc) {
    fprintf(stderr, "SQL error: %s\n", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(db);
    return 1;
  }  
  return 0;   
}

/*
 * Function to delete the item from the table
 */
int del_pkg_from_db(char *db_path, pkg_t *pkg) {
  /* Checking argument*/
  if(!db_path)
    return 1;

  sqlite3 *db;
  char *error_msg = 0; 
  char query[500];
  int rc;

  /* Opening database */ 
  rc = sqlite3_open(db_path, &db); 
  if(rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  /* Create query */
  strcpy(query, "DELETE FROM " PKG_TABLE " ");
  strcat(query, create_query(pkg));

  /* Execution query */
  rc = sqlite3_exec(db, query, NULL, NULL, &error_msg);
  if(rc) {
    fprintf(stderr, "SQL error: %s\n", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(db);
    return 1;
  }          
  return 0;
}

/*
 * Function to get an item from the table
 */
pkg_t* get_pkg_from_db(char *db_path, pkg_t *pkg) {
  /* Checking argument*/
  if(!db_path)
    return NULL;
  
  sqlite3 *db;
  char *error_msg = 0;
  char query[500];
  int rc;
  
  /* Opening database */ 
  rc = sqlite3_open(db_path, &db); 
  if(rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return NULL;
  }  
  
  /* Create query */
  strcpy(query, "SELECT * FROM " PKG_TABLE " ");
  strcat(query, create_query(pkg));

  /* Getting data from the table 
   * and record them in pkgs[] */
  char **s;
  int nrow, ncolumn;
  int i, t;
  pkg_t* pkgs = NULL;

  /* SQLite returns the data found on the criteria in s[] */
  rc = sqlite3_get_table(db, query, &s, &nrow, &ncolumn, &error_msg);
  if(rc) {
    fprintf(stderr, "SQL error: %s\n", error_msg);
    sqlite3_free(error_msg);
    sqlite3_close(db);
    return NULL;
  } 
  
  /* Defining array */
  pkgs = (pkg_t*)xcalloc(nrow + 1, sizeof(pkg_t));
  for(t=0, i=ncolumn; t<nrow; t++) {
    pkgs[t].name = s[i++];
    pkgs[t].version = s[i++];
    pkgs[t].descr = s[i++];
    pkgs[t].license = s[i++];
    pkgs[t].btime = s[i++];
    pkgs[t].itime = s[i++];
  }
  
  /* Add NULL to the end of the array */
  pkgs[t].name = NULL;

  sqlite3_free_table(s);
  return pkgs;
}

/* 
 * Function to create a parameter query
 */
char* create_query(pkg_t *pkg) {
  char *query = (char*)xcalloc(500, sizeof(char)); // Тут утечка
  int is_and = 0;

  if(pkg->name || pkg->version || pkg->descr || pkg->license || pkg->btime || pkg->itime) {
    strcat(query, "WHERE ");

    if(pkg->name) {
      strcat(query, "name='");
      strcat(query, pkg->name);
      strcat(query, "' ");
      is_and = 1;
    }
    if(pkg->version) {
      if(is_and) 
        strcat(query, "AND ");
      strcat(query, "version='");
      strcat(query, pkg->version);
      strcat(query, "' ");
      is_and = 1;
    }
    if(pkg->descr) {
      if(is_and) 
        strcat(query, "AND ");
      strcat(query, "descr='");
      strcat(query, pkg->descr);
      strcat(query, "' ");
      is_and = 1;
    }
    if(pkg->license) {
      if(is_and) 
        strcat(query, "AND ");
      strcat(query, "license='");
      strcat(query, pkg->license);
      strcat(query, "' ");
      is_and = 1;
    }  
    if(pkg->btime) {
      if(is_and) 
        strcat(query, "AND ");
      strcat(query, "btime='");
      strcat(query, pkg->btime);
      strcat(query, "' ");
      is_and = 1;
    }  
    if(pkg->itime) {
      if(is_and) 
        strcat(query, "AND ");
      strcat(query, "itime='");
      strcat(query, pkg->itime);
      strcat(query, "' ");
      is_and = 1;
    }
  }
  //printf("query = %s\n", query); //debug  
  return query;
}

