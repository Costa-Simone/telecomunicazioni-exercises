//Pin utilizzato
#define TxPin 13

//Variabili di Trama
const int S = 2;
const int N =  8;
const int Tb = 50;
const char P = 'N';

//variabili di appoggio
int i = 0;
bool bit = false;
int Car = 'A';
bool parita = false;
unsigned long Tempo = 0; //Variabile per memorizzare il tempo
  
void setup() {
  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
}

void loop() {
  //TRASMISSIONE
  trasmetti(P);
}

void trasmetti(char P) {
  //Variabili di Stato
  static int Sp = 0;
  static int Sf = 0;
  
  static char C = Car;
  
  if(Tempo == 0 || millis() > Tb + Tempo) { //Intercettore che ha validita'  di 50 giorni
    switch(Sp) {
    case 0: // idle
      Tempo = 0;
      digitalWrite(TxPin, HIGH);
      Sf = 1;
      parita = false;
      break;
     
     case 1: // start
      Tempo = millis();
      i = 0;
      digitalWrite(TxPin, LOW);
      Sf = 2;
      break;

     case 2: // trasmissione
      Tempo += Tb;
      bit = ((C & (1<<i)) != 0); //Estrazione bit dal byte
      digitalWrite(TxPin, bit);
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
      Tempo += Tb;
      if(P == 'E') {
        digitalWrite(TxPin, parita);
      }
      else if(P == 'O') {
        parita = !parita;
        digitalWrite(TxPin, parita);
      }
      Sf = 4;
      break;

     case 4: // stop
      Tempo += Tb;
      digitalWrite(TxPin, HIGH);
      i++;

      if(i == S) {
        Sf = 0;
      }
      break;
   }
   Sp = Sf;
  }
}
