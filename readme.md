# Projet Turtl - Par Nakhli Mohamed Amine and Fatima-Ezzahra Dhahbi

## Séparation de travail:

Mohamed Amine: Analyseur Syntaxique + Structure de Données Intermédiaire
Fatima Zahhra Analyseur Lexical

## Structure du Projet

### Analyseur Lexical (`scanner.ll`)
- Définit les tokens du langage (mots-clés, opérateurs, identifiants, nombres)
- Gère les commentaires (`--`) et les chaînes de caractères
- Reconnaît les couleurs au format hexadécimal (`#rrggbb`)
- Traite les paramètres de fonction (`$1`, `$2`, etc.)

### Analyseur Syntaxique (`parser.yy`)
- Définit la grammaire du langage
- Construit l'arbre syntaxique abstrait (AST)
- Gère les priorités des opérateurs arithmétiques
- Implémente les structures de contrôle (si/sinon, tant que, repete)

### Structure de Données Intermédiaire (`instructions.hh` / `instructions.cc`)
Classes principales implémentées :
- **Bloc** : Conteneur d'instructions
- **Avance/Recule/Saute** : Déplacements de base
- **Tourne** : Rotation (gauche/droite)
- **Si/TantQue/Repete** : Structures de contrôle
- **Condition** : Évaluation de conditions (mur/vide)
- **Tortues** : Définition du nombre de tortues
- **Couleur** : Modification de l'apparence
- **AppelFonction** : Appel de fonctions définies

## Fonctionnalités Implémentées

### Instructions de Base
- `avance [n]` : Avancer de n cases
- `recule [n]` : Reculer de n cases
- `saute [n]` : Sauter par-dessus un obstacle
- `tourne <à gauche|à droite> [n]` : Tourner de n×90°

### Structures de Contrôle
- **Conditions** : `si <condition>: ... fin si` avec support de `sinon`
- **Boucles** : 
  - `tant que <condition>: ... fin tant que`
  - `repete n fois: ... fin repete`

### Fonctionnalités Avancées
- **Fonctions** : Définition et appel avec paramètres (`$1`, `$2`, etc.)
- **Multi-tortues** : Support de plusieurs tortues avec l'opérateur `@n`
- **Couleurs** : `couleur <carapace|motif> #rrggbb [@n]`
- **Expressions arithmétiques** : Support de `+`, `-`, `*`, `/`, `()`
- **Commentaires** : Lignes commençant par `--`

## Compilation et Exécution

### Prérequis
- flex
- libfl-dev
- bison
- qt6-qmake
- qtbase6-dev

### Compilation
```bash
mkdir build
cd build
qmake ../projet.pro
make
```

### Exécution
```bash
./tortue
```

## Workflow de Développement

1. **Phase 1 : Analyse Lexicale**
   - Création du fichier `scanner.ll`
   - Définition des expressions régulières pour tous les tokens
   - Tests avec des exemples simples

2. **Phase 2 : Analyse Syntaxique**
   - Création du fichier `parser.yy`
   - Définition de la grammaire complète
   - Gestion des priorités et de l'associativité des opérateurs

3. **Phase 3 : Structure de Données**
   - Conception de l'AST (classes d'instructions)
   - Implémentation des méthodes `executer()`
   - Connexion avec l'interface graphique via le Driver

4. **Phase 4 : Tests et Débogage**
   - Tests avec les exemples fournis
   - Validation des résultats visuels
   - Corrections des bugs

## Utilisation de l'IA

L'assistance de l'IA a été utilisée dans les domaines suivants :

- **Détection de typos** : Identification et correction d'erreurs de frappe dans le code (ex: `toekn::COULEUR` → `token::COULEUR`)
- **Correction des expressions régulières** : Ajustement des regex dans `scanner.ll` pour reconnaître correctement les tokens (notamment pour les couleurs hexadécimales et les chaînes de caractères)
- **Complétion de fonctions** : Aide à l'implémentation de certaines méthodes dans les fichiers de variables et d'instructions (ex: implémentation complète des classes `Recule`, `Saute`, `Tourne`, `TantQue`, `Repete`)
- **Débogage** : Identification d'erreurs d'index dans les règles du parser (ex: correction de l'index `$6` → `$5` dans la règle REPETE)

