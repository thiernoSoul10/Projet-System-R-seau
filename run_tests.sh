#!/bin/bash

# Configuration
SHELL_EXEC="./shell"
TESTS_DIR="tests"
DRIVER="sdriver.pl"

# Vérifier que l'exécutable shell existe
if [ ! -f "$SHELL_EXEC" ]; then
    echo "[Erreur] L'exécutable '$SHELL_EXEC' est introuvable."
    echo "Veuillez compiler le projet avec 'make' avant de lancer les tests."
    exit 1
fi

# Vérifier que le script de test existe
if [ ! -f "$DRIVER" ]; then
    echo "[Erreur] Le script test '$DRIVER' est introuvable."
    exit 1
fi

echo "========================================"
echo "Lancement des tests automatiques"
echo "========================================"

# Trouver tous les fichiers testxx.txt, les trier et les exécuter
for test_file in $(ls $TESTS_DIR/test*.txt | sort); do
    test_name=$(basename "$test_file")
    
    echo ""
    echo ">> Exécution de $test_name :"
    echo "----------------------------------------"
    
    # Exécution du test via le script Perl
    perl "$DRIVER" -t "$test_file" -s "$SHELL_EXEC"
    
    echo "----------------------------------------"
done

echo ""
echo "[Succès] Tous les tests ont été exécutés."
echo "========================================"
