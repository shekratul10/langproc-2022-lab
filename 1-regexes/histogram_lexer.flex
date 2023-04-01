%option noyywrap

%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %} */

/* Bring in our declarations for token types and
   the yylval variable. */
#include "histogram.hpp"
#include <string.h>
#include <stdlib.h>
#include <string.h> 



// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);

/* End the embedded code section. */
%}


%%

[-]?[0-9]+([.][0-9]+)?  { fprintf(stderr, "Number : %s\n", yytext);
                          /*Decimal and Integer */
                          yylval.numberValue = std::stod(yytext);
                          return Number; }
  

[-]?[0-9]+[\/][0-9]+ { fprintf(stderr, "Number : %s\n", yytext);
                      /*Fraction */
                      std::string s(yytext);
                      int i = s.find_first_of("/");
                      std::string s1 = s.substr(0, i);
                      std::string s2 = s.substr(i+1, s.length()-i);
                      double num = std::stod(s1);
                      double den = std::stod(s2);
                      yylval.numberValue=num/den;
                      return Number;}

[a-zA-Z]+|\[([^\]]*)\]  { fprintf(stderr, "Word : %s\n", yytext);
                        if(yytext[0] == '['){
                           yytext++;
                           yytext[strlen(yytext)-1]='\0';
                           yylval.wordValue = new std::string(yytext);
                        }
                        else{
                           yylval.wordValue = new std::string;
                           *yylval.wordValue = yytext;
                        }
                        return Word; }


[\n]             { fprintf(stderr, "Newline\n"); }

.

[ ]

%%

/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
  exit(1);
}
