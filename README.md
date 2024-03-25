# Projet programmation impérative 3
## OUVERTURE BINAIRE DE LA BASE

A FAIRE
- code source + makefile
- notice README (comment créer exécutable, principal, exécutable de génération de fichiers de test et comment générer les fichiers de test)
- présentation LaTeX (sources et pdf)
- versions.txt (description)

PRESENTATION
- démo et étude du taux de compression

# Notes

- codage de Huffman: caractères codés sur nombre différent de bits
  - codes caractères fréquents sont courts, ceux des rares sont longs
  - UNE façon de décoder le fichier compressé

# Ajouté depuis dernière fois

add (4.1):
- makefile
- afficher_occurences(int tab[256])
- initialiser_occurences(int tab[256])
- ta fonction usage(char *s)
- l'ajout de l'ouverture du fichier en paramètre

add (4.2):
- arbre.h, arbre.c
- initialiser_arbre_huffman
- afficher_occurences CHANGE et prend maintenant un arbre_huffman, pas un tab d'occurences

atension:
- (4.2) "codage dans l'arbre" même chose que nb d'occurences? PAS SÛR DANS arbre.c à l'assignation de n->codage
- (4.2) j'ai séparé afficher_occurences et creer_noeuds_caracteres, mieux dans une seule boucle?
- (4.2.6) si je mets un < au lieu de <= pour le test min, change des choses (mais <= a l'air mieux)
- CHANGER VERSION POUR METTRE NOEUDS AU DEBUT DU TABLEAU?