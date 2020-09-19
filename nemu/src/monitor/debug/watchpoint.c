#include "watchpoint.h"
#include "expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(){
  WP *temp=free_;
  if(free_ == NULL)
  return NULL;
  free_ = free_->next;
  return temp;
}

void insert_wp(WP *wp,char* args){
  bool success = true;
  wp->expr = args;
  wp->NO = head->NO + 1;
  wp->next = head ->next ;
  wp->value = expr(args, &success);
  head = wp;
  printf("watchpoint %d: %s\n",wp->NO,args);
}

void free_wp(WP *wp){
  WP *temp=head;
  while(temp->next != wp)
    temp =temp->next;
  temp->next = wp->next;
  wp->next = free_;
  free_ = wp;
}

bool delete_wp(word_t num){
  WP* temp = head;
  while(temp != NULL){
    if(temp->NO == num){
      free_wp(temp);
      return true;
    }
    temp = temp->next;
  }
  return false;
}