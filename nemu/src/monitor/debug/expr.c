#include <isa.h>
#include <memory/paddr.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

enum {
  TK_NOTYPE = 256, TK_EQ, TK_HEXNUM , TK_DECNUM , TK_NEG , TK_AND , TK_REG, TL_NEG, TK_DEREF,

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
  {"--", TK_NOTYPE},  // minus-minus
  {"0x[0-9,a-f]+", TK_HEXNUM}, // hexnum
  {"[0-9]+", TK_DECNUM},  // decnum
  {"\\$[a-z]{2,3}",TK_REG},  // reg
  {"\\+", '+'},         // plus
  {"\\-", '-'},  // minus
  {"\\*", '*'},  // times
  {"\\/", '/'},  // divide
  {"\\(", '('},  // leftp
  {"\\)", ')'},  // rightp
  {"==", TK_EQ},  // equal 
  {"!=", TK_NEG},  // not equal
  {"&&", TK_AND},  // and

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

static Token tokens[64] __attribute__((used)) = {};
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
            memset(tokens[nr_token].str,'\0',sizeof(tokens[nr_token].str));
            strncpy(tokens[nr_token++].str,substr_start,substr_len);
            break;
          case TK_DECNUM:
            if(substr_len>31)
            {
              printf("The expression is too long");
              return false;
            }
            tokens[nr_token].type = TK_DECNUM;
            memset(tokens[nr_token].str,'\0',sizeof(tokens[nr_token].str));
            strncpy(tokens[nr_token++].str,substr_start,substr_len);
            break;
          case TK_REG:
            tokens[nr_token].type = TK_REG;
            memset(tokens[nr_token].str,'\0',sizeof(tokens[nr_token].str));
            strncpy(tokens[nr_token++].str,substr_start + 1,substr_len - 1);
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
          case TK_AND:
            tokens[nr_token++].type =TK_AND;
            break;
          case TK_EQ:
            tokens[nr_token++].type =TK_EQ;
            break;
          case TK_NEG:
            tokens[nr_token++].type =TK_NEG;
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

typedef struct error{
  bool *legal;
  char type;
} expr_error;

static bool check_parenthese(uint32_t p, uint32_t q, expr_error* error)
{
  if( p == q )
  return false;
  bool flag = true;
  if(tokens[p].type != '(' || tokens[q].type != ')' ){
    flag = false;
    *error->legal = check_parenthese_legal(p , q );
  }
  else
  *error->legal = check_parenthese_legal(p + 1, q - 1);

  return (flag && *error->legal);
}

static uint32_t singletoken_value(Token x, expr_error* error){
  int temp = 0,i;
  switch(x.type){
    case TK_DECNUM : 
      sscanf(x.str,"%d",&temp);
      break;
    case TK_HEXNUM : 
      sscanf(x.str,"%x",&temp);
      break;
    case TK_REG :
    {
      for( i = 0 ; i <= 7 ;i++)
      {
        if(strcmp(regsl[i],x.str)==0)
          return cpu.gpr[i]._32;
        if(strcmp(regsw[i],x.str)==0)
          return (uint32_t)cpu.gpr[i]._16;
        //if(strcmp(regsb[i],x.str)==0)
        //  return ;
      }
      *error->legal = false;
      error->type = 'r';
      break;
    }
    default:
    *error->legal = false;
     error->type = 's';
  }
  return temp;
}

static uint32_t main_operator(uint32_t p, uint32_t q ){
  int cnt = 0,judge = 0,temp[32] = {} ;
  for(int i = p ; i<=q ; i++)
  {
    if(tokens[i].type=='(') judge++ ;
    if(tokens[i].type==')') judge-- ;
    if(tokens[i].type== TK_AND || tokens[i].type == TK_EQ   ){
      if(judge==0)
      return i;
    }
  }
  judge =0;
  for(int i = p ; i<=q ; i++)
  {
    if(tokens[i].type=='(') judge++ ;
    if(tokens[i].type==')') judge-- ;
    if(tokens[i].type== '+' || tokens[i].type== '-' || tokens[i].type=='*' || tokens[i].type=='/' ){
      if(judge==0)
      temp[cnt++]=i;
    }
  }
  cnt--;

  for(int i=cnt ; i>=0; i--)
      if(tokens[temp[i]].type=='+' || tokens[temp[i]].type=='-')
        return temp[i];
 
  for(int i=cnt ; i>=0; i--)
      if(tokens[temp[i]].type=='*' || tokens[temp[i]].type=='/')
        return temp[i];
  
  for(int i=cnt ; i>=0; i--)
      if(tokens[temp[i]].type==TK_DEREF || tokens[temp[i]].type == TK_NEG)
        return temp[i];

  return 0;
}

static uint32_t eval(uint32_t p, uint32_t q,expr_error* error){

  if(p > q){
      *error->legal = false;
      error->type = 's';
      return 0;
  }
  else if( p == q ){
    return singletoken_value(tokens[p],error);
  }
  else if (check_parenthese(p,q,error) == true){
    return eval(p+1,q-1,error);
  }
  else{
    uint32_t op = main_operator(p,q);
    if( tokens[op].type == TK_DEREF || tokens[op].type == TK_NEG)
    {
      u_int32_t temp = eval( op + 1, q , error);
      switch(tokens[op].type){
        case TK_NEG: return -temp; 
        case TK_DEREF: return paddr_read(temp,4); 
        default: 
        *error->legal = false;
        error->type = 's';
      }
    }
    switch(tokens[op].type){
      case '+': return eval( p, op - 1 , error) + eval( op + 1, q , error); 
      case '-': return eval( p, op - 1 , error) - eval( op + 1, q , error); 
      case '*': return eval( p, op - 1 , error) * eval( op + 1, q , error); 
      case '/': {
        uint32_t temp = eval( op + 1, q , error);
        if (temp == 0){
          *error->legal = false;
          error->type = '0';
          temp = 1;
        }
        return eval( p, op - 1 , error) / temp; 
      }
      case TK_EQ:  return eval( p, op - 1 , error) == eval( op + 1, q , error); 
      case TK_NEG: return eval( p, op - 1 , error) != eval( op + 1, q , error); 
      case TK_AND: return eval( p, op - 1 , error) && eval( op + 1, q , error); 
      default: 
      *error->legal = false;
      error->type = 's';
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
  int i;
  expr_error error;
  error.legal = success;
  error.type = '\0';
  nr_token--;
  if ( tokens[0].type == '*' && (tokens[1].type == TK_HEXNUM  || tokens[1].type == TK_DECNUM || tokens[1].type == TK_REG) ) 
    tokens[0].type = TK_DEREF;
  if ( tokens[0].type == '-' && (tokens[1].type == TK_HEXNUM  ||  tokens[1].type == TK_DECNUM || tokens[1].type == TK_REG) ) 
    tokens[0].type = TK_NEG;

  for (i = 1; i < nr_token ;i++) 
  {
    if (tokens[i].type == '*' && (tokens[i+1].type == TK_HEXNUM  ||  tokens[i+1].type == TK_DECNUM || tokens[i+1].type == TK_REG) ) 
      tokens[i].type = TK_DEREF;
    if (tokens[i].type == '-' && (tokens[i+1].type == TK_HEXNUM  ||  tokens[i+1].type == TK_DECNUM || tokens[i+1].type == TK_REG) ) 
      tokens[i].type = TK_NEG;
  }

  word_t temp = eval(0,nr_token,&error);
  switch (error.type)
  {
    case 's':
      *error.legal = false;
      printf("\033[1m\033[41;37m Wrong \033[0m A syntax error in expression! \n");
      break;
    case '0':
      *error.legal = false; 
      printf("\033[1m\033[44;37m Warning \033[0m Division by zero! \n");
      break;
    case 'r':
      *error.legal = false; 
      printf("\033[1m\033[41;37m Wrong \033[0m No such regsiter! \n");
      break;
  }
  return temp;
}
