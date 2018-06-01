#include <Wire.h>
#define DS1307_ADDRESS 0x68
#include <SoftwareSerial.h> //SoftwareSerial Port
#define RxD 10 //Pin 10 pour RX, PB2 survotre board, a brancher sur le TX du HC-06
#define TxD 11 //Pin 11 pour TX, PB3 survotre board, a brancher sur le RX du HC-06
SoftwareSerial BTSerie(RxD,TxD);
byte zero = 0x00; //workaround for issue #527

#include <Adafruit_NeoPixel.h>
#define PIN 6
   int i=0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
  int x;
  int y;
  int z;
  int couleurHeure=150;
  int couleurMinute=250;
  int couleurSeconde=100;
  String c;
  String t;
  
void setup(){
Wire.begin();
Serial.begin(9600);
pinMode(RxD, INPUT);
pinMode(TxD, OUTPUT);
BTSerie.begin(9600);
setDateTime(3,49,35);
strip.begin();
strip.show();
}

void loop(){

  //On lit caractere par caractere sur le BTSerie
if (BTSerie.available()) {
t=String(BTSerie.read()).charAt(0);
  if (String(t)=="t"){
    setTimeBluetooth(String(BTSerie.read()));
  }  
  if (String(t)=="h"){
    couleurHeure=CouleurHMS(String(BTSerie.read()));
  }
  if (String(t)=="m"){
    couleurMinute=CouleurHMS(String(BTSerie.read()));
  }
  if (String(t)=="s"){
    couleurSeconde=CouleurHMS(String(BTSerie.read()));
  }

  }
strip.clear();
Serial.print(DonneHeure());
Serial.print("   ");
Serial.print(DonneMinute());
Serial.print("   ");
Serial.println(DonneSeconde());
EcritHeure(couleurHeure);
EcritMinute(couleurMinute);
EcritSeconde(couleurSeconde);
strip.show();
}

void setDateTime(int setHeure, int setMinute, int setSeconde){

byte second = setSeconde; //0-59
byte minute = setMinute; //0-59
byte hour = setHeure; //0-23
byte weekDay = 1; //1-7
byte monthDay = 1; //1-31
byte month = 1; //1-12
byte year = 0; //0-99

Wire.beginTransmission(DS1307_ADDRESS);
Wire.write(zero);

Wire.write(decToBcd(second));
Wire.write(decToBcd(minute));
Wire.write(decToBcd(hour));
Wire.write(decToBcd(weekDay));
Wire.write(decToBcd(monthDay));
Wire.write(decToBcd(month));
Wire.write(decToBcd(year));

Wire.write(zero); //start

Wire.endTransmission();

}

void setTimeBluetooth(String str){
String h;
String m;
String s;
     h=String(str).charAt(2);
     h+=String(str).charAt(3);
     m=String(str).charAt(5);
     m+=String(str).charAt(6);
     s=String(str).charAt(8);
     s+=String(str).charAt(9);
  setDateTime(h.toInt(),m.toInt(),s.toInt());
}
int CouleurHMS(String str){
    if (String(str).length()==5){
      c=String(str).charAt(2);
    }
    else if (String(str).length()==6){
      c=String(str).charAt(2);
      c+=String(str).charAt(3);
    }
    else{
      c=String(str).charAt(2);
      c+=String(str).charAt(3);
      c+=String(str).charAt(4);

    }
    return (c.toInt());
    }

byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val) {
// Convert binary coded decimal to normal decimal numbers
return ( (val/16*10) + (val%16) );
}
int secAvant=0;
int minuteAvant=0;
int heureAvant=0;

int DonneSeconde(){

// Reset the register pointer
Wire.beginTransmission(DS1307_ADDRESS);
Wire.write(zero);
Wire.endTransmission();

Wire.requestFrom(DS1307_ADDRESS, 7);

int second = bcdToDec(Wire.read());
int minute = bcdToDec(Wire.read());
int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time


  int boucle=0;
  int sec=0;
  int i=0;
  while (boucle==0) {
    if (i==second) {sec=i;boucle=1;}
    i=i+1;
    
  }
  return(sec);
}
void EcritSeconde(int couleur){

  if (couleur < 36){
    x=255;y=255;z=255;}
  else if (couleur > 36 and couleur < 72){
    x=255;y=0;z=255;}
  else if (couleur > 72 and couleur < 108){
    x=255;y=0;z=0;}
  else if (couleur > 108 and couleur < 144){
    x=255;y=255;z=0;}
  else if (couleur > 144 and couleur < 180){
    x=0;y=255;z=0;}
  else if (couleur > 180 and couleur < 216){
    x=0;y=255;z=255;}
  else {
    x=0;y=0;z=255;}
    
  strip.setPixelColor(DonneSeconde(),x, y, z);
  if (DonneSeconde()==0){strip.setPixelColor(59,0, 0, 0);}
}

int DonneHeure(){

// Reset the register pointer
Wire.beginTransmission(DS1307_ADDRESS);
Wire.write(zero);
Wire.endTransmission();

Wire.requestFrom(DS1307_ADDRESS, 7);

int second = bcdToDec(Wire.read());
int minute = bcdToDec(Wire.read());
int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time


  int boucle=0;
  int heure=0;
  int i=0;
  while (boucle==0) {
    if (i==hour) {heure=i;boucle=1;}
    i=i+1;
    
  }
  return(heure);
}
void EcritHeure(int couleur){
  int heur=DonneHeure()*5;
    if (heur>=60){
        heur=DonneHeure()*5-60;}

    
  if (DonneMinute()>=0 && DonneMinute()<12){
    heur=heur;}
  else if (DonneMinute()>=12 && DonneMinute()<24){
    heur=heur+1;}
  else if (DonneMinute()>=24 && DonneMinute()<36){
    heur=heur+2;}
  else if (DonneMinute()>=36 && DonneMinute()<48){
    heur=heur+3;}
  else if (DonneMinute()>=48 && DonneMinute()<=60){
    heur=heur+4;}

  if (couleur < 36){
    x=255;y=255;z=255;}
  else if (couleur > 36 and couleur < 72){
    x=255;y=0;z=255;}
  else if (couleur > 72 and couleur < 108){
    x=255;y=0;z=0;}
  else if (couleur > 108 and couleur < 144){
    x=255;y=255;z=0;}
  else if (couleur > 144 and couleur < 180){
    x=0;y=255;z=0;}
  else if (couleur > 180 and couleur < 216){
    x=0;y=255;z=255;}
  else {
    x=0;y=0;z=255;}
  strip.setPixelColor(heur,x, y, z);
  strip.setPixelColor(heur-1,x, y, z);
  strip.setPixelColor(heur+1,x, y, z);
  if (heur==59){strip.setPixelColor(0,0, 255, 0);}
  if (heur==0){strip.setPixelColor(58,0, 0, 0);strip.setPixelColor(59,0, 255, 0);}
  if (heur==1){strip.setPixelColor(59,0, 0, 0);}

}

int DonneMinute(){

// Reset the register pointer
Wire.beginTransmission(DS1307_ADDRESS);
Wire.write(zero);
Wire.endTransmission();

Wire.requestFrom(DS1307_ADDRESS, 7);

int second = bcdToDec(Wire.read());
int minute = bcdToDec(Wire.read());
int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time


  int boucle=0;
  int minut=0;
  int i=0;
  while (boucle==0) {
    if (i==minute) {minut=i;boucle=1;}
    i=i+1;
    
  }
  return(minut);
}
void EcritMinute(int couleur){
  if (couleur < 36){
    x=255;y=255;z=255;}
  else if (couleur > 36 and couleur < 72){
    x=255;y=0;z=255;}
  else if (couleur > 72 and couleur < 108){
    x=255;y=0;z=0;}
  else if (couleur > 108 and couleur < 144){
    x=255;y=255;z=0;}
  else if (couleur > 144 and couleur < 180){
    x=0;y=255;z=0;}
  else if (couleur > 180 and couleur < 216){
    x=0;y=255;z=255;}
  else {
    x=0;y=0;z=255;}
  strip.setPixelColor(DonneMinute(),x, y, z);
  strip.setPixelColor(DonneMinute()-1,x, y, z);
  if (DonneMinute()==0 or DonneMinute()==60){strip.setPixelColor(59,0, 0, 255);}
}

