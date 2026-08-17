%{
#include "scanner.hh"
#include <cstdlib>
#define YY_NO_UNISTD_H
using token = yy::Parser::token;
#undef  YY_DECL
#define YY_DECL int Scanner::yylex( yy::Parser::semantic_type * const lval, yy::Parser::location_type *loc )
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}

%option c++
%option yyclass="Scanner"
%option noyywrap

%%

%{
    yylval = lval;
%}

"fin"                { return token::END; }
"+"                  { return '+'; }
"-"                  { return '-'; }
"*"                  { return '*'; }
"/"                  { return '/'; }
"("                  { return '('; }
")"                  { return ')'; }
":"                  { return ':'; }
"@"                  { return '@'; }

<<EOF>>              { return token::END_OF_FILE; }

"direction"          { return token::DIRECTION; }

"jardin"             { return token::JARDIN; }

"avance"             { return token::AVANCE; }
"recule"             { return token::RECULE; }
"saute"              { return token::SAUTE; }
"tourne"             { return token::TOURNE; }

"fois"               { return token::FOIS; }

"mur"                { return token::MUR; }
"vide"               { return token::VIDE; }

"devant"             { return token::DEVANT; }
"derriere"           { return token::DERRIERE; }
"à gauche"           { return token::GAUCHE; }
"à droite"           { return token::DROITE; }

"pas de"             { return token::PASDE; }

"si"                 { return token::SI; }
"sinon"              { return token::SINON; }
"fin si"             { return token::FINSI; }

"tant que"           { return token::TANTQUE; }
"fin tant que"       { return token::FINTANTQUE; }

"repete"             { return token::REPETE; }
"fin repete"         { return token::FINREPETE; }

"fonction"           { return token::FONCTION; }
"fin fonction"       { return token::FINFONCTION; }

"tortues"            { return token::TORTUES; }

"couleur"            { return token::COULEUR; }

"carapace"           {
    yylval->build<std::string>(YYText());
    return token::IDENTIFIER;
}

"motif"              {
    yylval->build<std::string>(YYText());
    return token::IDENTIFIER;
}

"$"[0-9]+            {
    yylval->build<int>(std::atoi(YYText() + 1));
    return token::PARAMETRE;
}

"#"[0-9a-fA-F]{6}    {
    yylval->build<std::string>(YYText());
    return token::CHAINE;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval->build<std::string>(YYText());
    return token::IDENTIFIER;
}

\'([^\'\\]|\\.)*\'   {
    std::string s(YYText());
    s = s.substr(1, s.size() - 2);
    yylval->build<std::string>(s);
    return token::CHAINE;
}

[0-9]+               {
    yylval->build<int>(std::atoi(YYText()));
    return token::NUMBER;
}

[ \t]+               { /* Ignorer espaces et tabulations */ }

"--".*               { /* Ignorer commentaires */ }

"\n"                 {
    loc->lines();
    return token::NL;
}

.                    {
    std::cerr << "Caractère non reconnu : " << YYText() << std::endl;
}

%%