//Pin utilizzato
#define TxPin 13

// Variabili di Trama
int S = 2;
int N =  8;
char P = 'N';
int Tb = 50;

//variabili di stato
int Sp = 0;
int Sf = 0;

// Variabili d'Appoggio
int i = 0;
bool dato = true;
int bit = 0;
int Car = 'A';
bool parita = false;
  
void setup() {
  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
}

void loop() {
  //TRASMISSIONE
  trasmetti();
}

void trasmetti() {
    while(dato) {
    switch(Sp) {
    case 0: // idle
      digitalWrite(TxPin, HIGH);
      delay(100);
      Sf = 1;
      break;
     
     case 1: // start
      i = 0;
      digitalWrite(TxPin, LOW);
      delay(Tb);
      Sf = 2;
      break;

     case 2: // trasmissione
      bit = ((Car &(1<<i)) != 0); //Estrazione bit dal byte
      digitalWrite(TxPin, bit);
      delay(Tb);
      parita ^= bit; //Aggiorno la parità ogni volta
      i++;
      if(i == N && P == 'N') {
        Sf = 4; //No bit di parità
        i = 0;
      }
      else if(i == N && P != 'N') {
        Sf = 3;
        i = 0;
      }
      break;

     case 3: // parita
      if(P == 'E') {
        digitalWrite(TxPin, parita);
        delay(Tb);
      }
      else if(P == 'O') {
        parita = !parita;
        digitalWrite(TxPin, parita);
        delay(Tb);
      }
      Sf = 4;
      break;

     case 4: // stop
      digitalWrite(TxPin, HIGH);
      if(i < S) {
        i++;
        delay(Tb);
      }
      else if(i == S) {
        Sf = 0;
      }
      delay(Tb);
     
      break;
   }

   Sp = Sf;
  }
}
