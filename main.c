// Main //
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 

#include <errno.h>

#include <time.h>

#include "main.h"
#include "sqlite.h"
#include "utils.h"
#include "options.h"

/*
 * Function to check database
 */
static int check_db(char *db_path) {
  struct stat buf; 
  if(stat(db_path, &buf) == -1)
    if(errno == ENOENT)
      return 1;
  return 0;
}

pkg_t* get_pkgs(pkg_t *pkg) {
  pkg_t* pkgs = get_pkg_from_db(DB_PATH, pkg);
  if(!pkgs)
    return NULL;
  return pkgs;
}

int show_pkg(pkg_t *pkg) {
  if(!pkg)
    return -1;
  
  printf("\tpkg->name = %s\n", pkg->name);
  printf("\tpkg->version = %s\n", pkg->version);
  printf("\tpkg->descr = %s\n", pkg->descr);
  printf("\tpkg->license = %s\n", pkg->license);
  printf("\tpkg->btime = %s\n", pkg->btime);
  printf("\tpkg->itime = %s\n", pkg->itime);
  printf("\n");
  return 0;
}

int install_pkg(pkg_t *pkg) {
  /* Get the current time 
   * and discard \n at the end */
  const time_t t = time(NULL);
  char *s = ctime(&t);
  char *p = s;
  
  while(*p != '\n')
    p++;
  *p = '\0';     
  pkg->itime = s;

  if(!pkg->name || !pkg->version) {
    fprintf(stderr, "No name and version of package!\n");
    return 1;
  }
  add_pkg_to_db(DB_PATH, pkg);
  return 0;
}

int del_pkg(pkg_t *pkg) {
  if(!pkg->name || !pkg->version) {
    fprintf(stderr, "No name and version of package!\n");
    return 1;
  }
  del_pkg_from_db(DB_PATH, pkg);
  return 0;
}     

int main(int argc, char **argv) {
  if(check_db(DB_PATH))
    if(create_table(DB_PATH))
      exit(1);
  
  parse_opts(argc, argv);

  return 0;
}



