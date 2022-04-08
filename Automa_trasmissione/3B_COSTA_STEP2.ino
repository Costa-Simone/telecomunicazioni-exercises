#define TxPin 13

//variabili di stato
int Sp = 0;
int Sf = 0;

//variabili di appoggio
int i = 0;
int S = 2;
int N =  8;
int Tb = 50;
char P = 'N';
bool dato = true;
int bit = 0;
int Car = 'A';
bool parita = false;
unsigned long Tempo = 0; //memorizzare il tempo
  
void setup() {
  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
  Sp = 0;
}

void loop() {
  trasmetti(P);
}

void trasmetti(char P) {
  if(Tempo == 0 || millis() - Tempo >= Tb) {
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
