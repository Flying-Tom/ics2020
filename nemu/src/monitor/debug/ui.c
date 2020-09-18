#include <isa.h>
#include "expr.h"
#include "watchpoint.h"
#include <memory/paddr.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
int is_batch_mode();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_si(char *args ) {
  char *arg = strtok(NULL," ");
  int num = 1;
  if( arg != NULL)
    sscanf(arg,"%d",&num);
  cpu_exec(num);
  return 0;
}

static int cmd_info(char *args ) {
  char *arg = strtok(NULL," ");
  if(*arg=='r')
    isa_reg_display();
  return 0;
}

static int cmd_x(char *args ) {
  unsigned int num=0,address_start=0;
  char *arg = strtok(NULL," ");
  sscanf(arg,"%d",&num);
  arg = strtok(NULL," ");
  sscanf(arg,"0x%x",&address_start);
  for(int i=0 ; i < num ; i++)
  printf("0x%x : 0x%x\n",address_start + 4 * i ,paddr_read(address_start + 4 * i,4));
  return 0;
}

static int cmd_p(char *args ) {
  bool success = true;
  word_t expr_value = expr(args,&success);
  if(success)
    printf("%u\n", expr_value);
  return 0;
}

static int cmd_de(char *args ) {
  char de_buf[1024]="";
  char* de_buf_p;
  word_t temp,my_ans,len;
  FILE *fp = fopen(args,"r");
  while ((fgets(de_buf,1024,fp)) != NULL)
  {
    len = strlen(de_buf);
    de_buf[ len - 1 ] = '\0';
    de_buf_p = strtok(de_buf," ");
    sscanf(de_buf_p,"%u",&temp);
    de_buf_p = strtok(NULL," ");
    bool success = true;
    my_ans =  expr(de_buf_p,&success);
    if( temp == my_ans )
      printf("Yes, you are right!\n");
    else
      printf("Error! %s == %u != %u \n",de_buf_p,temp,my_ans);
  }
  return 0;
}


static int cmd_help(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "push the debug program one step more", cmd_si },
  { "info", "print the debug information", cmd_info },
  { "x", "scanf the ram", cmd_x },
  { "p", "print the value of an expression",cmd_p},
  { "de", "debug the function which needs polishing",cmd_de}
  //{ "de", "debug a new function",cmd_de}

  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop() {
  if (is_batch_mode()) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
