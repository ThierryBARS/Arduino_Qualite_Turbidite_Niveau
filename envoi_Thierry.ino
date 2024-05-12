// Ce code est celui du module Arduino embarqué


//-------------------------------------------------------------------------------------------------------------------------------
// On importe les librairies necessairses
//-------------------------------------------------------------------------------------------------------------------------------
#include <Wire.h>
#include<SoftwareSerial.h> 
//#include "rgb_lcd.h"

// macros pour utilisation du sensor de niveau d'eau
#define NO_TOUCH       0xFE
#define SEUIL      100
#define ATTINY1_HIGH_ADDR   0x78
#define ATTINY2_LOW_ADDR   0x77

//-------------------------------------------------------------------------------------------------------------------------------
//
//                                        Déclaration des constantes
//
//-------------------------------------------------------------------------------------------------------------------------------
// Constantes utilisées pour initialiser RGB - Red/Green/Blue - de l'afficheur LCD 
/*const int colorR = 0;
const int colorG = 255;
const int colorB = 0;*/
//---------------------------------------------
// Vitesse des communications de la ligne serie
//---------------------------------------------
const int Vitesse_Serie = 9600;
//---------------------------------------------
// Vitesse des communications de la ligne XBEE
//---------------------------------------------
const int Vitesse_XBEE = 9600;
//-----------------------------------------------------------------------------
// Broches de l'arduino utilisée pour le bouton Test
const int Broche_btn_Test = 4;   // 
//-----------------------------------------------------------------------------
// Broches de l'arduino utilisées pour la transmissio XBEE
//-----------------------------------------------------------------------------
const int Broche_Reception = 2;
const int Broche_Transmission = 3;
//-----------------------------------------------------------------------------
// Broches de l'arduino utilisées pour la transmissio XBEE
//-----------------------------------------------------------------------------
const int Broche_relay =  5; 
//-----------------------------------------------------------------------------
// Declaration de 2 tableaux pour le sensor de Niveau d'eau
//-----------------------------------------------------------------------------
unsigned char donnees_basses[8] = {0};
unsigned char donnees_hautes[12] = {0};
//-----------------------------------------------------------------------------
// Declaration de 2 tableaux pour le sensor de Niveau d'eau
//-----------------------------------------------------------------------------
#define NO_TOUCH       0xFE
#define SEUIL      100
#define ATTINY1_HIGH_ADDR   0x78
#define ATTINY2_LOW_ADDR   0x77
//-----------------------------------------------------------------------------
// le capteur turbidité utilise A0
// le capteur qualite utilise A1
//-----------------------------------------------------------------------------
const int Broche_Turbidite = A0;
const int Broche_Qualite = A1;

//-------------------------------------------------------------------------------------------------------------------------------
//
//                                         Déclaration des Variables
//
//-------------------------------------------------------------------------------------------------------------------------------
// Variable "lcd" de type rgb_lcd => ça sera notre affichage LCD
// rgb_lcd lcd;
//------------------------------------------------------------------
// Déclaration de notre module de communication 
// brancher le module xbee sur la broche D2/D3 (2 Broche_Reception, 3 Broche_Transmission)
SoftwareSerial xbee(Broche_Reception, Broche_Transmission);  
//------------------------------------------------------------------

int btn_Poussoir_ETAT = LOW ;     // état actuel du bouton poussoir
int btn_Poussoir_ETAT_Prec = LOW; // état precedent du bouton poussoir
bool etat_TEST = false;           // état actuel du système
/*
char caract_lu = 'z';
char caract_lu_prec = 'p';
*/

void setup()
{
  Serial.begin(Vitesse_Serie);    // initialisation de la communication serie pour Arduino
  xbee.begin(Vitesse_XBEE);       // initialisation de la communication serie pour le module xbee
  Wire.begin();
  pinMode(Broche_btn_Test,INPUT); // Déclaration du bouton test en entrée
  pinMode(Broche_relay, OUTPUT);  // Déclaration du relai en sortie
 // lcd.begin(16, 2); //initialisation de l'affcheur LCD//
  //lcd.setRGB(colorR , colorG, colorB);
}


/*
void Receptionne_XBEE()
{
  caract_lu=xbee.read();
  
  caract_lu="T0";// a effacer, en test jusqu'à remise en etat d'xbee

  if (caract_lu_prec != caract_lu){
    if (caract_lu =="T0") //si xbee recoit 'z//
      {
         Serial.println("Reçu: T0");           // ecrire Z sur le moniteur serie 
         caract_lu_prec=caract_lu;
      }
    else 
      if (caract_lu == "T1"){
        Serial.println("Reçu: T1");           // ecrire P sur le moniteur serie
        caract_lu_prec=caract_lu;
      }
  }
   switch (*(caract_lu)) { // premier caractere de la chaine (commande)
    case 'T':
          if (*(caract_lu+1) == '1')   // lecture du 2eme caractere
            { digitalWrite(Broche_relay, HIGH); }
          else   
            { digitalWrite(Broche_relay, LOW);  }
          break;
    default:
      break;
  }
}
*/

void Envoie_XBEE(char commande, char *valeur)
{
  Serial.print("Envoi_xbee: (dans envoie_xbee): ");
  Serial.print(commande);
  Serial.println(valeur);
  
  xbee.write(commande);  
  xbee.write(valeur);
}


bool Lecture_Boutton_Check()
// Comme c'est un bouton poussoir et qu'on ne veut introduire aucun delai inutile on memorise 
// l'etat précédent du bouton et on ne s'interesse qu'au chngement d'état
{
  btn_Poussoir_ETAT = digitalRead (Broche_btn_Test);  //lecture de l'état du bouton//
//  lcd.setCursor(0, 1);

  if(btn_Poussoir_ETAT_Prec!=btn_Poussoir_ETAT){
    if (btn_Poussoir_ETAT == HIGH) { //si on appuye sur le bouton
        if (etat_TEST == true){
          etat_TEST = false;
          Envoie_XBEE('C',"0");
          digitalWrite(Broche_relay, LOW);
          }
        else { 
          etat_TEST =  true;
          Envoie_XBEE('C',"1");
          digitalWrite(Broche_relay, HIGH); 
        }  
        btn_Poussoir_ETAT_Prec = btn_Poussoir_ETAT;
    }
    else {                  
      btn_Poussoir_ETAT_Prec = btn_Poussoir_ETAT;
    }
  }
  return etat_TEST;
}
//-----------------------------------------------------------------------------------------
//
// Section  relative au capteur de niveau d'eau
//
//-----------------------------------------------------------------------------------------
void Niveau_lire_12_DonneesHautes(void)
{
  memset(donnees_hautes, 0, sizeof(donnees_hautes));
  Wire.requestFrom(ATTINY1_HIGH_ADDR, 12);
  while (12 != Wire.available());

  for (int i = 0; i < 12; i++) { donnees_hautes[i] = Wire.read(); }
  delay(10);
}

void Niveau_lire_8_DonneesBasses(void)
{
  memset(donnees_basses, 0, sizeof(donnees_basses));
  Wire.requestFrom(ATTINY2_LOW_ADDR, 8);
  while (8 != Wire.available());

  for (int i = 0; i < 8 ; i++) { donnees_basses[i] = Wire.read(); }// receive a byte as character
  delay(10);
}

int Verif_Niveau_Eau()
{
  char valeur[6];
  int pourcentage;
  int compteur_section_basse = 0;
  int compteur_section_haute = 0;
  
    uint32_t barre_recouverte = 0;
    uint8_t nombre_de_barres = 0;
    Niveau_lire_8_DonneesBasses();
    Niveau_lire_12_DonneesHautes();
 
    for (int i = 0; i < 8; i++) {
      // On place un 1 au ième rang de barre_recouverte sans changer les autres bits
      if (donnees_basses[i] > SEUIL) {
        barre_recouverte |= 1 << i;
      }
    }
    for (int i = 0 ; i < 12; i++) {
      // On place un 1 au ième rang de barre_recouvertea partir du 8ème sans changer les autres bits
      if (donnees_hautes[i] > SEUIL) {
        barre_recouverte |= (uint32_t)1 << (8 + i);
      }
    }

    while (barre_recouverte & 0x01) 
    // On parcours chaque bit de "barre_recouverte" s'il est egale à 1 en masquant les autres bits avec 0x01
    // on incremente le nombre de barre et ca tant qu'on ne rencontre pas de 0
    {
      nombre_de_barres++;
      barre_recouverte >>= 1; // on passe au bit suivant
    }

    pourcentage = nombre_de_barres * 5;
    itoa(pourcentage,valeur,10); //converti l'entier en chaine de caracteres (10 = base 10)
    Envoie_XBEE('N',valeur); 
    return  pourcentage; //retourne le niveau d'eau en %, il y a 20 barres d'ou le x5 pour le pourcentage
}
//-----------------------------------------------------------------------------------------
//
// Section  relative au capteur de turbidite
//
//-----------------------------------------------------------------------------------------

float  Verif_Turbidite(void)
{
  char turbidite[6];
  int capteur_Turbidite = analogRead(Broche_Turbidite);// Lire la valeur analogique sur la broche Broche_Turbidite
  float tension = capteur_Turbidite * (5.0 / 1024.0); // Converti la valeur entiere (qui va de 0 à 1023) en une tension de  (0 - 5V)

  dtostrf(tension,5,2,turbidite); //dtostrf(float_value, min_width, num_digits_after_decimal, where_to_store_string)
  Envoie_XBEE('T',turbidite); 
  return tension;
}
//-----------------------------------------------------------------------------------------
//
// Section  relative au capteur de Qualité
//
//-----------------------------------------------------------------------------------------
float  Verif_Qualite(void)
{
  char qualite[6];
  int capteur_Qualite = analogRead(Broche_Qualite);// Lire la valeur analogique sur la broche Broche_Qualite
  float tension = capteur_Qualite * (5.0 / 1024.0); // Converti la valeur entiere (qui va de 0 à 1023) en une tension de  (0 - 5V)

  dtostrf(tension,5,2,qualite); //dtostrf(float_value, min_width, num_digits_after_decimal, where_to_store_string)
  Envoie_XBEE('Q',qualite); 
  return tension;
}
//-----------------------------------------------------------------------------------------
//
//                                  Boucle principale
//
//-----------------------------------------------------------------------------------------
void loop()
{
  int Capteur_Niveau;
  float  Capteur_Turbidite;
  float  Capteur_Qualite;
  bool ON_OFF;

  ON_OFF = Lecture_Boutton_Check();
  Capteur_Niveau = Verif_Niveau_Eau();
  Capteur_Turbidite = Verif_Turbidite();
  Capteur_Qualite = Verif_Qualite();
 
  Serial.print("Bouton Check      (dans loop): ");  Serial.println(ON_OFF);
  Serial.print("Capteur_Niveau    (dans loop): ");  Serial.println(Capteur_Niveau);
  Serial.print("Capteur_Turbidite (dans loop): ");  Serial.println(Capteur_Turbidite);
  Serial.print("Capteur_Qualité   (dans loop): ");  Serial.println(Capteur_Qualite);

   delay(2000);


  //Receptionne_XBEE();
    //    lcd.print("OUVERT");  //l'ecran LCD affiche ouvert//
}
