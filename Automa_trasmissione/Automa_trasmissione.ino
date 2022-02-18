
int Sp = 0;
int Sf = 0;
int dato = 0;
int i = 0;
char p = 'e';
int N = 0;
int S = 2;
int b = 0;
  
void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

}

void loop() {
  txtByte(dato);
   delay(1000);
}

void txtByte(char dato) {

  switch(Sf) {
    case 0:
      if(dato == 1) {
        Sf = 1;
      }
      
      break;

     case 1:
      Sf = 2;
      
      break;

     case 2:
      if(i < N) {
        i++;
      }
      else if(i == N && P != 'N') {
        Sf = 3;
      }
      else if(i == N && P == 'N') {
        Sf = 4;
      }
      
      break;

      case 3:
        Sf = 4;
        
        break;

       case 4:
        if(i < S) {
          i++;
        }
        else if(i == S) {
          Sf = 0;
        }
        
        break;
  }
}
