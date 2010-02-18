#include <stdlib.h>
#include <archive.h>
#include <unistd.h>
#include <time.h>    

#define _GNU_SOURCE

#include "utils.h"

void* xcalloc(int nmemb, size_t size) {
  void* ptr = calloc(nmemb, size);
  if(ptr == NULL) 
    exit(EALLOC);
  return ptr;
}    

int extract_pkg(char *filename, char *dir) {
  struct archive *a = archive_read_new();
  struct archive_entry *entry;    
  int rc;
  char *pwd = (char*)get_current_dir_name();

  archive_read_support_compression_all(a);
  archive_read_support_format_tar(a);

  rc = archive_read_open_filename(a, filename, 10240);
  if(rc != ARCHIVE_OK)
    return 1;

  if(dir)
    if(chdir(dir)) {
      fprintf(stderr, "Cann't change dir to %s", dir);
      return 1;
    }

  fprintf(stderr, "Extract %s to %s\n", filename, dir ? dir : "./");
  
  while(archive_read_next_header(a, &entry) == ARCHIVE_OK)
    archive_read_extract(a, entry, 0);

  archive_read_finish(a);
  chdir(pwd);
  return 0;
}

/*
 * Function for get the current time
 * and discard \n at the end 
 */
char* get_cur_time(void) {
  const time_t t = time(NULL);
  char *s = ctime(&t);
  char *p = s;
  
  while(*p != '\n')
    p++;
  *p = '\0';     
  return s;           
}

