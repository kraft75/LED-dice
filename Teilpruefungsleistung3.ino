#include <LiquidCrystal_I2C.h>

/* LCD einrichten */
// Deklaration eines LiquidCrystal_I2C-Objekts : I2C Adresse 0x27, 16 Spalten and 2 Reihen
LiquidCrystal_I2C lcd(0x27, 16, 2); 

/* Variablen und Kostanten für loop() */
int i; // Zählvariable zur De-Aktivierung der LEDs
const int BUTTON = 8; // Taster-Belegung
const int PINS[] = {2, 3, 4, 5, 6, 7}; // Pin-Belegung als Array

/* Variablen für die Entprellung des Tasters */
int aktuellerButtonZustand;
int letzterButtonZustand = LOW; // Letzte Ablesung des Button-Status
unsigned long letzteEntprellung = 0; // Letzte Umschaltung des Ausgangspins
unsigned long entprellZeit = 50;

/* Initialisierung der Pins */
void setup() {
  // LCD initialisieren
  lcd.init();
  // Hintergrundlicht einschalten
  lcd.backlight();

  // LEDs als Ausgänge
  for(i = 0; i < 6; ++i) {
    pinMode(PINS[i], OUTPUT); 
  }
  // Taster als Eingang mit Pull-Up Widerstand
  pinMode(BUTTON, INPUT_PULLUP);
}

/* Hauptprogramm */
void loop() {
  // aktuellen Zustand des Schalters zwischenspeichern
  int cache = digitalRead(BUTTON);

  // Änderung des Status vom Schalter, aufgrund eines Knopfdruckes oder Rauschen.
  if(cache != letzterButtonZustand) {
    // Timer aktualisieren
    letzteEntprellung = millis();
  }

  // Falls die aktuelle Ablesung größer als die Entprellzeit ist
  if((millis() - letzteEntprellung) - entprellZeit) {
    // Falls der Cache-Wert nicht mit dem aktuellen Buttonzustand übereinstimmt
    if(cache != aktuellerButtonZustand) {
      // wird der Wert des Zwischenspeichers dem aktuellen Schalter-Status zugewiesen
      aktuellerButtonZustand = cache;
      // Ist der aktuelle Schalter-Status auf HIGH
      if(aktuellerButtonZustand == HIGH) {
        // wird die Würfelfunktion ausgelöst
        anzeige();
        // werden alle LEDs auf LOW resetet
        reset();
      }
    }
  }

  // Speicherung vom letzten Zustand des 'Buttons
  letzterButtonZustand = cache;
}

/* LEDs gemäß dem der Augenzahl aktivieren */
void anzeige() {
  // Display löschen
  lcd.clear();
  
  // Zunächst LEDs ausschalten
  for(i = 0; i < 6; ++i) {
    digitalWrite(PINS[i], LOW);
  } 

  // Zufallsgenerator erzeugt Würfelzahl
  int auge = random(1, 7);
  
  // LEDS der entsprechenden Augenzahl aktivieren
  for(i = 0; i < auge; ++i) {
    digitalWrite(PINS[i], HIGH);
  }

  // Cursor-Anfang in Zeile 1 und Spalte 0
  lcd.setCursor(0, 0);
  // Ergebnis-Anzeige im Display
  lcd.print(String("Wurf: ") + String(auge));
  delay(1000);
  lcd.clear();

}

/* LEDs wieder zurücksetzen */
// ALLe LEDs deaktivieren
void reset() {
  for(i = 0; i < 7; ++i) {
    digitalWrite(PINS[i], LOW);
  }
}


