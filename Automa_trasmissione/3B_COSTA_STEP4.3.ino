#define TxPin 13

const int S = 2;
const int N =  8;
const int Tb = 50;
const char P = 'N';
const int attesa = 2000;

bool Tx();

//variabili di appoggio
int i = 0;
int j = 0;
bool bit = false;
char mex[] = { 'C', 'O', 'S', 'T', 'A', '\n'};
bool parita = false;
unsigned long Tempo = 0; //memorizzare il tempo
bool impegno = false;

void setup() {
  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
}

void loop() {
  if(Tx() == false) {
      Tempo += attesa;
      impegno = true;
  }
}

bool Tx() {
  static int Sp = 0;
  static int Sf = 0;
  static char datoCatturato;
  if(Tempo == 0 || millis() > Tempo + Tb) {
    switch(Sp) {
    case 0:
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
     
     case 1:
      Tempo = millis();
      i = 0;
      digitalWrite(TxPin, LOW);
      Sf = 2;
      datoCatturato = mex[j];
      impegno = true;
      break;

     case 2:
      Tempo += Tb;
      bit = ((datoCatturato & (1<<i)) != 0);
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
      impegno = true;
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
      impegno = true;
      break;

     case 4:
      Tempo += Tb;
      digitalWrite(TxPin, HIGH);
      i++;

      if(i == S) {
        Sf = 5;
        j++;
      }
      impegno = true; 
      break;

     case 5:
      Sf = 0;
      Tempo += Tb;
      
      if(datoCatturato == '\n') {
          j = 0;
      }
      impegno = true;
      break;
   }
   Sp = Sf;
  }
  return impegno;
}
