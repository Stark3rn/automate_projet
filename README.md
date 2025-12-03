Automates - Documentation
----

# Installation

```sh
gcc -c main.c -o main.o
gcc -c csv_reader.c -o csv_reader.o
gcc -c action_executor.c -o action_executor.o
gcc -c default_automate.c -o default_automate.o
gcc main.o csv_reader.o action_executor.o default_automate.o -o afd_project
```

# Utilisation
```sh
./afd_projet <csv_file_path>
```

Tableau des actions existantes (colonne "Action"). Les arguments sont a inserer dans la colonne "Action_Parametres"
|Afficher |Affiche le message contenu dans "Action_Parametres" dans la console|
|Attendre |Attend (sleep) la duree indiquee dans "Action_Parametres"|
|Executer_Cmd |Execution (systeme) de la commande indiquee dans "Action_Parametres"|
|Log |Ajoute le contenu de "Action_Parametres" dans le fichiers "logs.log"|�

# Securite

Concernant les bonnes pratiques de l'ANSSI, ce code utilise les macros pre-processeurs, et utilise les "ifndef" pour eviter les doublons d'inclusion (et donc les surcharges memoires).
De plus, les fonctions de "string.h" utilisees sont les versions "secure" (strncpy...)

# Erreurs

Toutes les erreurs sont redirigées vers la sortie "stderr" et ont le format "Err : <description de l'erreur> <details de l'erreur si necessaire>".

Tableau des erreurs et leurs solutions 
|Erreur|Solution|
----------------
|Blocage (etat <id_etat> = pas de transition)|Vous n'avez pas specifie de ID_Etat_Suivant ou alors il est incorrect. Veuillez en ajouter un. (9999 pour fin de programme).|
|Ouverture CSV / Chargement automate |Le fichier csv que vous avez specifie n'existe pas ou les permissions en lecture sont insuffisantes. Verifiez le chemin donne et verifiez les permissions (chmod +r <file.csv>)|
|Allocation memoire afd / Chargement automate| Il n'y a pas assez de place en memoire pour charger votre AFD. Liberez de la memoire (fermez des programmes gourmands, relancez votre machine si fuites memoires presentes)|
[Action ID <id> inconnu | Vous avez specifie une action inexistante. Veuillez vous referer a la table des actions ci-dessus (section utilisation)|
|Ouverture log file / ecriture log file|Pas de permissions pour ouvrir / ecrire dans le fichier "logs.log" (repertoire courant d'execution du script)|

# Journalisation

# Tests

# Structure du code
```
| ---- action_executor.c // Fonctions des differentes actions + fonction d'execution des actions
| ---- action_executor.h // Header d'inclusion du fichier "action_executor.c" »
| ---- action_types.h // Association id_action -> nom_action et definition structure "Action" »
| ---- afd.csv // automate de base pour tester les differentes actions
| ---- config_structs.h // structures transition & AFD + parametres ID de fin & nb transis max
| ---- csv_reader.c // Conversion CSV -> structure exploitable par l'automate
| ---- csv_reader.h // Header d'inclusion du fichier "csv_reader.c" »
| ---- default_automate.c // Fonctions de comportement de l'automate par defaut
| ---- default_automate.h // Header d'inclusion du fichier "default_automate.c" �
| ---- main.c // Programme principal : exploite les differents fichiers pour lancer un automate
```
