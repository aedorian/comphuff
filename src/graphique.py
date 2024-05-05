import csv
import matplotlib.pyplot as plt

def gen(nom_fich, axe_x, axe_y, fct_de, scale="none", base_log=10):
    x = []
    y = []
    e = []

    with open(nom_fich, 'r') as csvfile:
        lines = csv.reader(csvfile, delimiter=',')
        for row in lines:
            x.append(int(row[0]))
            y.append(float(row[1]))
            e.append(float(row[2]))

    plt.errorbar(x, y, yerr = e, capsize = 5, ecolor='red', linestyle = 'none')
    plt.plot(x, y, color = 'g', linestyle = 'dashed', marker = 'o')

    if scale == "log" :
        plt.xscale(scale, base=base_log);
            
    plt.xlabel(axe_x)
    plt.ylabel(axe_y)
    plt.title(f'{axe_y} en fonction {fct_de}', fontsize = 11)
    plt.grid()
    plt.savefig(nom_fich[:-3] + "png")


gen("taille.csv", "Taille", "Taux de compression", "de la taille", "log", 10)
plt.figure(2)
gen("car_diff.csv", "Nombre de caractère différent", "Taux de compression", "du nombre de caractères différent", "log", 2)
plt.figure(3)
gen("freq_dom.csv", "Nombre de caractère dominant", "Taux de compression", "du nombre de caractère dominant", "none")
plt.figure(4)
gen("freq_croiss.csv", "Taille", "Taux de compression", "de la taille, fréquence croissante", "log", 10)
plt.figure(5)
gen("nb_fich.csv", "Nombre de fichier", "Taux de compression", "du nombre de fichier", "log", 10)
plt.figure(6)
gen("prof_doss.csv", "Profondeur des dossiers", "Taux de compression", "de la profondeur des dossiers\n(nom de taille 1 à 2)", "log", 10);
