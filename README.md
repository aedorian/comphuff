# Projet programmation impérative 3
## OUVERTURE BINAIRE DE LA BASE

rapport: dire qu'on a pensé à 3 bits pour nb_bits

---

attention: maintenant on encode nb_char - 1 (et on récupère nb_char le vrai)

---

# A FAIRE (UPDATE)

# DERNIERS PROBLEMES
(ecrire chemin comme basename
boucle séparée pour lire dans les fichiers ou fonction même nom mais différente de version à version)
toujours bugs au niveau de getline
FAIRE DOSSIERS SI EXISTENT PAS? DANS V3 ET PLUS

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



A. FICHIER TEMPORAIRE, JUSTE UN PEU PLUS POUR RECONSTITUER A FIN DE DECOMPRESSION
B. SANS FICHIER TEMPORAIRE, REECRIRE COMPRESSION ET DECOMPRESSION ET GET ALPHABET


# **DEUX TRUCS SPECIAUX**
- refuser de décompresser si pas .comphuff?
---


- autres versions
- v6: afficher_arbre_graphique dans le GUI
- remettre static des aux dans arbre.h?
# QUESTIONS
- comment stocker nom du fichier/chemin? deux approches (mp prof?)

---

`xxd -b banane.txt.comphuff`

A FAIRE
- code source + makefile
- notice README (comment créer exécutable, principal, exécutable de génération de fichiers de test et comment générer les fichiers de test)
- présentation LaTeX (sources et pdf)
- versions.txt (description)

PRESENTATION
- regarder notre entête
- démo et étude du taux de compression
- taille - 1 pour encoder la taille
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


# ENTETE
(nb_char_differents)[(char)(nb_bits)(code)...]
- structure alphabet
- nom origine fichier?

# CONTENU
code compressé des caractères du fichier

- soit sauvegarde arbre directement
- soit chaque deux octets (char)(code)
	-> et recrée arbre petit à petit
- nombre max de nb_bits possible?

# Bugs A RESOUDRE ALBOSUMENT
- erreur avec mlv draw text
- un seul caractère: encodé par rien!!!!!!

# Notes

- faire un malloc avec toute la taille direct? pour fread

# Ajouté depuis dernière fois

- noeuds sont au début du tableau normalement lors de la création à partir du nombre d'occurence des caractères

atension:
- (4.2.7) arbre "Une banane" différent? pas grave?
- (4.2) "codage dans l'arbre" même chose que nb d'occurences? PAS SÛR DANS arbre.c à l'assignation de n->codage
- (4.2) j'ai séparé afficher_occurences et creer_noeuds_caracteres, mieux dans une seule boucle?
- (4.2.6) si je mets un < au lieu de <= pour le test min, change des choses (mais <= a l'air mieux)
