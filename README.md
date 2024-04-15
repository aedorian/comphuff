# Projet programmation impérative 3
## OUVERTURE BINAIRE DE LA BASE

# AJOUTE DEPUIS
- makefile pour toutes les versions
- début v1 et v2
- fonction pour vérifier si fichier est fichier ou dossier `https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file` dans utils.c
  - possible de tester version v2 (liste fichiers) avec `huff_v2 -c arfchieve fichiers/ eoeoele test.c`
- fonctions au début de huff_v2.c pour les prochaines versions, pour obtenir les noms des fichiers et tout (pour le moment ça les liste juste)

# à faire
- décompression dans un autre dossier: vérifier que le / est bien là ou pas
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
- démo et étude du taux de compression
- taille - 1 pour encoder la taille

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
