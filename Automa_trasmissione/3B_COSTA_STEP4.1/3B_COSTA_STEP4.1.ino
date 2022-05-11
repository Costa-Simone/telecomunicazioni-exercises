//Pin utilizzato
#define pin 12

//Variabili di Trama
#define s 2
#define N 8
#define P 'E'
#define Tb 50
#define attesa 2000

//Variabili d'Appoggio
bool bit = false;
char car = 'A';
int i = 0;
bool parita = false;
unsigned long Tempo = 0;  //Variabile per memorizzare il tempo
bool impegno = true;

void setup() {
	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH); //Inizio IDLE
}

void loop() {
  //TRASMISSIONE
	if(impegno == true)
	{
		impegno = Tx(car);
	}
	else if(millis() > (attesa + Tb))
	{
		Tempo += attesa;
		impegno = true;
	}
}

bool Tx(char car){
	//Variabili di Stato
	static int statoPresente = 0;
	static int statoFuturo = 0;
	static char carAus;

	if(Tempo == 0 || millis() > Tempo + Tb)  //Intercettore che ha validit�  di 50 giorni
	{
		switch(statoPresente)
		{
			case 0: // idle
				digitalWrite(pin, HIGH);
				carAus = car;
				parita = false;
				statoFuturo = 1;
				impegno = false;
				break;

			case 1: // idle
				Tempo = millis();
				digitalWrite(pin, LOW);
				statoFuturo=2;
				i=0;
				impegno = true;
				break;

			case 2: // trasmissione
				Tempo += Tb;
				bit = ((carAus & (1<<i))!= 0);  //Estrazione bit dal byte
				digitalWrite(pin, bit);
				parita ^= bit;
				i++;
				if(i == N)
				{
					i = 0;
					if(P == 'N')
					{
						statoFuturo = 4; //No bit di parità
					}
					else
					{
						statoFuturo = 3;
					}
				}
				impegno = true;
				break;

			case 3: // parita
				Tempo += Tb;
				if(P == 'O')
				{
					parita = !parita;
				}
				digitalWrite(pin,parita);
				statoFuturo = 4;
				impegno = true;
				break;

			case 4: // stop
				Tempo += Tb;
				digitalWrite(pin, HIGH);
				i++;
				if(i == s)
				{
					statoFuturo = 5;
				}
				impegno = true;
				break;
				
			case 5:
        statoFuturo = 0;
        break;
		}
		statoPresente = statoFuturo;
	}
	return impegno;
}
