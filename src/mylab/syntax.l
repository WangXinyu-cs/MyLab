%option noyywrap nodefault
%option outfile="syntax.lex.c"

%{
#include <cstring>
#include "dstruct.h"
#include "syntax.tab.h"
%}

EXP     ([Ee][-+]?[0-9]+)

%%

"+" |
"-" |
"*" |
"/" |
"\\" |
"=" |
":" |
";" |
"," |
"[" |
"]" |
"<" |
">" |
"(" |
")" |
"&" |
"~"     { return yytext[0]; }

==      { return EQUAL; }

clc   |
who   |
whos  |
clear |
exit    { yylval.strdata = strdup(yytext); return COMMAND; }

zeros |
ones  |
eye   |
rand  |
diag  |
blkdiag   |
cat       |
inv       |
transpose |
det      |
magic    |
max      |
min      |
prod     |
find     |
length   |
size     |
numel    |
reshape  |
linspace |
logspace |
sort     |
sum      |
dot      |
abs  |
fabs |
sqrt |
pow  |
exp  |
ln   |
lg   |
log  |
sin  |
cos  |
tan     { yylval.strdata = strdup(yytext); return FUNCTION; }

[a-zA-Z][a-zA-Z0-9]*    { yylval.strdata = strdup(yytext); return VARNAME; }

[0-9]+  { yylval.strdata = strdup(yytext); return INTEGER; }

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}?        { yylval.strdata = strdup(yytext); return FLOAT; }

'(\\'|[^'])*'   { yylval.strdata = strndup(yytext + 1, yyleng - 2); return STRING; }

[ \t]+  /* ignore whitespace */

\n      { return EOL; }

.       { return yytext[0]; }

<<EOF>> { return YYEOF; }
