#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

#define impulsions 13  // Cadran - fil rouge
#define rotation 12    // Cadran - fil bleu
#define combine 7      // Capteur DÃ©crochÃ© RacrochÃ©
#define dfPlayerRX 5
#define dfPlayerTX 6
#define simRX 2
#define simTX 3

SoftwareSerial softSerial(/*rx =*/dfPlayerRX, /*tx =*/dfPlayerTX);
SoftwareSerial SIM900(simRX, simTX); 
DFRobotDFPlayerMini myDFPlayer;
int chiffre;
byte etat = 0;
String numtel="";
bool appelEnCours = false;  // Variable pour suivre l'état de l'appel
void setupPhone()
{
 pinMode(impulsions, INPUT_PULLUP);
  pinMode(rotation, INPUT_PULLUP);
  pinMode(combine, INPUT_PULLUP);

}
void setupSound()
{
  softSerial.begin(9600);

 
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(softSerial, /*isACK = */true, /*doReset = */true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(20);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3
  delay(3000);  // Attendre 3 secondes
  myDFPlayer.pause();  // Arrêter la lecture

}

void setupSIM()
{
  Serial.println();
  Serial.println(F("Initializing SIM900..."));

  SIM900.begin(19200);  // Initialiser la communication avec le SIM900
  delay(1000);

  // Envoyer la commande AT
  SIM900.println("AT");
  delay(100);

  // Lire la réponse
  unsigned long timeout = millis();
  String response = "";

  while (millis() - timeout < 1000) {
    if (SIM900.available()) {
      char c = SIM900.read();
      response += c;
    }
  }

  // Vérifier la réponse
  if (response.indexOf("OK") != -1) {
    Serial.println(F("SIM900 is ready!"));
  } else {
    Serial.println(F("SIM900 not responding!"));
    Serial.print(F("Response: "));
    Serial.println(response);
  }
}

// Fonction pour passer un appel
void appellerNumero(String numero) {
  Serial.print(F("Appel en cours vers: "));
  Serial.println(numero);

  // Commande AT pour composer le numéro
  SIM900.print("ATD");
  SIM900.print(numero);
  SIM900.println(";");

  delay(100);

  appelEnCours = true;  // Marquer l'appel comme en cours
  Serial.println(F("Appel initié"));
}

// Fonction pour vérifier si on reçoit un appel
bool verifierAppelEntrant() {
  if (SIM900.available()) {
    String response = "";

    // Lire la réponse du SIM900
    while (SIM900.available()) {
      char c = SIM900.read();
      response += c;
      delay(10);
    }

    // Vérifier si c'est un appel entrant (RING)
    if (response.indexOf("RING") != -1) {
      Serial.println(F("Appel entrant détecté!"));

      // Extraire le numéro si disponible (format: +CLIP: "numero",...)
      int clipIndex = response.indexOf("+CLIP:");
      if (clipIndex != -1) {
        int startQuote = response.indexOf("\"", clipIndex);
        int endQuote = response.indexOf("\"", startQuote + 1);
        if (startQuote != -1 && endQuote != -1) {
          String numeroAppelant = response.substring(startQuote + 1, endQuote);
          Serial.print(F("Numéro: "));
          Serial.println(numeroAppelant);
        }
      }

      return true;
    }
  }

  return false;
}

// Fonction pour répondre à un appel
void repondreAppel() {
  Serial.println(F("Réponse à l'appel"));
  SIM900.println("ATA");
  delay(100);
  appelEnCours = true;  // Marquer l'appel comme en cours
}

// Fonction pour raccrocher
void raccrocherAppel() {
  Serial.println(F("Raccrochage"));
  SIM900.println("ATH");
  delay(100);
  appelEnCours = false;  // Marquer l'appel comme terminé
}

// Fonction pour appeler avec musique d'attente
void appellerAvecMusique(String numero) {
  Serial.println(F("Lancement de l'appel avec musique"));

  myDFPlayer.volume(20);  //Set volume value. From 0 to 30
 // Lancer le premier morceau du DFPlayer
  myDFPlayer.play(1);
  Serial.println(F("Musique démarrée"));

  // Appeler le numéro
  appellerNumero(numero);

  // Attendre que la personne décroche (surveillance des réponses du SIM900)
  unsigned long timeout = millis();
  bool connexionEtablie = false;

  while (millis() - timeout < 30000 && estDecroche()) {  // Timeout de 30 secondes
    if (SIM900.available()) {
      String response = "";
      while (SIM900.available()) {
        char c = SIM900.read();
        response += c;
        delay(10);
      }

      Serial.print(F("SIM900: "));
      Serial.println(response);

      // Vérifier si l'appel est établi (la personne a décroché)
      if (response.indexOf("CONNECT") != -1 || response.indexOf("OK") != -1) {
        connexionEtablie = true;
        break;
      }

      // Vérifier si l'appel a échoué
      if (response.indexOf("NO CARRIER") != -1 ||
          response.indexOf("BUSY") != -1 ||
          response.indexOf("NO ANSWER") != -1) {
        Serial.println(F("Appel non abouti"));
        myDFPlayer.pause();
        return;
      }
    }
  }

  // Arrêter la musique quand la personne décroche
  if (connexionEtablie) {
    Serial.println(F("Connexion établie - Arrêt de la musique"));
    myDFPlayer.pause();
  } else {
    Serial.println(F("Timeout ou combiné raccroché"));
    myDFPlayer.pause();
  }
}

void setup() {
   Serial.begin(9600);
   setupSIM();
    setupPhone();
setupSound();
 appellerAvecMusique("0033685561706");
  // put your setup code here, to run once:
}

bool estDecroche()
{
  return digitalRead(combine)== LOW;
}

String verifierEtFormaterNumero(String numero) {
  int longueur = numero.length();
  Serial.println("verifierEtFormaterNumero("+numero+")");
  // Format 0033 + 9 chiffres (13 chiffres total)
  if (longueur == 13 && numero.startsWith("0033")) {
    return numero;
  }

  // Format 0032 + 9 chiffres (13 chiffres total)
  if (longueur == 13 && numero.startsWith("0032")) {
    return numero;
  }

  // Format 04 + 6 chiffres (8 chiffres total)
  if (longueur == 8 && numero.startsWith("04")) {
     String numeroFormate = "0032" + numero.substring(1);
    return numeroFormate;
  }

  // Format 02 + 6 chiffres (8 chiffres total) - remplacer 0 par 0032
  if (longueur == 8 && numero.startsWith("02")) {
    String numeroFormate = "0032" + numero.substring(1);
    return numeroFormate;
  }

  return "";
}

bool numeroEstComplet(String numero) {
  int longueur = numero.length();
Serial.println("numeroEstComplet("+numero+")");
  
  // Vérifier si c'est un format valide et complet
  if ((longueur == 13 && (numero.startsWith("0033") || numero.startsWith("0032"))) ||
      (longueur == 8 && (numero.startsWith("04") || numero.startsWith("02")))) {
    return true;
  }

  return false;
}
void loop() {
 
  /*
  // Vérifier si on raccroche pendant un appel
  if (appelEnCours && !estDecroche()) {
    raccrocherAppel();
  }

  if (estDecroche()) {
    // si telephone est decroché

    if ((digitalRead(rotation) == LOW)) {
      chiffre = 0;
      while (digitalRead(rotation) == LOW) {
        int reading = digitalRead(impulsions);

        if (etat != reading) {
          if (reading == LOW) chiffre++;
          etat = reading;
        }
        delay(20);
       }
      if (chiffre >= 10) chiffre = 0;
      numtel += chiffre;

      if (numeroEstComplet(numtel)) {
        String numeroFormate = verifierEtFormaterNumero(numtel);
        Serial.println(numeroFormate);
        appellerAvecMusique(numeroFormate);  // Appeler avec musique
        numtel = ""; // Réinitialiser après affichage
      }

    }
  }else
  {
    numtel="";
  }*/
}

void printDetailDfPlayer(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }}