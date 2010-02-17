#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "main.h"
#include "utils.h"
#include "options.h"

typedef void (*getopts_t)(int, char**);

static void del_options(int, char**);
static void install_options(int, char**);
static void search_options(int, char**);
static void usage(int, char**);

struct cmd {
  char* cmd;
  getopts_t parse;
  char* descr;
};

static struct cmd cmds[] = {
  { "delete",  del_options, "delete package" },
  { "install", install_options, "install package" },
  { "help",    usage, "print this text and exit" },
  { "search",  search_options, "search package" },
  { "usage",   usage, "print this text and exit" },
  { NULL,  NULL, NULL }
};                                                   

static void del_options(int argc, char** argv) {
  int opt;
  char *opt_str = "v:n:";
  pkg_t* pkg = (pkg_t*)xcalloc(1, sizeof(pkg_t));
  pkg_t* gpkg;  // Pointer for get_pkgs()

  struct option long_opts[] = {
      { "name", required_argument, NULL, 'n' },
      { "version", required_argument, NULL, 'v' },
      { NULL, no_argument, NULL, 0 }, };
  int option_index = 0;

  while((opt = getopt_long(argc, argv, opt_str, long_opts, &option_index)) != -1)
    switch(opt) {
      case 'n':
        pkg->name = optarg;
        break;
      case 'v':
        pkg->version = optarg;
        break;
    }     
  
  gpkg = get_pkgs(pkg);
  if(gpkg->name) {
    printf("Deleting package:..\n"); 
    show_pkg(gpkg);
    if(!del_pkg(pkg))
      printf("Done!\n");
    else
      printf("Delete Function returned an error!!\n");
  }
  else
    printf("ABORT! This package is NOT installed.\n");
  return;
}

static void install_options(int argc, char** argv) {
  int opt;
  char *opt_str = "v:n:d:l:";
  pkg_t* pkg = (pkg_t*)xcalloc(1, sizeof(pkg_t)); 
  pkg_t* gpkg;  // Pointer for get_pkgs()

  struct option long_opts[] = {
      { "name", required_argument, NULL, 'n' },
      { "version", required_argument, NULL, 'v' },
      { "descr", required_argument, NULL, 'd' },
      { "license", required_argument, NULL, 'l' },
      { NULL, no_argument, NULL, 0 }, };
  int option_index = 0;
  
  while((opt = getopt_long(argc, argv, opt_str, long_opts, &option_index)) != -1)
    switch(opt) {
      case 'n':
        pkg->name = optarg;
        break;
      case 'v':
        pkg->version = optarg;
        break;
      case 'd':
        pkg->descr = optarg;
        break;  
      case 'l':
        pkg->license = optarg;
        break;  
    }     

  gpkg = get_pkgs(pkg);   
  if(!gpkg->name) {
    printf("Installation package:..\n"); 
    show_pkg(pkg);
    if(!install_pkg(pkg))
      printf("Done!\n");
    else
      printf("Delete Function returned an error!!\n");  
  }
  else {
    printf("ABORT! This package is ALREADY installed:\n");
    show_pkg(pkg);
  }
}

static void search_options(int argc, char** argv) {
  int opt; 
  int i = 0;
  char *opt_str = "v:n:d:l:";
  pkg_t* pkg = (pkg_t*)xcalloc(1, sizeof(pkg_t));
  pkg_t* gpkg;  // Pointer for get_pkgs()

  struct option long_opts[] = {
      { "name", required_argument, NULL, 'n' },
      { "version", required_argument, NULL, 'v' },
      { "descr", required_argument, NULL, 'd' },
      { "license", required_argument, NULL, 'l' },
      { NULL, no_argument, NULL, 0 }, };
  int option_index = 0;

  while((opt = getopt_long(argc, argv, opt_str, long_opts, &option_index)) != -1)
    switch(opt) {
      case 'n':
        pkg->name = optarg;
        break;
      case 'v':
        pkg->version = optarg;
        break;
      case 'd':
        pkg->descr = optarg;
        break;  
      case 'l':
        pkg->license = optarg;
        break;
    }     
  
  gpkg = get_pkgs(pkg);
  while(gpkg[i].name != NULL) {
    printf("Package #%d\n", i + 1);
    show_pkg(&gpkg[i]);
    i++;
  }  
  
  if(!i)
    printf("No matches found\n");
}

static void usage(int argc, char** argv) {
  struct cmd *p = cmds;

  puts("Package manager - version 0.0.0-dev");
  while(p->cmd != NULL) {
    printf("  %-7s - %s\n", p->cmd, p->descr);
    p++;
  }
}

void parse_opts(int argc, char **argv) {
  getopts_t parse = NULL;

  if(argc < 2)
    parse = usage;
  else {
    int i;
    for(i = 0; cmds[i].cmd; i++)
      if(!strcmp(cmds[i].cmd, argv[1])) {
        parse = cmds[i].parse;
        break;
      }
  }
  
  if(!parse)
    parse = usage;
  
  parse(argc, argv);
}



