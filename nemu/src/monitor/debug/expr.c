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
            //tokens[nr_token].str[substr_len] = '\0';
            break;
          case TK_DECNUM:
            if(substr_len>31)
            {
              printf("The expression is too long");
              return false;
            }
            tokens[nr_token].type = TK_DECNUM;
            strncpy(tokens[nr_token++].str,substr_start,substr_len);
            //tokens[nr_token].str[substr_len] = '\0';
            break;
          case '+':
            tokens[nr_token++].type ='+';
            break;
          case '-':
            tokens[nr_token++].type ='-';
            break;
          case '*':
            tokens[nr_token++].type ='*';
            break;
          case '/':
            tokens[nr_token++].type ='/';
            break;
          case '(':
            tokens[nr_token++].type ='(';
            break;
          case ')':
            tokens[nr_token++].type =')';
            break;
          case TK_EQ:
            tokens[nr_token++].type =TK_EQ;
            break;
          default:
            printf("There exists an undefined expression. Please check it. Failed! \n");
            //printf("%d",rules[i].token_type);
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


static bool check_parenthese_legal(uint32_t p, uint32_t q)
{
  int cnt=0;
  if(p==q)
  return true;
  for(int i = p ; i<= q ; i++)
  {
    if(cnt < 0)
        return false;

    if(tokens[i].type=='(')
        cnt++;

    if(tokens[i].type==')')
        cnt--;
  }
    if(cnt == 0)
      return true;
    else
      return false;
}

static bool check_parenthese(uint32_t p, uint32_t q, bool* legal, int* error_info)
{
  if( p == q )
  return false;
  bool flag = true;
  if(tokens[p].type != '(' || tokens[q].type != ')' ){
    flag = false;
    *legal = check_parenthese_legal(p , q );
  }
  else
  *legal = check_parenthese_legal(p + 1, q - 1);
  if(!(*legal))
  *error_info = 1;
  return (flag && *legal);
}

static uint32_t singletoken_value(Token x){
  int temp;
  switch(x.type){
    case TK_DECNUM : 
      sscanf(x.str,"%d",&temp);
      break;
    case TK_HEXNUM : 
      sscanf(x.str,"%x",&temp);
      break;
  }
  return temp;
}

static uint32_t main_operator(uint32_t p, uint32_t q ){
  int cnt = 0,judge = 0,temp[32] = {},flag=0 ;
  for(int i = p ; i<=q ; i++)
  {
    if(tokens[i].type=='(')
      judge++ ;
    if(tokens[i].type==')')
      judge-- ;
    if(tokens[i].type== '+' || tokens[i].type== '-' || tokens[i].type=='*' || tokens[i].type=='/' ){
      if(judge==0)
      temp[cnt++]=i;
    }
  }
  cnt--;
  for(int i=0  ; i<=cnt ; i++)
  {
    if(tokens[temp[i]].type=='+' || tokens[temp[i]].type=='-'){
      flag=1;
      break;
    }
  }
  
  if(flag==1){
    for(int i=cnt ; i>=0; i--)
      if(tokens[temp[i]].type=='+' || tokens[temp[i]].type=='-')
        return temp[i];
  }
  else{
    for(int i=cnt ; i>=0; i--)
      if(tokens[temp[i]].type=='*' || tokens[temp[i]].type=='/')
        return temp[i];
  }

  return 0;
}

static uint32_t eval(uint32_t p, uint32_t q,bool* legal,int* error_info){

  if(p > q){
      *legal = false;
      *error_info = 2;
      return 0;
  }
  else if( p == q ){
    return singletoken_value(tokens[p]);
  }
  else if (check_parenthese(p,q,legal,error_info) == true){
    return eval(p+1,q-1,legal, error_info);
  }
  else{
    uint32_t op = main_operator(p,q);
    switch(tokens[op].type){
      case '+': return eval( p, op - 1 , legal, error_info) + eval( op + 1, q , legal, error_info); break;
      case '-': return eval( p, op - 1 , legal, error_info) - eval( op + 1, q , legal, error_info); break;
      case '*': return eval( p, op - 1 , legal, error_info) * eval( op + 1, q , legal, error_info); break;
      case '/': return eval( p, op - 1 , legal, error_info) / eval( op + 1, q , legal, error_info); break;
      default: 
      *legal = false;
      *error_info = 3;
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
    int* error_info=0;
  nr_token--;
  uint32_t expr_value = eval(0,nr_token,success,error_info);
  if(*success == false){}
  {
    switch (*error_info)
    {
      case 1: 
        printf("Synax error! There may be something wrong with the ().\n");
        break;
      case 2: 
        printf("Synax error! There may be something wrong with the ().\n");
        break;
      case 3: 
        printf("Synax error! There exist undefined identifier.\n");
        break;
      default:
        printf("Synax error! \n");
    }
  }
  return expr_value;
}
