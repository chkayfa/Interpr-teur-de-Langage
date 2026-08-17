#include "driver.hh"
#include "../GUI/jardinHandler.hh"
#include "instructions.hh"
#include <iostream>

#include <map>
#include <string>
#include <QPoint>
#include <QColor>
#include <QSize>
#include "contexte.hh"

class JardinHandler;
class JardinRendering;

Driver::Driver(JardinHandler *J) : monJardin(J)
{
    // Par défaut, il y a au moins une tortue
}

Driver::~Driver()
{
    // Le destructeur gère la libération si nécessaire
}

// --- Gestion du Contexte (Variables) ---

const Contexte &Driver::getContexte() const
{
    return variables;
}

double Driver::getVariable(const std::string &name) const
{
    return variables.get(name);
}

void Driver::setVariable(const std::string &name, double value)
{
    variables[name] = value;
}

// --- Gestion des Tortues (Moteurs Graphiques) ---

int Driver::nombreTortues() const
{
    // On demande au moteur de rendu combien de tortues existent
    return monJardin->getJardin()->nombreTortues();
}

void Driver::setNombreTortues(int n)
{
    // Méthode pour initialiser X tortues dans le jardin
    monJardin->getJardin()->setNombreTortues(n);
}

void Driver::avancer(int id, int pas)
{
    // Le moteur de rendu utilise souvent 'step' pour avancer
    monJardin->getJardin()->step(id, pas);
}

void Driver::reculer(int id, int pas)
{
    monJardin->getJardin()->step(id, -pas);
}

void Driver::sauter(int id, int pas)
{
    // 'jump' déplace la tortue sans dessiner de trace
    monJardin->getJardin()->jump(id, pas);
}

void Driver::tourner(int id, Dir direction, int fois)
{
    // On calcule l'angle : 90 degrés * nombre de fois
    // GAUCHE est souvent négatif, DROITE positif (selon l'API du prof)
    float angle = 90.0f * fois;
    if (direction == Dir::GAUCHE)
        angle = -angle;
    monJardin->getJardin()->rotate(id, angle);
}

// --- Analyse du Terrain (Capteurs) ---

bool Driver::testerMur(int id, Dir direction)
{
    // On utilise les méthodes de détection du JardinRendering
    // direction est converti en entier (0:devant, 1:derriere, etc.)
    return monJardin->getJardin()->isWall(id, static_cast<int>(direction));
}

bool Driver::testerVide(int id, Dir direction)
{
    return monJardin->getJardin()->isEmpty(id, static_cast<int>(direction));
}

// --- Gestion des Fonctions (AST) ---

void Driver::enregistrerFonction(std::string nom, InstructionPtr corps)
{
    fonctions[nom] = corps;
}

InstructionPtr Driver::recupererFonction(std::string nom)
{
    if (fonctions.find(nom) != fonctions.end())
    {
        return fonctions[nom];
    }
    std::cerr << "Erreur : Fonction '" << nom << "' non définie." << std::endl;
    return nullptr;
}

void Driver::dessiner()
{
    monJardin->dessiner();
}

void Driver::nouveauJardin(std::string nomJardin)
{
    monJardin->getJardin()->construction(nomJardin);
}