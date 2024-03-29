#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char expr[128];
  uint32_t prev_value,value;
  /* TODO: Add more members if necessary */

} WP;

WP* new_wp();
void display_wp();
void insert_wp(WP* ,char* );
bool delete_wp(word_t );
bool check_wp();
#endif
