#define TxPin 13

const int S = 2;
const int N =  8;
const int Tb = 50;
const char P = 'N';

//variabili di stato
int Sf = 0;

//variabili di appoggio
int i = 0;
bool bit = false;
int Car = 'A';
bool parita = false;
unsigned long Tempo = 0; //memorizzare il tempo
  
void setup() {
  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
}

void loop() {
  trasmetti(P);
}

void trasmetti(char P) {
  static int Sp = 0;
  if(Tempo == 0 || millis() > Tb + Tempo) {
    switch(Sp) {
    case 0:
      Tempo = 0;
      digitalWrite(TxPin, HIGH);
      Sf = 1;
      parita = false;
      break;
     
     case 1:
      Tempo = millis();
      i = 0;
      digitalWrite(TxPin, LOW);
      Sf = 2;
      break;

     case 2:
      Tempo += Tb;
      bit = ((Car &(1<<i)) != 0);
      digitalWrite(TxPin, bit);
      parita ^= bit;
      i++;
      if(i == N && P == 'N') {
        Sf = 4;
        i = 0;
      }
      else if(i == N && P != 'N') {
        Sf = 3;
        i = 0;
      }
      break;

     case 3:
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

     case 4:
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
