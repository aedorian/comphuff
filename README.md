## Huffman coding based compression

Project for a programming class. The following instructions are in french

### Créer l'exécutable principal

      make clean; make huff_v[version]
      [version] de 0 à 5 compris

### Créer l'exécutable de la génération de fichiers de tests

      make clean; make

### Comment générer les fichiers de tests?

      ./test.sh

### Comment générer la présentation?

      pdflatex presentation.tex
      (le pdf est également disponible dans presentation.pdf)
# DEBUT
nous n'allons pas réexpliquer le principe de la compression de Huffman.
nous allons seulement revenir sur les points où nous avons dû prendre des décisions.

## 1 FICHIER: 
voici format choisi pour un fichier compressé
pour l'entête, on commence par écrire nb caractères différents
nb chars est encodé comme étant (nb_char - 1) pour que ça rentre sur 8 bits

on encode ensuite l'alphabet de cette manière: 1 char sur 8 bits, le nb bits sur lequel son codage est écrit, puis son codage

on écrit ensuite le contenu du fichier avec les bons codages
puisque l'on écrit octet par octet, il reste des bits après le contenu
on compte ce dépassement et on l'écrit au début du fichier (pour pouvoir le connaître à l'avance) avec la fonction rewind



## PLUSIEURS FICHIERS:
pour la compression de plusieurs fichiers,
il a fallu utiliser un format différent puisque cette fois, nécessaire de stocker nom du fichier (format utilisé de v2 à v5)

on crée donc un fichier temporaire qui va contenir les données des fichiers à compresser, à la suite
le format est celui ci: (expliquer)
pour le choix des séparateurs, le '\n' délimite le chemin du fichier car normalement dans système pas de '\n' dans nom fichier
et ASCII 1 car pas utilisé en pratique dans fichier texte, de plus proba qu'il y en ait 3 est faible
~~(on part des derniers caractères pour faire nos tests)~~

voici le schéma général pour la compression et décompression de plusieurs fichiers
le fichier temporaire est compressé même manière 1 seul fichier, cela devient archive comphuff
lors reconstitution,
lit le chemin du fichier (jusqu'au saut de ligne) et recréer tous les fichiers un par un
si il y a des dossiers, on reconstitue l'arborescence petit à petit

ce qui fait que pour les versions, un fichier compressé avec la v1 ne sera pas décompressable avec la v2 par exemple (pas le même format de fichier)




# ANALYSES
on a enlevé le meilleur et le pire résultat sur 30 tirages (MODIFIE)
justifier l'utilisation de certaines versions:
    - on utilise la v1 car c'est la plus propre, donc aussi la plus efficace
    - pour plusieurs fichiers et dossiers: v4

par rapport au graphique en ordonné : taux de compression : 
1 = taille du fichier compresser identique a la taille du fichier non compresser,
2 = taille du fichier réduit de moitié après la compression


## TAILLE DU FICHIER:
Efficace à partir de plus de 200 caractères. On s'attendait à avoir une compression inefficace en dessous de 50 caractères. Pas efficace en dessous à cause de l'alphabet : 32 caractères. Se stabilise => la taille de l'alphabet devient négligeable à partir de 10 000 octets.
au max on arrive à réduire la taille du fichier d'un quart (divise par 1,6)

## NOMBRE DE CHARS DIFFERENTS:
- 1 et 2 chars: à peine en dessous de 8 fois moins de place: entre 1 et 2 caractères, le codage prend 1 seul bit (0 ou 1). l'entête prend très peu de place mais explique le fait qu'on est à peine en dessous de 8 pour taux compression
- de 3 à 128 caractères: taux décroissant de manière exponentielle
- à plus de 128 caractères: profondeur arbre huffman minimale de 8, donc codage prend 1 octet voire plus donc compresse moins bien
~~"voire plus" car peut prendre 9 ou 10 bits en fonction de la profondeur de l'arbre max (donc prendre plus de bits qu'un octet, d'où le fait que le taux soit inférieur à 1)~~


## FREQUENCE CARACTERES:
avec 8 caractères différents
#### Fréquences dominantes
On s'attendait à une compression de moins en moins efficace, ce qui se vérifie et qui s'explique par construction de l'arbre de huffman, car les caractères dominant sont codé sur moins de bit et sont en majorité.
Plus il y a de carctères dominant, plus ils sont codé sur un nombre de bit grand, jusqu'à une même fréquence qui compresse de manière régulières.
de plus on a une très bonne compression dû au faite qu'il y a peu de caractères différents.

#### Fréquences croissantes
On a ensuite calculé le taux de compression avec une fréquence croissante, toujours sur 8 caractères. L'idée est de s'approcher de la génération d'un arbre de huffman avec une profondeur maximal.
Pour cela, les fréquences des caractères sont des puissance de 2.

La courbe est croissante car plus il y a de caractères dans le fichier, plus on arrive à un arbre de huffman de profondeur maximal. L'aléatoire sur de petit fichier a tendance à équilibré l'arbre.
On s'attendait à une compression efficace, et c'est le cas, la taille du fichier est divisé par 4 avec 100 000 caractères. les caractères les plus fréquent sont codé sur le moins de bit.

~~écart type important car façon de générer fichiers en fonction de la fréquence (aléatoire)~~


## PLUSIEURS FICHIERS:
Pour la compression de plusieurs fichier, on s'attendait à une compression stable, ce qui est le cas après 200 fichiers car on ne calcule qu'un seul alphabet pour tout les fichiers, ce qui fait gagner beaucoup de place. Ici le nom de fichier n'est pas très long et n'impacte pas réellement la compression.
La compression d'un seul ou deux fichiers n'est pas très efficace car nous avons tester sur des fichiers de 1000 caractères, et 64 caractères différents, et le chemin vers le fichier ainsi que le séparateur est pris en compte 
ÉCART TYPE : ????
    semble plus grand car échelle plus petite


## DOSSIERS IMBRIQUÉS
En se qui concerne la profondeur des dossiers, la compression est relativement stable au niveau des fichiers, au allentour de 1.45. Les écart type paraissent plus grand en raison de l'échelle. Cela s'explique par la taille des dossiers, qui sont seulement des numéro, ce qui ne prend pas beaucoup de place dans l'archive.
On pourrait s'attendre à un taux de compression réduit par la taille et les caractères des noms des dossiers et des fichiers.
