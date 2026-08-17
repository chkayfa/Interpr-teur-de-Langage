%require "3.0"
%defines
%define api.parser.class { Parser }
%define api.value.type variant
%define parse.assert
%locations

%code requires {
    #include "contexte.hh"
    #include "expression.hh"
    #include "expressionBinaire.hh"
    #include "expressionUnaire.hh"
    #include "expressionTernaire.hh"
    #include "constante.hh"
    #include "variable.hh"
    #include "instructions.hh"
    #include <memory>
    #include <vector>
    
    class Scanner;
    class Driver;
}

%parse-param { Scanner &scanner }
%parse-param { Driver &driver }

%code {
    #include <iostream>
    #include <string>
    
    #include "scanner.hh"
    #include "driver.hh"
    
    #undef  yylex
    #define yylex scanner.yylex
}


%token                  NL
%token                  END
%token                  END_OF_FILE
%token                  DIRECTION
%token                  JARDIN
%token <std::string>    CHAINE
%token <int>            NUMBER
%token <int>            PARAMETRE

%token <std::string>    IDENTIFIER 
%token AVANCE RECULE SAUTE TOURNE
%token SI SINON FINSI TANTQUE FINTANTQUE REPETE FINREPETE FONCTION FINFONCTION
%token FOIS GAUCHE DROITE DEVANT DERRIERE VIDE MUR PASDE TORTUES COULEUR
%token ':' '@' '+' '-' '*' '/' '(' ')'


%left '+' '-'
%left '*' '/'
%precedence NEG


/* instructionPtr pour les actions avancer, tourner ... */

%type <ExpressionPtr> expression expression_simple expression_fois
%type <InstructionPtr> instruction bloc_instructions commande structure_controle
%type <std::shared_ptr<Condition>> condition
%type <std::vector<ExpressionPtr>> liste_arguments
%type <int> tortue_opt
%type <int> direction

%%

programme:
    liste_globale END_OF_FILE { 
        driver.dessiner(); 
        YYACCEPT; 
    }

liste_globale:
      %empty
    | liste_globale element_global

element_global:
      instruction { if ($1) $1->executer(driver); }
    | definition_fonction
    | NL

/* Bloc d'instructions pour SI, TANT QUE, REPETE, FONCTION */
bloc_instructions:
      %empty { $$ = std::make_shared<Bloc>(); }
    | bloc_instructions instruction {
        auto bloc = std::static_pointer_cast<Bloc>($1);
        if ($2) bloc->ajouter($2);
        $$ = bloc;
    }

instruction:
      commande NL { $$ = $1; }
    | structure_controle { $$ = $1; }
    | NL { $$ = nullptr; }

commande:
      AVANCE expression tortue_opt { $$ = std::make_shared<Avance>($2, $3); }
    | AVANCE tortue_opt            { $$ = std::make_shared<Avance>(std::make_shared<Constante>(1), $2); }
    | RECULE expression tortue_opt { $$ = std::make_shared<Recule>($2, $3); }
    | RECULE tortue_opt            { $$ = std::make_shared<Recule>(std::make_shared<Constante>(1), $2); }
    | SAUTE expression tortue_opt  { $$ = std::make_shared<Saute>($2, $3); }
    | SAUTE tortue_opt             { $$ = std::make_shared<Saute>(std::make_shared<Constante>(1), $2); }
    | TOURNE GAUCHE expression tortue_opt  { $$ = std::make_shared<Tourne>(Dir::GAUCHE, $3, $4); }
    | TOURNE GAUCHE tortue_opt             { $$ = std::make_shared<Tourne>(Dir::GAUCHE, nullptr, $3); }
    | TOURNE DROITE expression tortue_opt  { $$ = std::make_shared<Tourne>(Dir::DROITE, $3, $4); }
    | TOURNE DROITE tortue_opt             { $$ = std::make_shared<Tourne>(Dir::DROITE, nullptr, $3); }
    | JARDIN CHAINE                { driver.nouveauJardin($2); $$ = nullptr; }
    | TORTUES expression          { $$ = std::make_shared<Tortues>($2); }
    | IDENTIFIER liste_arguments   { $$ = std::make_shared<AppelFonction>($1, $2); }
    | COULEUR IDENTIFIER CHAINE tortue_opt { 
        /* IDENTIFIER = carapace ou motif, CHAINE = couleur hexa */
        $$ = std::make_shared<Couleur>($2, $3, $4); 
    }

direction:
      DEVANT { $$ = 0; } 
    | DERRIERE { $$ = 1; } 
    | GAUCHE { $$ = 2; } 
    | DROITE { $$ = 3; }

tortue_opt:
      %empty { $$ = -1; }
    | '@' NUMBER { $$ = $2; }

/* Gestion de l'expression avec "fois" optionnel pour REPETE */
expression_fois:
      expression { $$ = $1; }
    | expression FOIS { $$ = $1; }

structure_controle:
      SI condition ':' NL bloc_instructions FINSI NL {
          $$ = std::make_shared<Si>($2, $5, nullptr);
      }
    | SI condition ':' NL bloc_instructions SINON ':' NL bloc_instructions FINSI NL {
          $$ = std::make_shared<Si>($2, $5, $9);
      }
    | TANTQUE condition ':' NL bloc_instructions FINTANTQUE NL {
          $$ = std::make_shared<TantQue>($2, $5);
      }
    | REPETE expression_fois ':' NL bloc_instructions FINREPETE NL {
          $$ = std::make_shared<Repete>($2, $5);
      }

condition:
      MUR direction tortue_opt      { $$ = std::make_shared<Condition>(CondType::MUR, $2, false, $3); }
    | VIDE direction tortue_opt     { $$ = std::make_shared<Condition>(CondType::VIDE, $2, false, $3); }
    | PASDE MUR direction tortue_opt { $$ = std::make_shared<Condition>(CondType::MUR, $3, true, $4); }
    | PASDE VIDE direction tortue_opt { $$ = std::make_shared<Condition>(CondType::VIDE, $3, true, $4); }

definition_fonction:
    FONCTION IDENTIFIER ':' NL bloc_instructions FINFONCTION NL {
        driver.enregistrerFonction($2, $5);
    }

liste_arguments:
      %empty { $$ = std::vector<ExpressionPtr>(); }
    | liste_arguments expression_simple { 
        $1.push_back($2); 
        $$ = $1; 
    }

/* Expression simple (sans opérateurs binaires, pour les arguments) */
expression_simple:
      NUMBER {
        $$ = std::make_shared<Constante>($1);
      }
    | PARAMETRE {
        $$ = std::make_shared<Parametre>($1);
      }
    | IDENTIFIER {
        $$ = std::make_shared<Variable>($1);
      }
    | '-' expression_simple %prec NEG {
        $$ = std::make_shared<ExpressionUnaire>($2, OperateurUnaire::neg);
      }
    | '(' expression ')' {
        $$ = $2;
      }

/* Expression complète (avec opérateurs binaires) */
expression:
      expression_simple {
        $$ = $1;
      }
    | expression '+' expression {
        $$ = std::make_shared<ExpressionBinaire>($1, $3, OperateurBinaire::plus);
      }
    | expression '-' expression {
        $$ = std::make_shared<ExpressionBinaire>($1, $3, OperateurBinaire::moins);
      }
    | expression '*' expression {
        $$ = std::make_shared<ExpressionBinaire>($1, $3, OperateurBinaire::multiplie);
      }
    | expression '/' expression {
        $$ = std::make_shared<ExpressionBinaire>($1, $3, OperateurBinaire::divise);
      }

%%

void yy::Parser::error(const location_type &l, const std::string &err_msg) {
    std::cerr << "Erreur : " << l << ", " << err_msg << std::endl;
}