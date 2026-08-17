#ifndef INSTRUCTION_HH
#define INSTRUCTION_HH

#include <memory>
#include <vector>
#include <string>
#include "expression.hh"

class Driver;

using ExpressionPtr = std::shared_ptr<Expression>;
class Instruction;
using InstructionPtr = std::shared_ptr<Instruction>;

enum class Dir
{
  DEVANT = 0,
  DERRIERE = 1,
  GAUCHE = 2,
  DROITE = 3
};

enum class CondType
{
  MUR,
  VIDE
};

// Classe de base
class Instruction
{
public:
  virtual ~Instruction() = default;
  virtual void executer(Driver &driver) = 0;
};

// Conteneur de plusieurs instructions (le corps d'un SI ou d'une BOUCLE)
class Bloc : public Instruction
{
  std::vector<InstructionPtr> liste;

public:
  void ajouter(InstructionPtr inst)
  {
    if (inst)
      liste.push_back(inst);
  }
  void executer(Driver &driver) override;
};

// Instruction AVANCE
class Avance : public Instruction
{
  ExpressionPtr distance;
  int tortueId; // -1 pour toutes, sinon 0, 1, 2...
public:
  Avance(ExpressionPtr d, int id) : distance(d), tortueId(id) {}
  void executer(Driver &driver) override;
};

// Instruction RECULE
class Recule : public Instruction
{
  ExpressionPtr distance;
  int tortueId;
public:
  Recule(ExpressionPtr d, int id) : distance(d), tortueId(id) {}
  void executer(Driver &driver) override;
};

// Instruction SAUTE
class Saute : public Instruction
{
  ExpressionPtr distance;
  int tortueId;
public:
  Saute(ExpressionPtr d, int id) : distance(d), tortueId(id) {}
  void executer(Driver &driver) override;
};

// Instruction TOURNE
class Tourne : public Instruction
{
  Dir direction;
  ExpressionPtr angle; // peut être nullptr (90 degrés par défaut)
  int tortueId;
public:
  Tourne(Dir d, ExpressionPtr a, int id) : direction(d), angle(a), tortueId(id) {}
  void executer(Driver &driver) override;
};

// Instruction TORTUES (définir le nombre de tortues)
class Tortues : public Instruction
{
  ExpressionPtr nombre;
public:
  Tortues(ExpressionPtr n) : nombre(n) {}
  void executer(Driver &driver) override;
};

// Instruction COULEUR
class Couleur : public Instruction
{
  std::string zone;      // "carapace" ou "motif"
  std::string couleurHex; // format "#rrggbb"
  int tortueId;
public:
  Couleur(const std::string &z, const std::string &c, int id) 
    : zone(z), couleurHex(c), tortueId(id) {}
  void executer(Driver &driver) override;
};

// Appel de fonction
class AppelFonction : public Instruction
{
  std::string nomFonction;
  std::vector<ExpressionPtr> arguments;
public:
  AppelFonction(const std::string &nom, const std::vector<ExpressionPtr> &args)
    : nomFonction(nom), arguments(args) {}
  void executer(Driver &driver) override;
};

// La classe Condition pour les SI et TANT QUE
class Condition
{
  CondType type;
  Dir direction;
  bool negation;
  int tortueId;

public:
  Condition(CondType t, int d, bool n, int id)
      : type(t), direction(static_cast<Dir>(d)), negation(n), tortueId(id) {}
  bool verifier(Driver &driver);
};

// Le nœud SI
class Si : public Instruction
{
  std::shared_ptr<Condition> cond;
  InstructionPtr blocAlors;
  InstructionPtr blocSinon;

public:
  Si(std::shared_ptr<Condition> c, InstructionPtr a, InstructionPtr s)
      : cond(c), blocAlors(a), blocSinon(s) {}
  void executer(Driver &driver) override;
};

// Le nœud TANT QUE
class TantQue : public Instruction
{
  std::shared_ptr<Condition> cond;
  InstructionPtr bloc;

public:
  TantQue(std::shared_ptr<Condition> c, InstructionPtr b)
      : cond(c), bloc(b) {}
  void executer(Driver &driver) override;
};

// Le nœud REPETE
class Repete : public Instruction
{
  ExpressionPtr nombreFois;
  InstructionPtr bloc;

public:
  Repete(ExpressionPtr n, InstructionPtr b)
      : nombreFois(n), bloc(b) {}
  void executer(Driver &driver) override;
};

#endif