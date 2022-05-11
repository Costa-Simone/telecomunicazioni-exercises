//Pin utilizzato
#define Pin_Tx 3  //Output scelto da Arduino
#define Pin_Rx 2  //Input scelto da Arduino

//Variabili di Trama
#define S 2
#define N 8
#define P 'E'
#define Tb 50
#define attesa 200

//Variabili d'Appoggio
char car = 'A';
unsigned long Tempo = 0;  //Variabile per memorizzare il tempo
bool impegno = true; //variabile per verificare se automa sta trasmettendo
char dato1 = '\0';

void setup() {
  pinMode(Pin_Tx, OUTPUT);
  pinMode(Pin_Rx, INPUT);
  digitalWrite(Pin_Tx, HIGH); //Inizio IDLE
  Serial.begin(9600);
}

bool txByte(char car);  //Prototipo Tx
char rxByte();  //Prototipo Rx

void loop() {
  //TRASMISSIONE
  if (impegno == true)
  {
    impegno = txByte(car);
  }
  else if (millis() > (attesa + Tb))
  {
    Tempo += attesa;
    impegno = true;
  }

  //RICEZIONE
  if (rxByte() != '\0') {
    //Serial.print();
  }
}

bool txByte(char car) {
  //Variabili di Stato
  static int statoPresente = 0;
  static int statoFuturo = 0;
  
  static char carAus;
  static bool bit = false;
  static int i = 0;
  static bool parita;

  if (Tempo == 0 || millis() > Tempo + Tb) { //Intercettore che ha validita'  di 50 giorni
    switch (statoPresente) {
      case 0: // idle
        digitalWrite(Pin_Tx, HIGH);
        
        carAus = car;
        parita = false;
        statoFuturo = 1;
        impegno = false;
        break;

      case 1:
        Tempo = millis(); //porta tempo al ms attuale
        statoFuturo = 2;
        i = 0;
        impegno = true;
        
        digitalWrite(Pin_Tx, LOW);
        break;

      case 2:
        Tempo += Tb;
        bit = ((carAus & (1 << i)) != 0); //Estrazione bit dal byte
        parita ^= bit;  //Aggiorno la parità ogni volta
        i++;
        
        digitalWrite(Pin_Tx, bit); //stampa del bit
        
        if (i == N) {
          i = 0; //azzeramento variabile posizione bit
          
          if (P == 'N') {
            statoFuturo = 4;  //No bit di parità
          }
          else {
            statoFuturo = 3;
          }
        }
        
        impegno = true;
        break;

      case 3: // parita
        Tempo += Tb;
        
        if (P == 'O')
        {
          parita = !parita;
        }
        
        digitalWrite(Pin_Tx, parita);
        
        statoFuturo = 4;
        impegno = true;
        break;

      case 4: // stop
        Tempo += Tb;
        i++;
        
        digitalWrite(Pin_Tx, HIGH);
        
        if (i == S)
        {
          statoFuturo = 5;
        }
        
        impegno = true;
        break;

      case 5: //azzeramento
        statoFuturo = 0;
        break;
    }
    
    statoPresente = statoFuturo;
  }
  
  return impegno;
}

char rxByte() {
  //Variabili di Stato
  static int statoPresente_Rx = 0;
  static int statoFuturo_Rx = 0;

  static bool bit = false;
  static bool bitPrecedente = false;
  static int k = 0;
  static bool parita_Rx;
  static char datoCat = 0;

  bool ricevuto = false;

  bit = digitalRead(Pin_Rx); //prendo valore del bit

  switch (statoPresente_Rx) {
    case 0:
      if ((bitPrecedente) && (!bit)) {
        statoFuturo_Rx = 1;
      }
      
      break;

    case 1:
      if (bit) { //Se ho avuto un disturbo, rumore
        statoFuturo_Rx = 0;
      }
      else {
        statoFuturo_Rx = 2;
        k = 0;
        parita_Rx = false;
      }

      break;

    case 2:
      datoCat = ((bit) ? 1 : 0) << k; //ricezione del bit
      parita_Rx ^= bit; //Aggiorno la parità ogni volta
      k++;
      
      if (k == N) {
        k = 0;
      
        if (P == 'N') {
            statoFuturo_Rx = 4; //No bit di parita'
          }
          else {
            statoFuturo_Rx = 3;
          }
      }
      
      break;

    case 3:
      if (P == 'O') {
        parita_Rx = !parita_Rx;
      }

      if (parita_Rx != digitalRead(Pin_Rx)) { //confronto tra le parita'
        datoCat = '\0';
      }

      statoFuturo_Rx = 4;
      break;

    case 4:
      if (!bit) {
        datoCat = '\0';
        k++;
      }

      if(k == S) {
        statoFuturo_Rx = 0;
        ricevuto = true; //carattere ricevuto
      }

      break;
  }
  
  statoPresente_Rx = statoFuturo_Rx;  //Aggiornamento stato
  bitPrecedente = bit;  //Aggiornamento bit

  if(ricevuto) {
    dato1 = datoCat;
  }
  
  return dato1;
}
