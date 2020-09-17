#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536];
static int bufptr = 0 ;

uint32_t choose(uint32_t n){
  uint32_t temp= rand();
  temp  = temp % n;
  return temp;
}

static inline void gen_num(){
  buf[bufptr++] = choose(10) + 48;
  return;
}

static inline void gen_rand_op(){
  switch (choose(4))
  {
    case 0 : buf[bufptr++] = '+'; break;
    case 1 : buf[bufptr++] = '-'; break;
    case 2 : buf[bufptr++] = '*'; break;
    case 3 : buf[bufptr++] = '/'; break;
  }
  return;
}

static inline void gen(char x){
  buf[bufptr++] = x;
  return;
}
static inline void gen_rand_expr() {
  //buf[0] = '\0';
  switch (choose(3)) {
    case 0: 
      gen_num(); 
      break;
    case 1: 
      gen('('); gen_rand_expr(); gen(')'); 
      break;
    default: 
      gen_rand_expr(); gen_rand_op(); gen_rand_expr(); 
      break;
  }
}

static char code_buf[65536 + 128]; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

void remove_u(char *p) {
  char *q = p;
  while ((q = strchr(q, 'u')) != NULL) {
    // reuse code_buf
    strcpy(code_buf, q + 1);
    strcpy(q, code_buf);
  }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  bufptr = 0;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
