#define CW 2 //  Ci 2 modi per definire il pin che vuoi usare
#define CCW 3 // Uno è usare il #define, una direttiva al preprocessore che non dichiara alcuna variabile 

int switchPin = 5;         // L'altra è dichiarare una variabile int 
int ledRed = 13;       // in entrambi i casi si assegna il numero del pin che si vuole utilizzare. 
int ledGreen = 12;      // per esempio in questo caso sul pin 12 e 13 dello schedino erano collegati i 2 LED
int sensor = 10;        // sullo schedino ci sono 13 pin a cui puoi collegare quello che ti pare, cerca su internet gli scheme dei singoli componenti per vedere in che modo fare il cablaggio
      
int reading_switch;      // inoltre PRIMA del setup e del loop puoi dichiarare delle variabili di appoggio che userai nel corso del programma
int reading_sensor;     // ad esempio in questo caso ho usato 2 variabili per leggere lo stato del toggle switch e del sensore di fine corsa(che poi non abbiamo usato)
int giu = 1;          // la variabile giu la utilizzo al posto di reading_sensor per capire se il ponte è alzato o abbassato visto che non funzionava il sensore, a differenza di reading_sensor
                      //questa variabile va messa a 0 o 1 manualmente. 1 = abbassato 0 = alzato

void setup()        //in setup() vanno inserite le funzioni che specificano se i pin che vuoi utilizzare sono per INPUT o per OUTPUT, sicuramente ci sono altri setup disponibili, ma io non li conosco
{
  pinMode(switchPin, INPUT); //pinMode(n,"INPUT") prende in ingresso il numero del pin e dice ad arduino che sarà utilizzato come un pin per ricevere un segnale in ingresso dalla periferica alla scheda
  pinMode(ledRed, OUTPUT);  //pinMode(n,"OUTPUT")  prende in ingresso il numero del pin e dice ad arduino che sarà utilizzato per inviare un segnale in output dalla scheda alla periferica
  pinMode(ledGreen, OUTPUT); // ogni pin che hai intenzione di usare va inizializzato altrimenti arduino non saprà come comportarsi.
  pinMode(CW, OUTPUT);  // CW e CCW sono clockwise e counterclockwise, le direzioni in cui deve girare il motore DC
  pinMode(CCW, OUTPUT); // Ovviamente siccome il motore va azionato dalla scheda, sono variabili output come anche i led (sono tutti attuatori)
  pinMode(sensor, INPUT); // gli switch e i sensori invece che invece inviano informazioni alla scheda sono variabili di input.
}



void loop() // questo è il loop principale, arduino esegue a ripetizione tutto quello che trova scritto in questo ciclo senza mai interrompersi finché non togli l'alimentazione o dai un comando di exit
{
  
  reading_switch = digitalRead(switchPin); // digitalRead fa una lettura dello stato del pin che gli dai in ingresso (leggiamo lo stato dell'interruttore in questo caso.)
  Serial.println(reading_switch); // scrive sul monitor seriale che arduino mette a disposizione. Per aprire il monitor seriale vai in strumenti (in questo caso abbiamo scritto lo stato dell'interruttore)
  
   while(reading_switch==0 && giu == 1){ // ciclo while che si ripete fino a che il ponte e basso e l'interruttore è nello stato 0.
  reading_switch = digitalRead(switchPin); // questo ciclo non fa nulla a parte leggere lo stato dell'interruttore ad ogni iterazione, aspettare 0.2 secondi e accendere il led verde.
  delay(200);                             // delay(x) mette il processore in attesa attiva per x ms.
  digitalWrite(ledGreen, HIGH);          // scrive sul pin che gli dai in ingresso, in questo caso HIGH indica acceso e LOW indica spento per i led.
  }
  
   if(reading_switch == 1 && giu == 1){ // quando l'interruttore viene cambiato il programma esce dal ciclo precedente e se il ponte è basso entra in questo blocco condizionato
    digitalWrite(ledGreen, LOW);        // spegne il led verde
    digitalWrite(ledRed, HIGH);         //accende quello rosso
    delay(50);                          //aspetta 0.05 secondi
    digitalWrite(CW,HIGH); // accende il motore nel senso orario 
    delay(4000);            //per 4 secondi
    digitalWrite(CW, LOW); //spegne il motore
    giu = 0;                // indica che il ponte è alzato
  }

  if(reading_switch == 0 && giu == 0){    //stesso blocco di prima ma si attiva quando il ponte è alzato e l'interruttore manda uno zero
    digitalWrite(ledRed, HIGH); // accende il rosso
    delay(50);
    digitalWrite(CCW,HIGH); // fa partire il motore in senso antiorario
    delay(3600);             // per 3.6 secondi
    digitalWrite(CCW, LOW);  // spegne il motore
    giu = 1;                  // dice che il ponte è abbassato
    digitalWrite(ledGreen, HIGH); // accende il led verde
    digitalWrite(ledRed, LOW);    // spegne quello rosso
  }
      
 while(reading_switch==1 && giu == 0){ // stessa cosa del primo ciclo while, non fa nulla a parte leggere lo switch e accendere il led rosso, questo nel caso in cui l'interruttore sia su 1 e il ponte sia alzato
  reading_switch = digitalRead(switchPin);
  delay(200);
  digitalWrite(ledRed, HIGH);
  
  }
// il codice andrebbe sistemato, non è necessario accendere il led ad ogni iterazione dei cicli ad esempio ma non avevo tempo, poi nell'ultimo ciclo se togli la digital write non cambia nulla visto che il led sarebbe già acceso.
// in generale non si dovrebbe mettere il processore in attesa attiva, ma non avevo tempo di scrivere le interrupt e non so come funzionano su arduino.
// 4 secondi e 3.6 li abbiamo calcolati facendo le prove, in caso usassimo i fine corsa non sarebbe necessario usare quel delay ma solamente leggere la variabile del fine corsa per vedere quando è 0 o 1
// nota che il fine corsa funziona con un condensatore che ci mette un po' a scaricarsi e passare da 1 a 0, quindi il tempo di reazione del programma per fermare il motore può variare a seconda della capacità
// e della costante di tempo del fine corsa. bisognerebbe fare dei test per capire dove posizionarlo sul ponte per avere una buona resa.
}
