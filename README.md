# Projet Système & Réseau : Mini-Shell (tsh)

Ce projet est une implémentation d'un mini-shell Unix en C. L'objectif est de comprendre et de manipuler les concepts fondamentaux des systèmes d'exploitation : la création de processus, l'exécution de programmes, la gestion des signaux, et le contrôle des jobs (processus en avant-plan et en arrière-plan).

## Structure du projet

Afin de garder le projet propre et professionnel, les fichiers ont été organisés de la manière suivante :

```
.
├── src/            # Code source (.c)
│   ├── shell.c     # Fichier principal contenant la boucle du shell
│   ├── readcmd.c   # Analyseur (parser) de commandes
│   └── csapp.c     # Fonctions utilitaires (wrappers) type CS:APP
├── include/        # Fichiers d'en-tête (.h)
│   ├── readcmd.h
│   └── csapp.h
├── tests/          # Fichiers de tests fournis (traces test01 à test09)
├── Makefile        # Script de compilation automatisée
├── sdriver.pl      # Script Perl permettant d'évaluer le shell avec les tests
└── README.md       # Ce fichier de documentation
```

## Compilation

Le projet utilise `make` pour automatiser la compilation. Pour compiler le shell, ouvrez un terminal à la racine du projet et exécutez la commande suivante :

```bash
make
```

Cela générera un exécutable nommé `shell` à la racine du projet.

Pour nettoyer les fichiers compilés (les `.o` et l'exécutable) :

```bash
make clean
```

## Exécution

Pour lancer le shell de façon interactive :

```bash
./shell
```
Vous obtiendrez alors le prompt personnalisé et vous pourrez commencer à taper des commandes (ex: `ls -l`, `pwd`, etc.).

Pour quitter le shell, vous pouvez utiliser la commande `quit` (si implémentée) ou le raccourci `Ctrl+D`.

## Tests

Des fichiers de traces et un script de test (`sdriver.pl`) sont fournis pour vérifier le bon fonctionnement de votre shell étape par étape.

### Tests automatiques (Recommandé)
Un script de test automatique a été ajouté pour vous simplifier la vie. Pour lancer l'intégralité des 9 tests d'un coup, il suffit de taper :
```bash
make test
```
Ou bien exécuter manuellement le script : `./run_tests.sh`

### Tests manuels
Pour lancer un test spécifique (par exemple `test01.txt`), utilisez la commande Perl directement :

```bash
perl sdriver.pl -t tests/test01.txt -s ./shell
```

**Options du script de test :**
- `-t <trace>` : Le fichier de test contenant les commandes à exécuter.
- `-s <shell>` : Le chemin vers votre exécutable shell.
- `-h` : Afficher l'aide du script Perl.
- `-v` : Mode verbeux (affiche plus de détails lors de l'exécution).

Exemple pour lancer le test 03 en mode verbeux :
```bash
perl sdriver.pl -t tests/test03.txt -s ./shell -v
```

