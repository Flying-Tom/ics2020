#include <isa.h>
#include <memory/paddr.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

extern const char *regsl[];
extern const char *regsw[];
extern const char *regsb[];

enum
{
    TK_NOTYPE = 256,
    TK_EQ,
    TK_HEXNUM,
    TK_DECNUM,
    TK_NEG,
    TK_AND,
    TK_REG,
    TK_NEQ,
    TK_DEREF,

    /* TODO: Add more token types */

};

static struct rule
{
    char *regex;
    int token_type;
} rules[] = {

    /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

    {" +", TK_NOTYPE},           // spaces
    {"0x[0-9,a-f]+", TK_HEXNUM}, // hexnum
    {"[0-9]+", TK_DECNUM},       // decnum
    {"\\$[a-z]{2,3}", TK_REG},   // reg
    {"\\+", '+'},                // plus
    {"\\-", '-'},                // minus
    {"\\*", '*'},                // times
    {"\\/", '/'},                // divide
    {"\\(", '('},                // leftp
    {"\\)", ')'},                // rightp
    {"==", TK_EQ},               // equal
    {"!=", TK_NEQ},              // not equal
    {"&&", TK_AND},              // and

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
    int i;
    char error_msg[128];
    int ret;

    for (i = 0; i < NR_REGEX; i++)
    {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if (ret != 0)
        {
            regerror(ret, &re[i], error_msg, 128);
            panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
        }
    }
}

typedef struct token
{
    int type, priority;
    char str[32];
} Token;

static Token tokens[128] __attribute__((used)) = {};
static int nr_token __attribute__((used)) = 0;
static int priority_used[16] = {};

static bool make_token(char *e)
{
    int position = 0;
    int i;
    regmatch_t pmatch;
    nr_token = 0;

    while (e[position] != '\0')
    {
        /* Try all rules one by one. */
        for (i = 0; i < NR_REGEX; i++)
        {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
            {
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;

                Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
                    i, rules[i].regex, position, substr_len, substr_len, substr_start);

                position += substr_len;

                /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

                switch (rules[i].token_type)
                {
                case TK_NOTYPE:
                    break;
                case TK_HEXNUM:
                    if (substr_len > 31)
                    {
                        printf("The expression is too long");
                        return false;
                    }
                    tokens[nr_token].type = TK_HEXNUM;
                    tokens[nr_token].priority = 0;
                    memset(tokens[nr_token].str, '\0', sizeof(tokens[nr_token].str));
                    strncpy(tokens[nr_token++].str, substr_start, substr_len);
                    break;
                case TK_DECNUM:
                    if (substr_len > 31)
                    {
                        printf("The expression is too long");
                        return false;
                    }
                    tokens[nr_token].type = TK_DECNUM;
                    tokens[nr_token].priority = 0;
                    memset(tokens[nr_token].str, '\0', sizeof(tokens[nr_token].str));
                    strncpy(tokens[nr_token++].str, substr_start, substr_len);
                    break;
                case TK_REG:
                    tokens[nr_token].type = TK_REG;
                    tokens[nr_token].priority = 0;
                    memset(tokens[nr_token].str, '\0', sizeof(tokens[nr_token].str));
                    strncpy(tokens[nr_token++].str, substr_start + 1, substr_len - 1);
                    break;
                case '+':
                    tokens[nr_token].type = '+';
                    tokens[nr_token++].priority = 4;
                    break;
                case '-':
                    tokens[nr_token].type = '-';
                    tokens[nr_token++].priority = 4;
                    break;
                case '*':
                    tokens[nr_token].type = '*';
                    tokens[nr_token++].priority = 3;
                    break;
                case '/':
                    tokens[nr_token].type = '/';
                    tokens[nr_token++].priority = 3;
                    break;
                case '(':
                    tokens[nr_token].priority = 1;
                    tokens[nr_token++].type = '(';
                    break;
                case ')':
                    tokens[nr_token].priority = 1;
                    tokens[nr_token++].type = ')';
                    break;
                case TK_AND:
                    tokens[nr_token].type = TK_AND;
                    tokens[nr_token++].priority = 11;
                    break;
                case TK_EQ:
                    tokens[nr_token].type = TK_EQ;
                    tokens[nr_token++].priority = 7;
                    break;
                case TK_NEG:
                    tokens[nr_token].type = TK_NEG;
                    tokens[nr_token++].priority = 7;
                    break;
                default:
                    printf("There exists an undefined expression. Please check it. Failed! \n");
                }

                break;
            }
        }

        if (i == NR_REGEX)
        {
            printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
            return false;
        }
    }

    return true;
}

static bool check_parenthese_legal(uint32_t p, uint32_t q)
{
    int cnt = 0;
    if (p == q)
        return true;
    for (int i = p; i <= q; i++)
    {
        if (cnt < 0)
            return false;
        cnt += (tokens[i].type == '(');
        cnt -= (tokens[i].type == ')');
    }
    if (cnt == 0)
        return true;
    else
        return false;
}

static bool check_parenthese(uint32_t p, uint32_t q, char *error)
{
    if (p == q)
        return false;
    bool flag = true, errortemp = true;
    if (tokens[p].type != '(' || tokens[q].type != ')')
    {
        flag = false;
        errortemp = check_parenthese_legal(p, q);
    }
    else
        errortemp = check_parenthese_legal(p + 1, q - 1);
    return (flag && errortemp);
}

static uint32_t singletoken_value(Token *x, char *error)
{
    int temp = 0;
    switch (x->type)
    {
    case TK_DECNUM:
        sscanf(x->str, "%d", &temp);
        break;
    case TK_HEXNUM:
        sscanf(x->str, "%x", &temp);
        break;
    case TK_REG:
    {
        bool success;
        temp = isa_reg_str2val(x->str, &success);
        if (success == false)
            *error = 'r';
        break;
    }
    default:
        *error = 's';
    }
    return temp;
}

static uint32_t main_operator(uint32_t p, uint32_t q)
{
    int judge = 0, location = -1;
    for (int j = 12; j >= 2; j--)
    {
        judge = 0;
        for (int i = p; i <= q; i++)
        {
            judge += (tokens[i].type == '(');
            judge -= (tokens[i].type == ')');
            if (tokens[i].priority == j)
            {
                if (judge == 0)
                {
                    if (j == 2)
                        return i;
                    else
                        location = i;
                }
            }
        }
        if (location != -1)
            return location;
    }

    return 0;
}

static word_t eval(uint32_t p, uint32_t q, char *error)
{

    if (p > q)
    {
        *error = 's';
        return 0;
    }
    else if (p == q)
        return singletoken_value(&tokens[p], error);
    else if (check_parenthese(p, q, error) == true)
        return eval(p + 1, q - 1, error);
    else
    {
        word_t op = main_operator(p, q);
        if (tokens[op].type == TK_DEREF || tokens[op].type == TK_NEG)
        {
            int temp = eval(op + 1, q, error);
            switch (tokens[op].type)
            {
            case TK_NEG:
                return -temp;
            case TK_DEREF:
                return paddr_read(temp, 4);
            default:
                *error = 's';
            }
        }
        switch (tokens[op].type)
        {
        case '+':
            return eval(p, op - 1, error) + eval(op + 1, q, error);
        case '-':
            return eval(p, op - 1, error) - eval(op + 1, q, error);
        case '*':
            return eval(p, op - 1, error) * eval(op + 1, q, error);
        case '/':
        {
            word_t temp = eval(op + 1, q, error);
            if (temp == 0)
            {
                *error = '0';
                return eval(p, op - 1, error);
            }
            return eval(p, op - 1, error) / temp;
        }
        case TK_EQ:
            return eval(p, op - 1, error) == eval(op + 1, q, error);
        case TK_NEQ:
            return eval(p, op - 1, error) != eval(op + 1, q, error);
        case TK_AND:
            return eval(p, op - 1, error) && eval(op + 1, q, error);
        default:
            *error = 's';
        }
    }
    return 0;
}

word_t expr(char *e, bool *success)
{
    if (!make_token(e))
    {
        *success = false;
        return 0;
    }
    /* TODO: Insert codes to evaluate the expression. */
    int i, temp = 0;
    char error = '1';
    nr_token--;
    while (tokens[temp].type == '*')
    {
        tokens[temp].type = TK_DEREF;
        tokens[temp++].priority = 2;
    }
    temp = 0;
    while (tokens[temp].type == '-')
    {
        tokens[temp].type = TK_NEG;
        tokens[temp++].priority = 2;
    }

    for (i = 1; i < nr_token; i++)
    {
        if (tokens[i].type == '*' && tokens[i - 1].priority != 0 && tokens[i - 1].type != ')')
        {
            temp = i;
            while (tokens[temp].type == '*')
            {
                tokens[temp].type = TK_DEREF;
                tokens[temp++].priority = 2;
            }
        }

        if (tokens[i].type == '-' && tokens[i - 1].priority != 0 && tokens[i - 1].type != ')')
        {
            temp = i;
            while (tokens[temp].type == '-')
            {
                tokens[temp].type = TK_NEG;
                tokens[temp++].priority = 2;
            }
        }
    }
    for (i = 0; i <= nr_token; i++)
        priority_used[tokens[i].priority] = (tokens[i].priority >= 2);
    word_t value_temp = eval(0, nr_token, &error);
    if (error != '1')
        *success = false;
    switch (error)
    {
    case 's':
        printf("\033[1m\033[41;37m Wrong \033[0m A syntax error in expression! \n");
        break;
    case '(':
        printf("\033[1m\033[41;37m Wrong \033[0m A syntax error about paretheses in expression ! \n");
        break;
    case '0':
        printf("\033[1m\033[44;37m Warning \033[0m Division by zero! \n");
        break;
    case 'r':
        printf("\033[1m\033[41;37m Wrong \033[0m No such regsiter! \n");
        break;
    }
    return value_temp;
}
