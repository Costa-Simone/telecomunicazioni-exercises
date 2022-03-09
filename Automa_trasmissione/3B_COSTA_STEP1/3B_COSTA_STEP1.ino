#define TxPin 13

int Sp = 0;
int Sf = 0;
int i = 0;
int S = 2;
int N =  8;
int Tb = 50;
char P = 'N';
bool dato = true;
int bit = 0;
int Car = 'A';
bool parita = false;
  
void setup() {
  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
}

void loop() {
  trasmetti();
}

void trasmetti() {
    while(dato) {
    switch(Sp) {
    case 0:
      digitalWrite(TxPin, HIGH);
      delay(100);
      Sf = 1;
      break;
     
     case 1:
      i = 0;
      digitalWrite(TxPin, LOW);
      delay(Tb);
      Sf = 2;
      break;

     case 2:
      bit = ((Car &(1<<i)) != 0);
      digitalWrite(TxPin, bit);
      delay(Tb);
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

     case 4:
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
