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
  WP *cur = head;
  uint32_t cnt = 0;
  if(head == NULL)
    head = wp;
  else
  {
    while (cur->next != NULL)
    {  
      cnt = cur->NO;
      cur = cur->next;
    }
    cur->next = wp;
    cnt = cur->NO;
  }
  
  wp->NO = cnt + 1;
  wp->expr = args;
  wp->value=expr(args, &success);
  wp->next = NULL;
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

void display_wp(){
  WP* cur = head;
  while(cur != NULL){
    printf("%d : %s == %d\n",cur->NO,cur->expr,cur->value );
    cur = cur->next;
  }
}