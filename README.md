# Projet programmation impérative 3
## OUVERTURE BINAIRE DE LA BASE

parler des fonctions appelées à comp et décomp du multifichiers

parler du séparateur et de comment on le lit et tout (on le rajoute si test mauvais)

expliquer comment marche la v5 (recréation d'arborescence, etc...)

ne compile pas les dossiers vides (à cacher?)

TESTER SI DECOMPRESSION DANS UN DOSSIER MARCHE BIEN A CHAQUE FOIS

- j'ai fait deux fonctions pour compiler dans un fichier et pour reconstruire l'arborescence
  - bugs au niveau de getline? noms de fichier bizarre (`'t2'$'\n'` au lieu de `t2`)
  - s'arrête quand on ne peut plus rien lire ("erreur lors de la lecture du fichier")
- j'ai testé de compresser de l'unicode (VOIR la section **DEUX TRUCS SPECIAUX**)

à chaque ouverture, check si fichier existe (fonction (déléguer à tom))
chaque version s'utilise avec chaque version (compressé de v1 peut pas être décomp avec v2)



1. un seul fichier texte

2. plusieurs fichiers (en liste) - ou un seul fichier

3. fichiers sont dans un dossier (check si il n'y a pas de sous dossiers?)
	-> décompression: dossier doit être recréé et fichiers décompressés dedans
	
4. dossier qui doit contenir fichiers texte et dossiers ayant eux-même des fichiers texte

5. liste de dossiers et fichiers






A FAIRE
- code source + makefile
- notice README (comment créer exécutable, principal, exécutable de génération de fichiers de test et comment générer les fichiers de test)
- présentation LaTeX (sources et pdf)
- versions.txt (description)

PRESENTATION
- table de correspondance pour noms de dossiers (quand profondeur grande)
# TESTS A FAIRE (x,y)
-> mettre la variance à chaque fois dans le CSV
- TAILLE (caractères entièrement aléatoires)
  - courbe 1 (non compressé): (x) taille du fichier, (y) taille du fichier
  - courbe 2 (compressé): (x) taille originale, (y) taille compressée
- FREQUENCE CARACTERES (taille donnée)
- NOMBRE DE CARACTERES DIFFERENTS (taille et fréquence donnée)
- DOSSIERS: nb_fichiers, sous-dossiers/profondeur dossiers
1-100000 (logarithmique)
10 ou 20 tirages pour une taille
