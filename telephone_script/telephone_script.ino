#define impulsions 13  // Cadran - fil rouge
#define rotation 12    // Cadran - fil bleu
#define combine 7      // Capteur DÃ©crochÃ© RacrochÃ©
int chiffre;
byte etat = 0;
String numtel="";
void setup() {
  pinMode(impulsions, INPUT_PULLUP);
  pinMode(rotation, INPUT_PULLUP);
  pinMode(combine, INPUT_PULLUP);
  Serial.begin(9600);



  // put your setup code here, to run once:
}

bool estDecroche()
{
  return digitalRead(combine)== LOW;
}
void loop() {
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
       if (numtel.length()==10) {
       Serial.println(numtel);
       }
     
    }
  }else
  {
    numtel="";
  }
}