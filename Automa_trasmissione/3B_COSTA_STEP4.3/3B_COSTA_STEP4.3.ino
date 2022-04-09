#define TxPin 13

const int S = 2;
const int N =  8;
const int Tb = 50;
const char P = 'N';
const int attesa = 2000;

bool Tx();

//variabili di appoggio
bool bit = false;
char mex[] = { 'C', 'O', 'S', 'T', 'A', '\n'};
bool parita = false;
unsigned long Tempo = 0; //memorizzare il tempo
bool impegno = true; //variabile per verificare se automa sta trasmettendo

void setup() {
  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
}

void loop() {
  if(Tx() == false && Tempo != 0) {
      Tempo += attesa;
      impegno = true;
  }
}

bool Tx() {
  static int Sp = 0;
  static int Sf = 0;
  static char datoCatturato;
  static int i = 0, j = 0;
  
  if(Tempo == 0 || millis() > Tempo + Tb) {
    switch(Sp) {
    case 0: //idle
      digitalWrite(TxPin, HIGH);
      Sf = 1;
      parita = false;

      if(j == 0) {
        impegno = false;
      }
      else {
        impegno = true;
      }
      
      break;
     
     case 1: //start
      Tempo = millis(); //porta tempo al ms attuale
      i = 0;
      digitalWrite(TxPin, LOW);
      Sf = 2;
      datoCatturato = mex[j]; //cattura carattere da trasmettere
      impegno = true;
      break;

     case 2: //trasmissione
      Tempo += Tb;
      bit = ((datoCatturato & (1<<i)) != 0); //trasmissione del carattere attuale
      digitalWrite(TxPin, bit); //stampa del bit
      parita ^= bit;
      i++;
      
      if(i == N && P == 'N') {
        Sf = 4;
        i = 0; //azzeramento variabile posizione bit
      }
      else if(i == N && P != 'N') {
        Sf = 3;
        i = 0;
      }
      
      impegno = true;
      break;

     case 3: //parita
      Tempo += Tb;
      
      if(P == 'E') {
        digitalWrite(TxPin, parita);
      }
      else if(P == 'O') {
        parita = !parita;
        digitalWrite(TxPin, parita);
      }
      
      Sf = 4;
      impegno = true;
      break;

     case 4: //stop
      Tempo += Tb;
      digitalWrite(TxPin, HIGH);
      i++;

      if(i == S) {
        Sf = 5;
        j++;
      }
      
      impegno = true; 
      break;

     case 5: //azzeramento
      Sf = 0;
      Tempo += Tb;
      
      if(datoCatturato == '\n') {
          j = 0; //azzeramento variabile posizione stringa
      }
      
      impegno = true;
      break;
   }
   
   Sp = Sf;
  }
  
  return impegno;
}
