#ifndef _UTILS_
#define _UTILS_ 

#define EALLOC 1 
extern void* xcalloc(int nmemb, size_t size);
extern int extract_pkg(char *filename, char *dir);
extern char* get_cur_time(void);

#endif

