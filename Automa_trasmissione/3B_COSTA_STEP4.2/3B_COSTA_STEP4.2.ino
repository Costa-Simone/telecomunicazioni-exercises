//Pin utilizzato
#define TxPin 13

//Variabili di Trama
const int S = 2;
const int N =  8;
const int Tb = 50;
const char P = 'N';
const int attesa = 2000;

bool Tx(char mex[]);

//Variabili d'Appoggio
int i = 0;
int j = 0;
bool bit = false;
char mex[] = { 'C', 'O', 'S', 'T', 'A', '\0'}; // Stringa da trasmettere
bool parita = false;
unsigned long Tempo = 0; //Variabile per memorizzare il tempo
bool impegno = true;

void setup() {
  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
}

void loop() {
  //TRASMISSIONE
  if(impegno == true) {
      impegno = Tx(mex);
  }
  else {
      //Tempo += attesa;
      impegno = true;
  }
}

bool Tx(char mex[]) {
  //Variabili di Stato
  static int Sp = 0;
  static int Sf = 0;
  
  static char datoCatturato;
  
  if(Tempo == 0 || millis() > Tempo + Tb) { //Intercettore che ha validita'  di 50 giorni
    switch(Sp) {
    case 0: // idle
      digitalWrite(TxPin, HIGH);
      Sf = 1;
      parita = false;
      impegno = false;
      break;
     
     case 1: // start
      Tempo = millis();
      i = 0;
      digitalWrite(TxPin, LOW);
      Sf = 2;
      datoCatturato = mex[j];
      impegno = true;
      break;

     case 2: // trasmissione
      Tempo += Tb;
      bit = ((datoCatturato & (1<<i)) != 0); //Estrazione bit dal byte
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
      impegno = true;
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
      impegno = true;
      break;

     case 4: // stop
      Tempo += Tb;
      digitalWrite(TxPin, HIGH);
      i++;

      if(i == S) {
        Sf = 0;
        j++;

        if(datoCatturato == '\0') {
          j = 0;
        }
      }
      impegno = true;
      break;
   }
   Sp = Sf;
  }
  return impegno;
}
