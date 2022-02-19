#define TxPin 13

int Sp = 0;
int Sf = 0;
int i = 0;
int S = 2;
int N =  8;
int Tb = 1000;
char P = 'N';
bool dato = true;
bool bit = true;
char Car = 'A';
int C = 65;
int somma = 0;
int numero[8];
  
void setup() {
  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
}

void loop() {
   switch(Sp) {
    case 0:
      digitalWrite(TxPin, HIGH);
      
      if(dato) {
        Sf = 1;
      }
     else {
      Sf = 0; 
     }

      delay(Tb);
       delay(1000);
      
      break;
     
     case 1:
      Sf = 2;
      i = 0;

      digitalWrite(TxPin, LOW);
      
      delay(Tb);
      
      break;

     case 2:
       if(i < N) {
        if(C % 2 == 0) { 
          bit = false;
          numero[i] = 0;

          digitalWrite(TxPin, LOW);
          
          C = C / 2;
        }
        else {
          bit = true;
          numero[i] = 1;

          digitalWrite(TxPin, HIGH);

          C = (C - 1) / 2;
        }

        i++;
         
        delay(100);
      }
      else if(i == N && P == 'N') {
        Sf = 4;
        i = 0;
      }
      else if(i == N && P != 'N') {
        Sf = 3;
      }
      
      break;

     case 3:
      somma = numero[0] ^ numero[1];
     
      for(int j = 2; j < N - 1; j++) {
         somma ^= numero[j];
      }

      if(somma == 1) {
        digitalWrite(TxPin, HIGH);
      }
      else {
        digitalWrite(TxPin, LOW);
      }
     
      Sf = 4;
      
      delay(Tb);
      
      break;

     case 4:
      digitalWrite(TxPin, LOW);
     
      if(i < S) {
        i++;

        delay(Tb);
      }
      else if(i == S) {
        Sf = 0;
        dato = false;
      }

      delay(Tb);
     
      break;
   }

   Sp = Sf;
}
