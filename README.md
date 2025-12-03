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

| Action       | Description                                                               |
| ------------ | ------------------------------------------------------------------------- |
| Afficher     | Affiche le message contenu dans **Action_Parametres** dans la console     |
| Attendre     | Attend (sleep) la durée indiquée dans **Action_Parametres** (en secondes) |
| Executer_Cmd | Exécute la commande système indiquée dans **Action_Parametres**           |
| Log          | Ajoute le contenu de **Action_Parametres** dans le fichier `logs.log`     |

# Securite

Concernant les bonnes pratiques de l'ANSSI, ce code utilise les macros pre-processeurs, et utilise les "ifndef" pour eviter les doublons d'inclusion (et donc les surcharges memoires).
De plus, les fonctions de "string.h" utilisees sont les versions "secure" (strncpy...)

# Erreurs

Toutes les erreurs sont redirigées vers la sortie "stderr" et ont le format "Err : <description de l'erreur> <details de l'erreur si necessaire>".

| Erreur                                       | Solution                                                                                                                           |
| -------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------- |
| Blocage (etat <id_etat> = pas de transition) | Vérifiez que **ID_Etat_Suivant** est correct. Ajoutez un ID valide (ex : 9999 pour fin de programme)                               |
| Ouverture CSV / Chargement automate          | Le fichier CSV n’existe pas ou les permissions de lecture sont insuffisantes. Vérifiez le chemin et utilisez `chmod +r <file.csv>` |
| Allocation mémoire AFD / Chargement automate | Pas assez de mémoire pour charger l’automate. Fermez des programmes gourmands ou relancez la machine si nécessaire                 |
| Action ID <id> inconnue                      | Vous avez spécifié une action inexistante. Référez-vous à la table des actions                                                     |
| Ouverture/écriture fichier log               | Pas de permissions pour ouvrir/écrire dans `logs.log`. Vérifiez les droits sur le répertoire courant                               |


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
