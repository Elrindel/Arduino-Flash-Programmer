# Arduino Flash Programmer

Ce projet est en deux parties.

La partie ``[PC]`` correspond au programme en Python qui permet une utilisation simplifiée de la partie ``[Arduino]``.

Le schéma pour raccorder la mémoire à l'Arduino est disponible dans le dossier ``Schema``. Les fichiers **.pro** et **.sch** sont fait pour le logiciel Kicad (le fichier **.jpg** est simplement une capture d'écran du schéma).

Retrouvez tous les détails de ce projet dans mon article : [Programmer une mémoire Flash (EEPROM) 32 broches avec un Arduino](https://elrindel.github.io/programmateur-eeprom-diy-arduino)

## [PC] Utilisation du client en Python

```
Usage : flashprogrammer.py [OPTION]... [DATAS]

Options :
   -p, --port        Serial port
   -s, --sequence    Flash sequence, see below (default : 1)
   -c, --command     Flash command, see below (default : R)
   -a, --address     Flash memory address for R and W commands, see the format below (default : 0 for R command)
   -l, --length      Data length for R and W commands, see the format below (default : 1 for R command, data length for W command)
   -f, --file        DATAS is a file for W and R command
   -x, --hex         To get the results of the I and R commands in hexadecimal

Flash sequence :
   1  : 5555/AA, 2AAA/55, 5555/A0
   2  : 555/AA, 2AA/55, 555/A0

Flash command :
   I  : Get manufacturer ID and device ID (2 bytes)
   R  : Read address (use -a)
   W  : Write data to address (use -a, -l, -f, DATAS)
   E  : Erase memory

Address and Length format :
   [0-9]+     for a decimal value (example : 1234)
   x[0-9A-F]+ for hexadecimal value (example : x4D2)
```

**ATTENTION :** Ce programme ne gère pas les tailles des mémoires. Si vous tentez d'écrire plus loin que le permet votre mémoire alors l'écriture recommencera au début de la mémoire, au risque donc d'écraser d'autres données.

C'est donc à vous de calculer la taille des données que vous souhaitez envoyer/lire suivant la capacité de votre mémoire (et l'adresse à partir de laquelle vous commencez à lire/écrire).

### Exemples

**Lecture de la mémoire avec enregistrement du résultat dans un fichier :**

``flashprogrammer.py -f -p COM1 -c R -l x80000 fichier-de-sauvegarde.bin``

*Pour l'exemple j'ai mis l'option -f au début pour bien montrer que cette option ne permet pas de définir le fichier directement mais permet simplement d'interpréter DATAS comme étant un fichier (DATAS étant toujours à la fin de la commande), dans la pratique j'ai plutôt tendance à mettre -f juste avant DATAS.*

Cette commande va donc lire **524288 octets (512Ko)** à partir de l'adresse **0**, et enregistrer le résultat dans le fichier ``fichier-de-sauvegarde.bin``.

**Ecriture d'un fichier dans la mémoire :**

``flashprogrammer.py -f -p COM1 -c W -a 0 fichier-a-ecrire.bin``

*Pour l'écriture, l'adresse de départ doit être spécifiée afin d'éviter d'écrire à partir de l'adresse 0 par erreur. Même remarque pour précédemment pour l'option -f.*

Cette commande va écrire toutes les données du fichier ``fichier-a-ecrire.bin`` dans la mémoire à partir de l'adresse **0**. Puisque la taille des données n'est pas spécifiée dans la commande, tout le fichier sera écrit.

**Ecrire des données dans la mémoire :**

``flashprogrammer.py -p COM1 -c W -a 0 TEST WRITE DATA``

Cette commande va écrire ``TEST WRITE DATA`` dans la mémoire à partir de l'adresse **0**.

## [Arduino] Communication série avec l'Arduino

### Paramètres de connexion

Configuration de la communication série : **8 data bits, no parity, 1 stop bit, speed 115200 baud**

**ATTENTION :** Comme pour le programme en Python, ce code ne gère pas la taille de la mémoire, c'est à vous de savoir quelle taille fait votre mémoire et d'adapter les adresses ainsi que la longueur des données que vous tentez de lire/écrire.

### Séquences

Vous devez utiliser les bonnes séquences pour communiquer avec votre mémoire. Vérifier donc le datasheet pour savoir laquelle utiliser.

Les séquences sont préconfigurées directement dans le code Arduino dans la fonction ``set_sequence``.

Actuellement le programme possède deux séquences (les deux plus courantes) mais ne possède pas la commande de verrouillage/déverrouillage de l'écriture, c'est une feature que j'ajouterai certainement dans une prochaine version (pour bien faire il faudrait que j'ai en ma possession une mémoire avec cette option ...).

### Commandes

Une commande correspond à un caractère suivi de ses arguments (si nécessaire).

Le premier argument doit être collé au caractère de la commande, puis les suivants doivent être séparés par un espace ou un retour à la ligne (voir les exemples ci-dessous).

Les arguments sont soit en décimal, soit en hexadécimal préfixés avec le caractère **x**.

Voici la liste des commandes (voir les détails de chaque commande plus bas) :

| Commande | Nb Arg | Description                                                              |
| -------- | ------ | ------------------------------------------------------------------------ |
| ``T``    | 0      | Change de timeout pour les commandes envoyées via la communication série |
| ``S``    | 1      | Définit les séquences à utiliser                                         |
| ``R``    | 2      | Lit *x* octets à partir de l'adresse spécifiée                           |
| ``W``    | 2-3    | Ecrit *x* octets à partir de l'adresse spécifiée                         |
| ``E``    | 0      | Efface complètement toute la mémoire                                     |
| ``I``    | 0      | Retourne l'identifiant du fabriquant et du modèle                        |

Les commandes ``R`` et ``I`` peuvent être préfixées du caractère **X** afin de récupérer le résultat en hexadécimal.

#### Commande ``T``

Le timeout par défaut est de ``200ms``.

Chaque appel à cette commande permet d'augmenter le timeout de ``2000ms`` jusqu'au maximum de ``14200ms`` puis reboucle à partir de ``200ms``.

Le but étant de permettre une utilisation manuelle tout en aillant un timeout assez faible par défaut pour limiter les latences lors de l'utilisation via un programme.

Ce timeout est cependant utilisé uniquement pour les arguments et est nécessaire seulement si un argument n'est pas terminé par un caractère espace ou un retour à la ligne.

Le timeout permet donc de terminer la saisie d'un argument pour ne pas rester bloquer sur cette étape. La seule saisie bloquante est réalisée lors de l'écriture (voir les détails ci-dessous).

**Exemples :**

- ``T``

**Valeurs de retour :**

- ``OK TIMEOUT`` *(``TIMEOUT`` étant la valeur décimale du timeout)*

#### Commande ``S``

Sélectionne une des séquences préconfigurées dans le programme (dans la fonction ``set_sequence``).

**Arguments :**

1. Numéro de séquence

**Exemples :**

- ``S2`` *(sélectionne la 2ème séquence préconfigurée)*

**Valeurs de retour :**

- ``OK`` Si la séquence demandée est bien disponible dans le programme
- ``ERR`` Si la séquence est introuvable

#### Commande ``R``

**Arguments :**

1. Adresse ciblée
2. Nombre d'octets à lire

**Exemples :**

- ``R12345 50`` *(Lit 50 octets à partir de l'adresse **12345 soit 0x3039**)*
- ``RxFFFF 1`` *(Lit l'octet à l'adresse **0xFFFF**)*
- ``XR1234 x20`` *(Lit 32 octets à partir de l'adresse **1234 soit 0x4D2** et retourne le résultat en hexadécimal)*

**Valeurs de retour :**

- Données brutes ou représentation hexadécimale si la commande est préfixée par un **X**
- ``ERR`` Si l'adresse ou la taille sont incorrectes

#### Commande ``W``

Le buffer de la communication série de l'Arduino étant limité, l'écriture est faite par **lot de 32 octets** et il est important d'attendre que ces octets soient bien traités avant d'envoyer la suite, sinon il y a un risque de perdre des données au passage !

Afin de savoir si l'Arduino est prêt à recevoir la suite des données/instructions, le programme envoi le caractère **>**. Ce caractère est donc envoyé dès le début de l'écriture, puis tous les 32 caractères, puis une dernière fois à la fin pour indiquer que le traitement est bien terminé.

Pour écrire moins de 32 caractères, il est possible d'envoyer les données directement en argument sans tenir compte des caractères **>** (en théorie ça devrait fonctionne pour des données plus longues, le buffer étant de 64 octets, mais il vaut mieux prévoir une marge de sécurité).

Il n'y a pas de timeout sur l'envoi des données à écrire ! Tant que le nombre d'octets spécifié en argument n'est pas atteint, le programme attendra de recevoir les données.

**Arguments :**

1. Adresse ciblée
2. Nombre d'octets à écrire
3. Données à écrire (voir le texte ci-dessus pour plus de détails)

**Exemples :**

- ``W12345 5 DATAS`` *(Demande à écrire 5 octets à partir de l'adresse **12345** puis va écrire **DATAS**)*
- ``Wx1234 500`` *(Demande à écrire 500 octets à partir de l'adresse **0x1234**)*

**Valeurs de retour :**

- ``>`` Lorsque le programme est en attente de données ou vient de finir de traiter les données
- ``OK SIZE`` Fin d'écriture des données *(``SIZE`` étant le nombre d'octets écrits)*
- ``ERR`` Si l'adresse ou la taille sont incorrectes

#### Commande ``E``

Efface simplement l'intégralité de la mémoire.

**Exemples :**

- ``E``

**Valeurs de retour :**

- ``OK``

#### Commande ``I``

Retourne les identifiants du fabricant et du modèle de la mémoire.

Si cette commande retourne des valeurs qui ne correspondent pas à la mémoire, soit vous utilisez la mauvaise séquence, soit il y a un problème de timing dans le programme Arduino (des délais trop longs ou trop courts).

**Exemples :**

- ``I``
- ``XI`` *(Pour récupérer les valeurs en hexadécimal)*

**Valeurs de retour :**

- 2 octets représentants respectivement ID fabricant et ID modèle ou représentation hexadécimale des 2 octets si la commande est préfixée du caractère **X**