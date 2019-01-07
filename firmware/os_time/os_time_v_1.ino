//Часы с семисегментным дисплеем и RTC. ПО часов.
//Версия: 1.0 
//Сборка: Неизвестно.
//ПО: Молоткин Константин
// Telegram : kosia_zlo
// Учитель : Митина Е.Н. 
// Сборка : 23.11.2018

#include <SoftwareSerial.h> 
#include <TM74HC595Display.h>
#include <iarduino_RTC.h>  
#include <TimerOne.h>
iarduino_RTC time(RTC_DS1307); 
unsigned char compileTime[] = __TIME__;  
int SCLK = 12; // выводы дисплея на плате
int RCLK = 11; // 
int DIO = 10; //
int c = 0; // переменная часов целая часть
int cday = 0;
int c2 = 0;
int m = 0; // переменная минут целая часть
int m1 = 0;
int d =0 ;
int d1 =0; 
int mont = 0 ;
int mont1 = 0;
TM74HC595Display disp(SCLK, RCLK, DIO); // указываем выводы дисплея 
unsigned char LED_0F[29];

void setup() {
  LED_0F[0] = 0xC0; //0 // библиотека отображения символов
  LED_0F[1] = 0xF9; //1
  LED_0F[2] = 0xA4; //2
  LED_0F[3] = 0xB0; //3
  LED_0F[4] = 0x99; //4
  LED_0F[5] = 0x92; //5
  LED_0F[6] = 0x82; //6
  LED_0F[7] = 0xF8; //7
  LED_0F[8] = 0x80; //8
  LED_0F[9] = 0x90; //9
  LED_0F[10] = 0x88; //A
  LED_0F[11] = 0x83; //b
  LED_0F[12] = 0xC6; //C
  LED_0F[13] = 0xA1; //d
  LED_0F[14] = 0x86; //E
  LED_0F[15] = 0x8E; //F
  LED_0F[16] = 0xC2; //G
  LED_0F[17] = 0x89; //H
  LED_0F[18] = 0xF9; //I
  LED_0F[19] = 0xF1; //J
  LED_0F[20] = 0xC3; //L
  LED_0F[21] = 0xA9; //n
  LED_0F[22] = 0xC0; //O
  LED_0F[23] = 0x8C; //P
  LED_0F[24] = 0x98; //q
  LED_0F[25] = 0x92; //S
  LED_0F[26] = 0xC1; //U
  LED_0F[27] = 0x91; //Y
  LED_0F[28] = 0xFE; //hight -

    Serial.begin(9600);                                  // Инициируем передачу данных в монитор последовательного порта
    time.begin(); 
    
    Timer1.initialize(1500); 
    Timer1.attachInterrupt(timerIsr);
 //load   
disp.set(LED_0F[20], 3);
delay(1000);
disp.set(LED_0F[22], 2);
delay(1000);
disp.set(LED_0F[10], 1);
delay(1000);
disp.set(LED_0F[13], 0);
delay(1000);
disp.clear();
//----
disp.set(LED_0F[28], 3);
delay(1000);
disp.set(LED_0F[28], 2);
delay(1000);
disp.set(LED_0F[28], 1);
delay(1000);
disp.set(LED_0F[28], 0);
delay(1000);
//cos
disp.set(LED_0F[12], 3);
delay(1000);
disp.set(LED_0F[22], 2);
delay(1000);
disp.set(LED_0F[25], 1);
delay(1000);
disp.clear();
//version
disp.set(LED_0F[26], 3);
delay(1000);
disp.dispFloat(-1.0, 1); //send float indicators
delay(1000);
  
disp.clear();

//data 
   d = (time.day / 10);
   d1 = (time.day % 10);
   mont =(time.month /10);
   mont1 = (time.month % 10);
 disp.clear() ;
 disp.dispFloat(00.00, 2); 
if(d == 0){
  disp.set(LED_0F[0], 3);
  
}
 if(d == 1){
  disp.set(LED_0F[1], 3);
}
 if(d == 2){
  disp.set(LED_0F[2], 3);
}
if(d == 3){
  disp.set(LED_0F[3], 3);
}
 if(d == 4){
  disp.set(LED_0F[4], 3);
 }
 if(d == 5){
  disp.set(LED_0F[5], 3);
}
 if(d == 6){
  disp.set(LED_0F[6], 3);
}
if(d == 7){
  disp.set(LED_0F[7], 3);
}
if(d == 8){
  disp.set(LED_0F[8], 3);
 }
 if(d == 9){
 disp.set(LED_0F[9], 3);
 }

  
if(d1 == 0){
  disp.set(LED_0F[0], 2);
  
}
 if(d1 == 1){
  disp.set(LED_0F[1], 2);
}
 if(d1 == 2){
  disp.set(LED_0F[2], 2);
}
if(d1 == 3){
  disp.set(LED_0F[3], 2);
}
 if(d1 == 4){
  disp.set(LED_0F[4], 2);
 }
 if(d1 == 5){
  disp.set(LED_0F[5], 2);
}
 if(d1 == 6){
  disp.set(LED_0F[6], 2);
}
if(d1 == 7){
  disp.set(LED_0F[7], 2);
}
if(d1 == 8){
  disp.set(LED_0F[8], 2);
 }
 if(d1 == 9){
 disp.set(LED_0F[9], 2);
 }

 
if(mont == 0){
  disp.set(LED_0F[0], 1);
  
}
 if(mont == 1){
  disp.set(LED_0F[1], 1);
}
 if(mont == 2){
  disp.set(LED_0F[2], 1);
}
if(mont == 3){
  disp.set(LED_0F[3], 1);
}
 if(mont == 4){
  disp.set(LED_0F[4], 1);
 }
 if(mont == 5){
  disp.set(LED_0F[5], 1);
}
 if(mont == 6){
  disp.set(LED_0F[6], 1);
}
if(mont == 7){
  disp.set(LED_0F[7], 1);
}
if(mont == 8){
  disp.set(LED_0F[8], 1);
 }
 if(mont == 9){
 disp.set(LED_0F[9], 1);
 }
  
 if(mont1 == 0){
  disp.set(LED_0F[0], 0);
  
}
 if(mont1 == 1){
  disp.set(LED_0F[1], 0);
}
 if(mont1 == 2){
  disp.set(LED_0F[2], 0);
}
if(mont1 == 3){
  disp.set(LED_0F[3], 0);
}
 if(mont1 == 4){
  disp.set(LED_0F[4], 0);
 }
 if(mont1 == 5){
  disp.set(LED_0F[5], 0);
}
 if(mont1 == 6){
  disp.set(LED_0F[6], 0);
}
if(mont1 == 7){
  disp.set(LED_0F[7], 0);
}
if(mont1 == 8){
  disp.set(LED_0F[8], 0);
 }
 if(mont1 == 9){
 disp.set(LED_0F[9], 0);
 }
  
delay(1000);


    
}
  void loop(){
    
//Время на дисплее 



   
    c =( time.Hours / 10) ; //берём от часов целую часть
   c2 =( time.Hours % 10) ;// берём от часов остаток от деления
   m =( time.minutes / 10) ; // берём от минут целую часть
   m1 =( time.minutes % 10) ; // берём от минут остаток от деления
   
   
 Serial.println( time.gettime("d-m-Y, H:i:s, D"));
 

//-----------------------------
if( time.Hours == 00 && time.minutes == 00){
  time.seconds -- ; // отнимает одну секунду для коректировки времени из-за особености RTC
  //data при смене дня 
   d = (time.day / 10);
   d1 = (time.day % 10);
   mont =(time.month /10);
   mont1 = (time.month % 10);
 disp.clear() ;
 disp.dispFloat(00.00, 2); 
if(d == 0){
  disp.set(LED_0F[0], 3);
  
}
 if(d == 1){
  disp.set(LED_0F[1], 3);
}
 if(d == 2){
  disp.set(LED_0F[2], 3);
}
if(d == 3){
  disp.set(LED_0F[3], 3);
}
 if(d == 4){
  disp.set(LED_0F[4], 3);
 }
 if(d == 5){
  disp.set(LED_0F[5], 3);
}
 if(d == 6){
  disp.set(LED_0F[6], 3);
}
if(d == 7){
  disp.set(LED_0F[7], 3);
}
if(d == 8){
  disp.set(LED_0F[8], 3);
 }
 if(d == 9){
 disp.set(LED_0F[9], 3);
 }

  
if(d1 == 0){
  disp.set(LED_0F[0], 2);
  
}
 if(d1 == 1){
  disp.set(LED_0F[1], 2);
}
 if(d1 == 2){
  disp.set(LED_0F[2], 2);
}
if(d1 == 3){
  disp.set(LED_0F[3], 2);
}
 if(d1 == 4){
  disp.set(LED_0F[4], 2);
 }
 if(d1 == 5){
  disp.set(LED_0F[5], 2);
}
 if(d1 == 6){
  disp.set(LED_0F[6], 2);
}
if(d1 == 7){
  disp.set(LED_0F[7], 2);
}
if(d1 == 8){
  disp.set(LED_0F[8], 2);
 }
 if(d1 == 9){
 disp.set(LED_0F[9], 2);
 }

 
if(mont == 0){
  disp.set(LED_0F[0], 1);
  
}
 if(mont == 1){
  disp.set(LED_0F[1], 1);
}
 if(mont == 2){
  disp.set(LED_0F[2], 1);
}
if(mont == 3){
  disp.set(LED_0F[3], 1);
}
 if(mont == 4){
  disp.set(LED_0F[4], 1);
 }
 if(mont == 5){
  disp.set(LED_0F[5], 1);
}
 if(mont == 6){
  disp.set(LED_0F[6], 1);
}
if(mont == 7){
  disp.set(LED_0F[7], 1);
}
if(mont == 8){
  disp.set(LED_0F[8], 1);
 }
 if(mont == 9){
 disp.set(LED_0F[9], 1);
 }
  
 if(mont1 == 0){
  disp.set(LED_0F[0], 0);
  
}
 if(mont1 == 1){
  disp.set(LED_0F[1], 0);
}
 if(mont1 == 2){
  disp.set(LED_0F[2], 0);
}
if(mont1 == 3){
  disp.set(LED_0F[3], 0);
}
 if(mont1 == 4){
  disp.set(LED_0F[4], 0);
 }
 if(mont1 == 5){
  disp.set(LED_0F[5], 0);
}
 if(mont1 == 6){
  disp.set(LED_0F[6], 0);
}
if(mont1 == 7){
  disp.set(LED_0F[7], 0);
}
if(mont1 == 8){
  disp.set(LED_0F[8], 0);
 }
 if(mont1 == 9){
 disp.set(LED_0F[9], 0);
 }
  
delay(1000);
disp.clear();
}
//---------------
if(c == 0){
  disp.set(LED_0F[0], 3);
}
 if(c == 1){
  disp.set(LED_0F[1], 3);
}
 if(c == 2){
  disp.set(LED_0F[2], 3);
}
//--------------
if(c2 == 0){
  disp.set(LED_0F[0], 2);
}
 if(c2 == 1){
  disp.set(LED_0F[1], 2);
}
 if(c2 == 2){
  disp.set(LED_0F[2], 2);
}
if(c2 == 3){
  disp.set(LED_0F[3], 2);
}
 if(c2 == 4){
  disp.set(LED_0F[4], 2);
 }
 if(c2 == 5){
  disp.set(LED_0F[5], 2);
}
 if(c2 == 6){
  disp.set(LED_0F[6], 2);
}
if(c2 == 7){
  disp.set(LED_0F[7], 2);
}
if(c2 == 8){
  disp.set(LED_0F[8], 2);
 }
  if(c2 == 9){
  disp.set(LED_0F[9], 2);
 }
//-----------------------------
if(m == 0){
  disp.set(LED_0F[0], 1);
}
 if(m == 1){
  disp.set(LED_0F[1], 1);
}
 if(m == 2){
  disp.set(LED_0F[2], 1);
}
if(m == 3){
  disp.set(LED_0F[3], 1);
}
 if(m == 4){
  disp.set(LED_0F[4], 1);
 }
 if(m == 5){
  disp.set(LED_0F[5], 1);
}
 if(m == 6){
  disp.set(LED_0F[6], 1);
}
if(m == 7){
  disp.set(LED_0F[7], 1);
}
 if(m == 8){
  disp.set(LED_0F[8], 1);
 }
  if(m == 9){
 disp.set(LED_0F[9], 1);
 }
//----------------------
if(m1 == 0){
  disp.set(LED_0F[0], 0);
}
 if(m1 == 1){
  disp.set(LED_0F[1], 0);
}
 if(m1 == 2){
  disp.set(LED_0F[2], 0);
}
if(m1 == 3){
  disp.set(LED_0F[3], 0);
}
 if(m1 == 4){
  disp.set(LED_0F[4], 0);
}
 if(m1 == 5){
  disp.set(LED_0F[5], 0);
}
if(m1 == 6){
  disp.set(LED_0F[6], 0);
}
 if(m1 == 7){
  disp.set(LED_0F[7], 0);
}
 if(m1 == 8){
  disp.set(LED_0F[8], 0);
}
if(m1 == 9){
  disp.set(LED_0F[9], 0);
}
}

////-----------  
void timerIsr()
{
  disp.timerIsr(); // обновления дисплея 
}


void bibl (){

  }
  
void chasstek ()
{ 
}
