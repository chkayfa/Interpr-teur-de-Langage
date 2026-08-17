#ifndef DRIVER_H
#define DRIVER_H

#include <string>

#include "contexte.hh"

class Driver
{
private:
    Contexte variables;

public:
    Driver();
    ~Driver();

    const Contexte &getContexte() const;
    double getVariable(const std::string &name) const;
    void setVariable(const std::string &name, double value);
    int nombreTortues() const; // Retourne le nombre de tortues actives
    void setNombreTortues(int n);
    void avancer(int id, int pas);
    void reculer(int id, int pas);
    void sauter(int id, int pas);
    void tourner(int id, Dir direction, int fois);
    void changerCouleur(int id, std::string cible, std::string hexa);

    // Analyse du terrain
    bool testerMur(int id, Dir direction);
    bool testerVide(int id, Dir direction);

    // Gestion des fonctions (le stockage du bloc d'instructions)
    void enregistrerFonction(std::string nom, InstructionPtr corps);
    InstructionPtr recupererFonction(std::string nom);
};

#endif
