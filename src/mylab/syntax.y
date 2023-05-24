%parse-param { std::vector<Statement*> *stmt_list }

%{
#include "dstruct.h"
#include "mylab.h"
%}

%union
{
    char    *strdata;
    Express *express;
    std::vector<Express*>   *explist;
    std::vector<std::vector<Express*>*> *exparray;
    Statement   *stmt;
    std::vector<std::string>  *namelist;
}

%token <strdata> INTEGER FLOAT STRING VARNAME FUNCTION COMMAND
%token EQUAL EOL YYEOF

%type <express> express digit
%type <stmt>    stmt
%type <explist> explist paramlist vector digitlist
%type <exparray> mtx_rows matrix
%type <namelist> namelist

%nonassoc EQUAL ':'
%left '+' '-'
%left '*' '/' '\\'
%precedence  LOGINOT
%precedence  UMINUS

%destructor { free($$); } <strdata>
%destructor { delete $$; } <express>
%destructor { delete $$; } <stmt>
%destructor { delete_explist($$); } <explist>
%destructor { delete_mtxrows($$); } <exparray>
%destructor { delete $$; } <namelist>

%start stmtlist

%%

stmtlist
  : stmtprefix EOL  { YYACCEPT; }
  | stmtprefix stmt EOL
    {
        stmt_list->push_back($2);
        YYACCEPT;
    }
  | stmtprefix error EOL
    {
        Statement *st = new Statement();
        st->m_type = Stmt_Type::ST_ERROR;
        st->m_error = yyerror_text;
        stmt_list->push_back(st);
        YYABORT;
    }
  | YYEOF
    {
        Statement *st = new Statement();
        st->m_type = Stmt_Type::ST_EOF;
        stmt_list->push_back(st);
        YYABORT;
    }
  ;

stmtprefix
  : %empty
  | stmtprefix stmt ';'
    {
        stmt_list->push_back($2);
    }
  ;

stmt
  : express
    {
        $$ = new Statement();
        $$->m_type = Stmt_Type::ST_EXPRESS;
        $$->m_express = $1;
    }
  | VARNAME '=' express
    {
        $$ = new Statement();
        $$->m_type = Stmt_Type::ST_ASSIGN;
        $$->m_leftvar = $1;
        $$->m_express = $3;
    }
  | COMMAND namelist
    {
        $$ = new Statement();
        $$->m_type = Stmt_Type::ST_COMMAND;
        $$->m_command = $1;
        $$->m_namelist = $2;
    }
  ;

express
  : express '+' express { $$ = compose_express('+', $1, $3); }
  | express '-' express { $$ = compose_express('-', $1, $3); }
  | express '*' express { $$ = compose_express('*', $1, $3); }
  | express '/' express { $$ = compose_express('/', $1, $3); }
  | express '\\' express { $$ = compose_express('\\', $1, $3); }
  | '(' express ')' { $$ = $2; }
  | VARNAME
    {
        $$ = new Express();
        $$->m_type = ET_VARIABLE;
        $$->m_varname = $1;
    }
  | '-' express %prec UMINUS { $$ = compose_express('U', $2, NULL); }
  | INTEGER
    {
        $$ = new Express();
        $$->set_integer(atol($1));
        free($1);
    }
  | FLOAT
    {
        $$ = new Express();
        $$->set_float(atof($1));
        free($1);
    }
  | STRING
    {
        $$ = new Express();
        $$->set_string($1);
        free($1);
    }
  | FUNCTION '(' paramlist ')'
    {
        $$ = new Express();
        $$->m_type = Express_Type::ET_FUNCTION;
        $$->m_funcname = $1;
        $$->m_explist = $3;
    }
  | vector
    {
      $$ = new Express();
      $$->m_type = Express_Type::ET_VECTOR;
      $$->m_explist = $1;
    }
  | matrix
    {
      $$ = new Express();
      $$->m_type = Express_Type::ET_MATRIX;
      $$->m_exparray = $1;
    }
  | '~' express %prec LOGINOT { $$ = compose_express('~', $2, NULL); }
  | express EQUAL express { $$ = compose_express(EQUAL, $1, $3); }
  | express ':' express ':' express
    {
        $$ = new Express();
        $$->m_type = ET_VECTOR_DIFF;
        $$->m_explist = new std::vector<Express*>();
        $$->m_explist->push_back($1);
        $$->m_explist->push_back($3);
        $$->m_explist->push_back($5);
    }
  ;

namelist
  : %empty { $$ = new std::vector<std::string>(); }
  | VARNAME namelist
    {
      $2->insert($2->begin(), $1);
      free($1);
      $$ = $2;
    }
  ;

vector
  : '<' digitlist '>' { $$ = $2; }
  ;

matrix
  : '[' mtx_rows ']'  { $$ = $2; }
  ;

digitlist
  : %empty  { $$ = new std::vector<Express*>(); }
  | digit digitlist
    {
      $2->insert($2->begin(), $1);
      $$ = $2;
    }
  ;

digit
  : INTEGER
    {
        $$ = new Express();
        $$->set_integer(atol($1));
        free($1);
    }
  | FLOAT
    {
        $$ = new Express();
        $$->set_float(atof($1));
        free($1);
    }
  | '-' INTEGER %prec UMINUS
    {
        $$ = new Express();
        $$->set_integer(-atol($2));
        free($2);
    }
  | '-' FLOAT %prec UMINUS
    {
        $$ = new Express();
        $$->set_float(-atof($2));
        free($2);
    }
  ;

mtx_rows
  : digitlist
    {
        $$ = new std::vector<std::vector<Express*>*>();
        $$->push_back($1);
    }
    | digitlist row_sepa mtx_rows
      {
        $3->insert($3->begin(), $1);
        $$ = $3;
      }
  ;

explist
  : express
    {
        $$ = new std::vector<Express*>();
        $$->push_back($1);
    }
  | explist ',' express
    {
        $1->push_back($3);
        $$ = $1;
    }
  ;

row_sepa
  : ';'
  | ';' EOL
  ;

paramlist
  : %empty  { $$ = new std::vector<Express*>(); }
  | explist
    { $$ = $1; }
  ;
