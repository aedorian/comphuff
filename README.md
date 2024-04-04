# Projet programmation impérative 3
## OUVERTURE BINAIRE DE LA BASE

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
