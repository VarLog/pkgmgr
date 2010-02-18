#ifndef _MAIN_
#define _MAIN_ 

typedef struct pkg {
  char *name;
  char *version;
  char *descr;
  char *license;
  char *btime;    // build time
  char *itime;    // install time
} pkg_t;     

extern pkg_t* get_pkgs(pkg_t *pkg);
extern int show_pkg(pkg_t *pkgs);
extern int install_pkg(pkg_t *pkg, char *filename);
extern int del_pkg(pkg_t *pkg); 

#endif

