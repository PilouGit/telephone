# Téléphone à Cadran Rotatif Connecté

Projet Arduino permettant de transformer un ancien téléphone à cadran rotatif en téléphone GSM fonctionnel.

## Description

Ce projet donne une seconde vie aux téléphones vintage en les connectant au réseau GSM moderne. Le système détecte la composition sur le cadran rotatif, passe des appels via un module SIM900, et joue une musique d'attente pendant la connexion.

## Fonctionnalités

- **Composition par cadran rotatif** : Détection des impulsions du cadran pour composer les numéros
- **Appels GSM** : Passage et réception d'appels via le réseau mobile
- **Musique d'attente** : Lecture automatique d'une mélodie pendant la connexion
- **Détection du combiné** : Reconnaissance du décrochage/raccrochage
- **Formatage automatique** : Conversion des numéros belges et français au format international

## Matériel Requis

### Composants principaux
- **Arduino** (Uno, Nano ou compatible)
- **Module SIM900/SIM900A** avec carte SIM active
- **DFPlayer Mini** avec carte micro SD
- **Téléphone à cadran rotatif** vintage

### Connexions

| Composant | Pin Arduino | Description |
|-----------|-------------|-------------|
| Cadran (impulsions) | 13 | Fil rouge - Détection des impulsions |
| Cadran (rotation) | 12 | Fil bleu - Détection de la rotation |
| Combiné | 7 | Capteur décroché/raccroché |
| DFPlayer RX | 5 | Communication série avec DFPlayer |
| DFPlayer TX | 6 | Communication série avec DFPlayer |
| SIM900 RX | 2 | Communication série avec SIM900 |
| SIM900 TX | 3 | Communication série avec SIM900 |

## Installation

### 1. Bibliothèques Arduino

Installez les bibliothèques suivantes via le gestionnaire de bibliothèques Arduino :

- `DFRobotDFPlayerMini` - Pour le contrôle du lecteur MP3
- `SoftwareSerial` (incluse par défaut)

### 2. Préparation de la carte SD

1. Formatez une carte micro SD en FAT32
2. Créez un dossier `mp3` à la racine
3. Ajoutez votre fichier audio de musique d'attente nommé `0001.mp3`

### 3. Carte SIM

- Insérez une carte SIM active (avec crédit) dans le module SIM900
- Désactivez le code PIN de la carte SIM

### 4. Téléchargement

1. Ouvrez `telephone_script.ino` dans l'IDE Arduino
2. Sélectionnez votre carte Arduino
3. Téléversez le code

## Configuration

### Modifier le numéro de test

Dans la fonction `setup()` (ligne 210), vous pouvez modifier le numéro appelé au démarrage :

```cpp
appellerAvecMusique("0033685561706");  // Remplacez par votre numéro
```

### Réglage du volume

Dans `setupSound()` et `appellerAvecMusique()`, ajustez le volume (0-30) :

```cpp
myDFPlayer.volume(20);  // Valeur entre 0 et 30
```

### Formats de numéros supportés

Le système reconnaît automatiquement ces formats :

- **Format belge court** : `02XXXXXX` ou `04XXXXXX` → converti en `0032XXXXXXXXX`
- **Format français international** : `0033XXXXXXXXX` (13 chiffres)
- **Format belge international** : `0032XXXXXXXXX` (13 chiffres)

## Utilisation

1. **Décrochez le combiné** - Le système détecte le décrochage
2. **Composez le numéro** sur le cadran rotatif
3. **La musique d'attente démarre** automatiquement
4. **L'appel est établi** - La musique s'arrête quand le correspondant décroche
5. **Raccrochez** pour terminer l'appel

## Détails Techniques

### États du système

- `appelEnCours` : Indique si un appel est en cours
- `numtel` : Stocke le numéro composé progressivement
- `chiffre` : Compteur d'impulsions pour chaque rotation du cadran

### Fonctions principales

- `setupPhone()` : Initialisation des pins du téléphone
- `setupSound()` : Initialisation du DFPlayer Mini
- `setupSIM()` : Initialisation et test du module SIM900
- `appellerAvecMusique()` : Lance l'appel avec musique d'attente
- `verifierAppelEntrant()` : Détecte les appels entrants
- `repondreAppel()` : Répond à un appel entrant
- `raccrocherAppel()` : Termine l'appel en cours

### Communication série

Le système utilise deux liaisons SoftwareSerial :
- **DFPlayer** : 9600 bauds
- **SIM900** : 19200 bauds

Le moniteur série (9600 bauds) affiche les événements pour le débogage.

## Dépannage

### Le DFPlayer ne fonctionne pas

- Vérifiez que la carte SD est bien formatée en FAT32
- Assurez-vous que le fichier audio est nommé `0001.mp3` dans le dossier `mp3`
- Vérifiez les connexions RX/TX (attention au croisement)

### Le SIM900 ne répond pas

- Vérifiez que la carte SIM est bien insérée et active
- Le code PIN doit être désactivé
- Assurez-vous que le module SIM900 est correctement alimenté (besoin d'une alimentation externe)
- Vérifiez les connexions série

### Le cadran ne fonctionne pas

- Testez la continuité des fils du cadran
- Vérifiez que les pins utilisent `INPUT_PULLUP`
- Utilisez le moniteur série pour déboguer les impulsions

## Améliorations Possibles

- [ ] Répertoire téléphonique avec raccourcis
- [ ] Historique des appels
- [ ] Sonnerie personnalisée pour les appels entrants
- [ ] Écran LCD pour afficher le numéro composé
- [ ] Support de la numérotation à fréquences vocales (DTMF)

## Licence

Ce projet est sous licence libre. Vous êtes encouragé à le modifier et le partager.

## Contributions

Les contributions sont les bienvenues ! N'hésitez pas à :
- Signaler des bugs
- Proposer de nouvelles fonctionnalités
- Améliorer la documentation

## Auteur

Projet de téléphone vintage Arduino GSM

---

**Note** : Ce projet nécessite une carte SIM active avec un forfait voix. Les coûts des appels dépendent de votre opérateur.
