#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_HEXNUM , TK_DECNUM ,

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal 
  {"0x[0-9,a-f]+", TK_HEXNUM}, // hexnum
  {"[0-9]+", TK_DECNUM},  // decnum
  {"\\-", '-'},  // minus
  {"\\*", '*'},  // times
  {"\\/", '/'},  // divide
  {"\\(", '('},  // leftp
  {"\\)", ')'},  // rightp

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;
  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        
        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          case TK_HEXNUM:
            if(substr_len>31)
            {
              printf("The expression is too long");
              return false;
            }
            tokens[nr_token].type = TK_HEXNUM;
            strncpy(tokens[nr_token++].str,substr_start,substr_len);
            break;
          case TK_DECNUM:
            if(substr_len>31)
            {
              printf("The expression is too long");
              return false;
            }
            tokens[nr_token].type = TK_DECNUM;
            strncpy(tokens[nr_token++].str,substr_start,substr_len);
            break;
          case '+':
            tokens[nr_token++].type ='+';
            break;
          case '-':
            tokens[nr_token++].type ='-';
            break;
          case TK_EQ:
            tokens[nr_token++].type =TK_EQ;
            break;
          case '(':
            tokens[nr_token++].type ='(';
            break;
          case ')':
            tokens[nr_token++].type =')';
            break;
          default:
            printf("There exists an undefined expression. Please check it. Failed! \n"); 
            return false;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}
static uint32_t check_parenthese(uint32_t p, uint32_t q){
  
  int cnt=0;
  for(int i = p ; i<= q ; i++)
  {
    if(tokens[i].type=='(')
      cnt++;
    if(tokens[i].type==')')
    {
      if(cnt==0)
        return false;
      else
        cnt--;
    }
  }
  return cnt==0;
}

static uint32_t singletoken_value(Token x){
  int temp;
  switch(x.type){
    case TK_DECNUM : sscanf(x.str,"%d",&temp);
    case TK_HEXNUM : sscanf(x.str,"%x",&temp);
  }
  return temp;
}

static uint32_t main_operator(uint32_t p, uint32_t q){
  int cnt = 0,judge = 0,temp[32] = {},flag=0;
  for(int i = p ; i<=q ; i++)
  {
    if(tokens[i].type=='(')
      judge++ ;
    if(tokens[i].type==')')
      judge++ ;
    if(tokens[i].type== '+' || tokens[i].type== '-' || tokens[i].type=='*' || tokens[i].type=='/' ){
      if(judge==0)
      temp[cnt++]=i;
    }
  }
  cnt--;
  for(int i=0  ; i<=cnt ; i++)
  {
    if(tokens[temp[i]].type=='*' || tokens[temp[i]].type=='/'){
      flag=1;
      break;
    }
  }
  
  if(flag==1){
    for(int i=cnt ; i>=0; i--)
      if(tokens[temp[i]].type=='*' || tokens[temp[i]].type=='/')
        return i;
  }
  else{
    for(int i=cnt ; i>=0; i--)
      if(tokens[temp[i]].type=='+' || tokens[temp[i]].type=='-')
        return i;
  }

  return 0;
}

static uint32_t eval(uint32_t p, uint32_t q){
  if(p > q){
      printf("illegal");//need to modify
      return 0;
  }
  else if( p == q){
    return singletoken_value(tokens[p]);
  }
  else if (check_parenthese(p,q) == true){
    return eval(p+1,q-1);
  }
  else{
    int op = main_operator(p,q);
    switch(tokens[op].type){
      case '+': return eval( p, op - 1) + eval( op + 1, q);
      case '-': return eval( p, op - 1) - eval( op + 1, q);
      case '*': return eval( p, op - 1) * eval( op + 1, q);
      case '/': return eval( p, op - 1) / eval( op + 1, q);
      default: assert(0);
    }
  }
  return 0;
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  
  /* TODO: Insert codes to evaluate the expression. */
  nr_token--;
  return eval(0,nr_token);
}
