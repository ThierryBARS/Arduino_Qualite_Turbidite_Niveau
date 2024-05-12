// Ce code est celui du module Arduino déporté


//-------------------------------------------------------------------------------------------------------------------------------
// On importe les librairies necessairses
//-------------------------------------------------------------------------------------------------------------------------------
#include<SoftwareSerial.h> 
#include "rgb_lcd.h"
#include <Wire.h>


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
const int Broche_btn_Test = 5;   // 
//-----------------------------------------------------------------------------
// Broches de l'arduino utilisées pour la transmissio XBEE
//-----------------------------------------------------------------------------
const int Broche_Reception = 7;
const int Broche_Transmission = 8;
//-----------------------------------------------------------------------------
// Broches de l'arduino utilisées pour la transmissio XBEE
//-----------------------------------------------------------------------------
const int Broche_relay =  5; 
//-----------------------------------------------------------------------------
// Nombre de lignes et de colonnes de l'afficheur LCD
//-----------------------------------------------------------------------------
const int nbr_lgn = 2;  // Nombre de lignes du LCD
const int nbr_col = 16; // Nombre de colonnes du LCD

const int couleur_rouge = 0;
const int couleur_verte = 2550;
const int couleur_bleue = 0;

const int ligne_qualite = 0;
const int ligne_turbidite = 0;
const int ligne_niveau = 1;
const int ligne_pompe = 1;
const int ligne_valve = 1;
const int colonne_qualite = 0;
const int colonne_turbidite = 8;
const int colonne_niveau = 0;
const int colonne_pompe = 8;  // +1 pour ecrire la valeur (ON / OFF)
const int colonne_valve = 12; // +1 pour ecrire la valeur (Ouv(erte) / Fer(mée))

//-----------------------------------------------------------------------------
// Etat ouvert/ferme de la valve
//-----------------------------------------------------------------------------
const char OUVERTE[] = "ouv";
const char FERMEE[] = "fer";
//-----------------------------------------------------------------------------
// Etat on/off de la pompe
//-----------------------------------------------------------------------------
const char ON[] = "on";
const char OFF[] = "off";


//-------------------------------------------------------------------------------------------------------------------------------
//
//                                         Déclaration des Variables
//
//-------------------------------------------------------------------------------------------------------------------------------
// Variable "lcd" de type rgb_lcd => ça sera notre affichage LCD
// rgb_lcd lcd;
//------------------------------------------------------------------
// Déclaration de notre module de communication 
// brancher le module xbee sur la broche D7/D8 (7 Broche_Reception, 8 Broche_Transmission) 
SoftwareSerial xbee(Broche_Reception,Broche_Transmission);  

//------------------------------------------------------------------
// Déclaration de notre afficheur LCD
//------------------------------------------------------------------
rgb_lcd lcd;

/*
int btn_Poussoir_ETAT = LOW ;     // état actuel du bouton poussoir
int btn_Poussoir_ETAT_Prec = LOW; // état precedent du bouton poussoir
bool etat_TEST = false;           // état actuel du système
*/
char *caract_lu = "0";
char *caract_lu_prec = "0";


void setup() {
  Serial.begin(Vitesse_Serie);      // initialisation de la communication serie pour Arduino
  xbee.begin(Vitesse_XBEE);         // initialisation de la communication serie pour le module xbee
  pinMode(Broche_btn_Test,INPUT);   // Déclaration du bouton test en entrée
  
  pinMode(Broche_relay, OUTPUT);    // Déclaration du relai en sortie
  digitalWrite(Broche_relay, LOW);  // Initialise le relai à 0

  Wire.begin();                     // initialise l'I2C
  lcd.begin(nbr_col,nbr_lgn);       // initialisation de l'affichuer LCD
  lcd.setRGB(couleur_rouge, couleur_verte, couleur_bleue);
  lcd.clear();
}
/*
void Envoie_XBEE(char *commande, char *valeur)
{
 xbee.write(commande);  
  xbee.write(valeur); 
  Serial.print("Envoi: ");
  Serial.print(commande);
  Serial.println(valeur);
  switch (commande) {
    case 'T':
          if (valeur == '1')   
            { digitalWrite(Broche_relay, HIGH); }
          else   
            { digitalWrite(Broche_relay, LOW);  }
          break;
    default:
      break;
  }
}


void Lecture_Boutton_Test()
// Comme c'est un bouton poussoir et qu'on ne veut introduire aucun delai inutile on memorise 
// l'etat précédent du bouton et on ne s'interesse qu'au chngement d'état
{
  btn_Poussoir_ETAT = digitalRead (Broche_btn_Test);  //lecture de l'état du bouton//
//  lcd.setCursor(0, 1);

  if(btn_Poussoir_ETAT_Prec!=btn_Poussoir_ETAT){
    if (btn_Poussoir_ETAT == HIGH) { //si on appuye sur le bouton
        if (etat_TEST == true){
          etat_TEST = false;
          Envoie_XBEE("T","0");
          }
        else { 
          etat_TEST =  true;
          Envoie_XBEE("T","1");
        }  
        btn_Poussoir_ETAT_Prec = btn_Poussoir_ETAT;
    }
    else {                  
      btn_Poussoir_ETAT_Prec = btn_Poussoir_ETAT;
    }
  }

}
*/
//-------------------------------------------------------------------------
// Ensemble de procedures pour ne raffraichir qu'une partie du LCD
//-------------------------------------------------------------------------
void LCD_Qualite(char *Qualite)
{ lcd.setCursor(colonne_qualite, ligne_qualite); lcd.write("Q:" );
  lcd.setCursor(colonne_qualite+2, ligne_qualite); lcd.write(Qualite);}

void LCD_Turbidite(char *Turbidite)
{ lcd.setCursor(colonne_turbidite, ligne_turbidite); lcd.write("T:" );
  lcd.setCursor(colonne_turbidite+2, ligne_turbidite); lcd.write(Turbidite);}

void LCD_Niveau(char *Niveau)
{ lcd.setCursor(colonne_niveau, ligne_niveau); lcd.write("N:" );
  lcd.setCursor(colonne_niveau+2, ligne_niveau); lcd.write(Niveau);}

void LCD_Pompe(const char *Pompe)
{ lcd.setCursor(colonne_pompe, ligne_pompe); lcd.write("P" );
  lcd.setCursor(colonne_pompe+1, ligne_pompe); lcd.write(Pompe);}

void LCD_Valve(const char *Valve)
{  lcd.setCursor(colonne_valve, ligne_valve); lcd.write("V" );
  lcd.setCursor(colonne_valve+1, ligne_valve); lcd.write(Valve);}

void LCD_Efface()
{ lcd.clear();}

void LCD_AfficheTout( char *Qualite,  char *Turbidite,  char *Niveau,  char *Pompe, char *Valve)
{
  LCD_Qualite(Qualite);
  LCD_Turbidite(Turbidite);
  LCD_Niveau(Niveau);
  LCD_Pompe(Pompe);
  LCD_Valve(Valve);
}
//-------------------------------------------------------------------------

void Receptionne_XBEE()
{
 // caract_lu=xbee.read(); // effacer le //, apres remise en etat d'xbee
  
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
void loop()
{

  Receptionne_XBEE();
  LCD_AfficheTout("0.4","0.5","0.6", ON, OUVERTE);
 // Lecture_Boutton_Test();
}
