//Zadeklarowanie dwóch stałych odpowiadającym numerom wejść
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int stepPin = 2; 
const int dirPin = 5; 
const int servoPin = 11;
 const int enable = 8; 
 int pos=0;
 int przelicznik=0;
  int tab[4]={1,5,10,100};
  
  const int przyciskWPrawo=0;
 const int przyciskWLewo=9;
 const int przyciskStart=12;
 const int przyciskPrzelicznik=10;

 int dlugoscGlobalna=10;
 Servo servo;
 LiquidCrystal_I2C lcd(0x27,2,16);

void setup() 
{
  // Ustawienie dwóch PINów jako wyjścia
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode (enable,OUTPUT);
  pinMode(przyciskWPrawo,INPUT);
  pinMode(przyciskWLewo,INPUT);
  pinMode(przyciskStart,INPUT);
  pinMode(przyciskPrzelicznik,INPUT);
  digitalWrite (enable,LOW);
  digitalWrite(dirPin,LOW);
  servo.attach(servoPin);
  servo.write(0);
  lcd.begin(2,16);
    lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void tnij()
{
  delay(1000);
  servo.write(180);
  delay(1000);
  servo.write(0);
  delay(1000);
}

void natnijIzolacje()
{
  delay(1000);
  servo.write(125);
  delay(1000);
  servo.write(0);
  delay(1000);
}

void przesun(int dlugosc, bool kierunek=0)
{
  int iloscKrokow=dlugosc/0.1125;
  
  digitalWrite (dirPin, kierunek);
    for(int i=0;i<iloscKrokow;i++)
    {
      digitalWrite(stepPin,LOW);
      delay(10);
      digitalWrite(stepPin,HIGH);
      delay(10);
    }
}

void wyswietlDlugosc(int dlugosc)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Wybrana dlugosc");
  lcd.setCursor(0,1);
  lcd.print(dlugosc);
  lcd.print("mm");
  lcd.print("   +/-");
  lcd.print(tab[przelicznik]);
}

void program(int dlugosc)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TRWA PRZYCINANIE");
  int naciecieIzolacji=3;
  int cofniecie=1;
  tnij();
  przesun(naciecieIzolacji);
  natnijIzolacje();
  przesun(dlugosc-2*naciecieIzolacji);
  natnijIzolacje();
  przesun(naciecieIzolacji);
  tnij();
  przesun(cofniecie,1);
  przesun(cofniecie,0);
  delay(1000);
}

void inkrementujDlugosc()
{
  if(dlugoscGlobalna+tab[przelicznik]<=9999)
  {
  dlugoscGlobalna+=tab[przelicznik];
  }
}

void dekrementujDlugosc()
{
  if(dlugoscGlobalna-tab[przelicznik]>=10)
  {
    dlugoscGlobalna-=tab[przelicznik];
  }
}

void inkrementujPrzelicznik()
{
    if(przelicznik<3)
    {
      przelicznik++;
    }
    else
    {
      przelicznik=0;
    }
}

void loop(){
if(digitalRead(przyciskWLewo)==0)
{
  dekrementujDlugosc();
  delay(100);
}
if(digitalRead(przyciskWPrawo)==0)
{
  inkrementujDlugosc();
  delay(100);
} 
if(digitalRead(przyciskStart)==0)
{
  program(dlugoscGlobalna);
  delay(1000);
}
if(digitalRead(przyciskPrzelicznik)==0)
{
  inkrementujPrzelicznik();
  Serial.println("zmieniono");
  delay(500);
}
 wyswietlDlugosc(dlugoscGlobalna);
}
