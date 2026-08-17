#include "instructions.hh"
#include "../parser/driver.hh"
#include <cmath>

void Bloc::executer(Driver &driver)
{
  for (auto &inst : liste)
    inst->executer(driver);
}

void Avance::executer(Driver &driver)
{
  int d = distance ? distance->calculer(driver.getContexte()) : 1;
  if (tortueId == -1)
  {
    for (int i = 0; i < driver.nombreTortues(); ++i)
      driver.avancer(i, d);
  }
  else
  {
    driver.avancer(tortueId, d);
  }
}

void Recule::executer(Driver &driver)
{
  int d = distance ? distance->calculer(driver.getContexte()) : 1;
  if (tortueId == -1)
  {
    for (int i = 0; i < driver.nombreTortues(); ++i)
      driver.reculer(i, d);
  }
  else
  {
    driver.reculer(tortueId, d);
  }
}

void Saute::executer(Driver &driver)
{
  int d = distance ? distance->calculer(driver.getContexte()) : 1;
  if (tortueId == -1)
  {
    for (int i = 0; i < driver.nombreTortues(); ++i)
      driver.sauter(i, d);
  }
  else
  {
    driver.sauter(tortueId, d);
  }
}

void Tourne::executer(Driver &driver)
{
  // Par défaut 90 degrés (un quart de cercle)
  int degres = angle ? angle->calculer(driver.getContexte()) : 90;
  
  // Direction: GAUCHE = sens antihoraire (positif), DROITE = sens horaire (négatif)
  if (direction == Dir::DROITE)
    degres = -degres;
  
  if (tortueId == -1)
  {
    for (int i = 0; i < driver.nombreTortues(); ++i)
      driver.tourner(i, degres);
  }
  else
  {
    driver.tourner(tortueId, degres);
  }
}

void Tortues::executer(Driver &driver)
{
  int n = nombre->calculer(driver.getContexte());
  driver.definirNombreTortues(n);
}

void Couleur::executer(Driver &driver)
{
  // Extraire RGB du format #rrggbb
  if (couleurHex.length() != 7 || couleurHex[0] != '#')
    return; // Format invalide
  
  int r = std::stoi(couleurHex.substr(1, 2), nullptr, 16);
  int g = std::stoi(couleurHex.substr(3, 2), nullptr, 16);
  int b = std::stoi(couleurHex.substr(5, 2), nullptr, 16);
  
  if (tortueId == -1)
  {
    // Appliquer à toutes les tortues
    for (int i = 0; i < driver.nombreTortues(); ++i)
    {
      if (zone == "motif")
        driver.changerCouleurMotif(i, r, g, b);
      else // "carapace" par défaut
        driver.changerCouleurCarapace(i, r, g, b);
    }
  }
  else
  {
    if (zone == "motif")
      driver.changerCouleurMotif(tortueId, r, g, b);
    else // "carapace" par défaut
      driver.changerCouleurCarapace(tortueId, r, g, b);
  }
}

void AppelFonction::executer(Driver &driver)
{
  driver.appelFonction(nomFonction, arguments);
}

bool Condition::verifier(Driver &driver)
{
  bool resultat = false;
  if (tortueId == -1)
  {
    // "si mur devant" sans @ veut dire "si mur devant pour TOUTES les tortues" (ET logique)
    resultat = true;
    for (int i = 0; i < driver.nombreTortues(); ++i)
    {
      bool test = (type == CondType::MUR) ? driver.testerMur(i, direction) : driver.testerVide(i, direction);
      resultat = resultat && test;
    }
  }
  else
  {
    resultat = (type == CondType::MUR) ? driver.testerMur(tortueId, direction) : driver.testerVide(tortueId, direction);
  }
  return negation ? !resultat : resultat;
}

void Si::executer(Driver &driver)
{
  if (cond && cond->verifier(driver))
  {
    if (blocAlors)
      blocAlors->executer(driver);
  }
  else
  {
    if (blocSinon)
      blocSinon->executer(driver);
  }
}

void TantQue::executer(Driver &driver)
{
  while (cond && cond->verifier(driver))
  {
    if (bloc)
      bloc->executer(driver);
  }
}

void Repete::executer(Driver &driver)
{
  int n = nombreFois->calculer(driver.getContexte());
  // Arrondir à l'entier le plus proche si nécessaire
  n = std::round(n);
  
  for (int i = 0; i < n; ++i)
  {
    if (bloc)
      bloc->executer(driver);
  }
}