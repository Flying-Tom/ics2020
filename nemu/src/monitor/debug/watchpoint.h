#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char* expr;
  uint32_t value;
  /* TODO: Add more members if necessary */

} WP;

#endif
