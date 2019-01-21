/******************************************************ОБЩАЯ ИНФОРМАЦИЯ*********
  Версия                      : 1.1
  Сборка номер                : 54
  Сборка дата                 : 08.01.2019
  Сборка устройства           : Молоткин Константин и выпускник ..... 
  Программное Обеспечение     : Молоткин Константин 
  Курс                        : 4
  Учебное заведение           : Самарский авиационный техникум
  Группа                      : ВМ-54.
  Telegram                    : https://t.me/kosia_zlo 
  Канал                       : https://t.me/joinchat/AAAAAFk49unFlsLftfWZDA
  Последня версия ПО          : https://github.com/kosia-zlo/arduino_rtc
  Описание                    : Простые часы на Arduino Nano с RTC модулем и 7 семисегментным дисплеем
  Преподаватель               : Митина Е.Н. 
  
*****************************************************************************************************************/ 
//****************************************************************** БИБЛИОТЕКИ ***********
#include <avr/sleep.h>
#include <SoftwareSerial.h> 
#include <TM74HC595Display.h>
#include <iarduino_RTC.h>  
#include <TimerOne.h>


//***************************************************************** ПОДКЛЮЧЁННЫЕ МОДУЛИ **********
iarduino_RTC time(RTC_DS1307);  // Подключённый модуль RTC 
int SCLK = 12; // выводы дисплея на плате
int RCLK = 11; // 
int DIO = 10; //
TM74HC595Display disp(SCLK, RCLK, DIO); // указываем выводы дисплея 


// *************************************************************** ОБЩИЕ НАСТРОЙКИ ***********************

#define  PAR         1                   // 0 для выключения. 1 для включения. ОТОБРАЖЕНИЯ ПАРЫ
#define  TIMERT      0                   // 0 для выключения. 1 для включения. ТАЙМЕР
#define  TEST        0                   // 0 для выключения. 1 для включения. ФУНКЦИЯ ТЕСТА ЭКРАНА
#define  LOAD        1                   // 0 для выключения. 1 для включения. ФУНКЦИЯ ЗАГРУЗКИ
#define  DISPIN      1                   // 0 для выключения. 1 для включения. ФУНКЦИЯ ОТОБРАЖЕНИЯ ДАТЫ ПРИ ЗАГРУЗКЕ
#define  OTK         0                   // 0 для выключения. 1 для включения.  ОТКЛЮЧЕНИЕ ДИСПЛЕЯ В ПРОСТОЕ 
#define  OTK_VER     5                   // ВРЕМЯ ПРОСТОЯ ДО ОТКЛЮЧЕНИЯ ДИСПЛЕЯ не сделал  /сделать разделения по сегментам
#define  COLBIB      100                 //  Символов в библиотеки +1 для запаса
#define  VER         1.1                //  ВЕРСИЯ ПО НЕ ТРОГАЙ/ ЕСЛИ ЧТО-ТО ИЗМЕНЯЕТЕ В КОДЕ, СООБЩИТЕ МНЕ!!!
#define  SBR         54                 //  ВЕРСИЯ СБОРКИ НЕ ТРОГАЙ/ ЕСЛИ ЧТО-ТО ИЗМЕНЯЕТЕ В КОДЕ, СООБЩИТЕ МНЕ!!!
#define  SBR_DATA    08.01.2019         //  ДАТА СБОРКИ НЕ ТРОГАЙ/ ЕСЛИ ЧТО-ТО ИЗМЕНЯЕТЕ В КОДЕ, СООБЩИТЕ МНЕ!!!
/*
 * ПЛАНИРУЕМЫЕ ФУНКЦИИ
 * ВРЕМЯ ПРОСТОЯ ДО ОТКЛЮЧЕНИЯ ДИСПЛЕЯ  
 * сделать разделения по сегментам в анимациях
 * 4 и 5 режим змейки
 * НОЧНОЙ РЕЖИМ!
 * 
 */


// *************************************************************** НАСТРОЙКИ БУДИЛЬНИКА ***********************

#define  BUD         0     // 0 для выключения. 1 для включения. ФУНКЦИЯ БУДИЛЬНИКА
#define  BUD_CHAS    6     // время будильника (часы)
#define  BUD_MIN     30     // время будильника (минуты)

// *************************************************************** НАСТРОЙКИ НОЧНОГО РЕЖИМА ***********************

#define  NOCH             1    // 0 для выключения. 1 для включения. ФУНКЦИЯ НОЧЬ
#define  NOCH_CHAS_ON     1   // время ночного режима (вкл)
#define  NOCH_CHAS_OFF    7   // время ночного режима (выкл)

// *************************************************************** НАСТРОЙКИ АНИМАЦИИ ***********************
#define  ANIM        2                   //  ФУНКЦИЯ АНИМАЦИИ 0 - выключение 1 - бегующие строчки 2- вращение 3- "конфети"
#define  ANIM_R      1                   //  0 для выключения. 1 для включения. РАНДОМНАЯ АНИМАЦИЯ С ОПРЕДЕЛЁННЫМ ПЕРИОДОМ
#define  ANIM_T      0.3                 //  ВРЕМЯ РАНДОМНОЙ АНИМАЦИИ минуты. 0 для рандомного числа
#define  ZMEI        75                  //  СКОРОСТЬ ЗМЕЙКИ
#define  TEST_T      150                 //  СКОРОСТЬ ПРОГОНКИ ТЕСТА
//***************************************************************ПЕРЕМЕННЫЕ*****************

int chaws = 0;                                                          //таймер
int mint = 0;                                                           //минуточки
int var = 0;                                                            //
int mint1 = 0;
int cr = 0;                                                             // переменная вывода дисплея
int para_flag = 0;                                                      // переменная отображения пар
int c = 0; // переменная часов целая часть
int cday = 0;
int c2 = 0;
int m = 0; // переменная минут целая часть
int m1 = 0;
//int d =0 ;
//int d1 =0; 
//int mont = 0 ;
//int mont1 = 0;
unsigned long last_time = 0; //
long timer_anim; // 
unsigned char LED_0F[COLBIB]; // количество символов в библиотеки +1




// *************************************************ДЛЯ РАЗРАБОТЧИКОВ ***********************
void setup() {
   time.begin(); 
   Serial.begin(9600);                                  // Инициируем передачу данных в монитор последовательного порта
   Timer1.initialize(1600); 
   Timer1.attachInterrupt(timerIsr);
   if (ANIM_T == 0 ){
   timer_anim = random(1, 60);}

  bibl();
    for( int x = 0; x<4; x++)
    {zmeika();} 
   disp.clear();
  if (TEST == 1){
  test();}
   if (LOAD == 1 ){
  loading();} 
 if( DISPIN == 1 ){
  datadisp();
 }
  if (NOCH == 1 ){ 
  if ( NOCH_CHAS_OFF  > time.Hours ){
disp.set(LED_0F[0], 3);
  disp.dispFloat(time.Hours, 2);
  disp.dispFloat(time.minutes, 0);
  delay(1000);
  }
  else {
  disp.dispFloat(time.Hours, 2);
  disp.dispFloat(time.minutes, 0);
  delay(1000);
  }
  }
    mint1 = time.minutes ;
    chaws = time.Hours +1;
    mint = time.minutes ;
  set_sleep_mode( SLEEP_MODE_IDLE) ;     // Режим энерго сбережения
  sleep_mode();                         // Режим пониженого энергопотребления включается

}
void loop(){
  if (ANIM_R == 1 ){
    if (ANIM_T == 0 ){
     // Serial.println(timer_anim);
      if(millis() - last_time > (timer_anim)*1000*60) // 60 количество секунд в минуте / 1000 = 1 секунда.
   {
    //Serial.println("АНИМАЦИЯ c 0");
    anima();
    timer_anim = random(1, 60);
    last_time = millis();
  }
    }
    
    else{
     if(millis() - last_time > (ANIM_T)*1000*60) // 60 количество секунд в минуте / 1000 = 1 секунда.
   {
   // Serial.println("АНИМАЦИЯ");
    anima();
    last_time = millis();
  }
  }
  }
  if (BUD == 1 ){
    budilnik();}
  if (TIMERT == 1){
    timer();}
  if (PAR == 1 ){
    para(); }    
//Время на дисплее
if (NOCH == 1 ){
  hochka();
} 
  
 if(time.minutes == 30 && time.seconds == 00){
  datadisp();  }
  if( time.Hours == chaws && time.minutes == 00 && time.seconds == 00 ){
  time.seconds -- ; // отнимает одну секунду для коректировки времени из-за особености RTC
  datadisp();
 chaws = chaws + 1;
 if (chaws==24){
 chaws == 0;
 }}
//--------------------------------- 
if(time.minutes != mint & time.minutes != 00 & time.minutes != 30 ) 
   
 {
      if (ANIM == 0 ){
      mint = time.minutes;}
     if (ANIM == 1 ){
    probeg();
    mint = time.minutes ;
    }
     if (ANIM == 2 ){
    crug();
    mint = time.minutes ;
    }
     if (ANIM == 3 ){
    confeti();
    mint = time.minutes ;
    } 
    }  
    
if (10 >time.Hours){
  disp.set(LED_0F[0], 3);
  disp.dispFloat(time.Hours, 2);
  disp.dispFloat(time.minutes, 0);
  delay(900);
  }
  else{
  disp.dispFloat(time.Hours, 2);
  disp.dispFloat(time.minutes, 0);
  delay(1000);
  }
  

  

  Serial.println( time.gettime("d-m-Y, H:i:s, D"));
 }
////-----------  
void timerIsr()
{
  disp.timerIsr(); // обновления дисплея ЭТУ ФУНКЦИЮ НЕ ТРОГАЕМ!
}

void bibl (){
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
  LED_0F[21] = 0xA9; //h
  LED_0F[22] = 0xC0; //O
  LED_0F[23] = 0x8C; //P
  LED_0F[24] = 0x98; //q
  LED_0F[25] = 0x92; //S
  LED_0F[26] = 0xC1; //U
  LED_0F[27] = 0x91; //Y
  // Черточки
  LED_0F[28] = 0xFE; //hight -
  LED_0F[29] = 0b10111111; //mid -
  LED_0F[30] = 0b11110111; //low -
  LED_0F[31] = 0b11110110; //low&hight -
  LED_0F[32] = 0b10110110; // low&hight&mid -
  LED_0F[33] = 0b11111111; //
  LED_0F[34] = 0b00000000; // полное заполнение
  
  LED_0F[35] = 0b01110111; // _.
  
  //Эффект прокрутки              \\ '-' - верхний сегмент  \\ -' - верхний правый боковой сегмент \\  '- - верхний левый боковой сегмент , - нижний сегмент
  LED_0F[36] = 0b11111110; // 
  LED_0F[37] = 0b11111101; //
  LED_0F[38] = 0b11111011; //
  LED_0F[39] = 0b11110111; // 
  LED_0F[40] = 0b11101111; //
  LED_0F[41] = 0b11011111; //
  LED_0F[42] = 0b10111111; //
  LED_0F[43] = 0b01111111; // .
  
  //Маленькие буквы
  LED_0F[44] = 0b10100011; // о
  LED_0F[45] = 0b00101011; // n.
  LED_0F[46] = 0b01100011; // v.
  LED_0F[47] = 0b11100011; // v 
  LED_0F[48] = 0b01111111; // v. резерв
  LED_0F[49] = 0b01111111; // v. резерв

  //Вертикальные чёрточки  \\   *- не горит сторона \\ |- сторона полностью \\ ' - верхний боковой сегмент \\ , - нижний боковой сегмент \\   
  LED_0F[50] = 0xF9;       // *|
  LED_0F[51] = 0b11101101; // ,'
  LED_0F[52] = 0b11011011; // ',
  LED_0F[53] = 0b11001111; // |*
  LED_0F[54] = 0b11011101; // ''
  LED_0F[55] = 0b11101011; // ,,
  LED_0F[56] = 0b11001001; // ||

 //Элементы змейки
 LED_0F[60] = 0b11111100; // Верхний и боковой
 LED_0F[61] = 0b11111000; // Верхний и боковой 2
 LED_0F[62] = 0b11110000; // Верхний и боковой 2 и нижний 
 LED_0F[63] = 0b11110001; // Боковой 2 и нижний 
 LED_0F[64] = 0b11110011; // Боковой  и нижний 
 LED_0F[65] = 0b11100111; // 
 LED_0F[66] = 0b10100111; // 
 LED_0F[67] = 0b10101111; // 
 LED_0F[68] = 0b10011111; // Боковой  и средний
 LED_0F[69] = 0b10011101; // Боковой  и средний
 LED_0F[70] = 0b10111101; // Боковой  и средний
 LED_0F[71] = 0b11011100; // Боковой и верхний  и средний
 LED_0F[72] = 0b11011000; // Боковой и верхний  и средний
 LED_0F[73] = 0b11100001; // Боковой 2 и нижний 
 LED_0F[74] = 0b11000111; // Боковой 2 и нижний 
 LED_0F[75] = 0b11001110; // Боковой 2 и нижний
 LED_0F[76] = 0b11011110; // Боковой 2 и нижний  
}
void loading ()
{ 
if (ANIM == 0 ){ 
disp.clear();
//load   
disp.set(LED_0F[20], 3);
disp.set(LED_0F[22], 2);
disp.set(LED_0F[10], 1);
disp.set(LED_0F[13], 0);
delay(500);
disp.clear();
//cos
disp.set(LED_0F[12], 3);
delay(100);
disp.set(LED_0F[22], 2);
delay(100);
disp.set(LED_0F[25], 1);
delay(100);
disp.clear();
//version
disp.set(LED_0F[46], 3);
disp.dispFloat(-(VER), 1); //индикация с точкой
delay(1250);
disp.clear();
}
if (ANIM == 1 ){
    probeg();
    //load 
disp.set(LED_0F[28], 3);
delay(50);  
disp.set(LED_0F[20], 3);
disp.set(LED_0F[28], 2);
delay(50);
disp.set(LED_0F[22], 2);
disp.set(LED_0F[28], 1);
delay(50);
disp.set(LED_0F[10], 1);
disp.set(LED_0F[28], 0);
delay(50);
disp.set(LED_0F[13], 0);
delay(550);
disp.clear();
//cos
disp.set(LED_0F[28], 3);
delay(50);
disp.set(LED_0F[12], 3);
delay(100);
disp.set(LED_0F[28], 2);
delay(50);
disp.set(LED_0F[22], 2);
delay(100);
disp.set(LED_0F[28], 1);
delay(50);
disp.set(LED_0F[25], 1);
delay(100);
disp.clear();
//version
disp.set(LED_0F[28], 3);
delay(50);
disp.set(LED_0F[46], 3);
disp.set(LED_0F[28], 2);
delay(50);
disp.set(LED_0F[28], 1);
delay(50);
disp.set(LED_0F[28], 0);
delay(50);
disp.dispFloat(-(VER), 1); //индикация с точкой
delay(1250);
disp.clear();
probeg();
    }
if (ANIM == 2 ){
    crug();
    disp.clear();
//load 
cr=3;
crugif();  
disp.set(LED_0F[20], 3);
cr=2;
crugif();
disp.set(LED_0F[22], 2);
cr=1;
crugif();
disp.set(LED_0F[10], 1);
cr=0;
crugif();
disp.set(LED_0F[13], 0);
delay(500);
disp.clear();
//cos
cr=3;
crugif();
disp.set(LED_0F[12], 3);
delay(100);
cr=2;
crugif();
disp.set(LED_0F[22], 2);
delay(100);
cr=1;
crugif();
disp.set(LED_0F[25], 1);
delay(100);
disp.clear();

//version
cr=3;
crugif();
disp.set(LED_0F[46], 3);
cr=2;
crugif();
disp.set(LED_0F[29], 2);
cr=1;
crugif();
cr=0;
crugif();
disp.dispFloat(-(VER), 1); //индикация с точкой
delay(1250);
disp.clear();
crug();
}    
if (ANIM == 3 ){
    confeti();
    disp.clear();
//load 
cr=3;
confetiif();  
disp.set(LED_0F[20], 3);
cr=2;
confetiif();
disp.set(LED_0F[22], 2);
cr=1;
confetiif();
disp.set(LED_0F[10], 1);
cr=0;
confetiif();
disp.set(LED_0F[13], 0);
delay(500);
disp.clear();
//cos
cr=3;
confetiif();
disp.set(LED_0F[12], 3);
delay(100);
cr=2;
confetiif();
disp.set(LED_0F[22], 2);
delay(100);
cr=1;
confetiif();
disp.set(LED_0F[25], 1);
delay(100);
disp.clear();

//version
cr=3;
confetiif();
disp.set(LED_0F[46], 3);
cr=2;
confetiif();
cr=1;
confetiif();
cr=0;
confetiif();
disp.dispFloat(-(VER), 1); //индикация с точкой
delay(1250);
disp.clear();
   confeti();
    }  
}
void datadisp()
{
 // d = (time.day / 10);
 // d1 = (time.day % 10);
 // mont =(time.month /10);
 // mont1 = (time.month % 10);
if (ANIM == 0 ){
  //data 
  disp.clear();
  disp.set(LED_0F[13], 3);
  disp.set(LED_0F[10], 2);
  disp.set(LED_0F[18], 1);
  disp.set(LED_0F[10], 0);
  delay(1300);
  disp.clear() ;
if (10 >time.day){
  disp.set(LED_0F[0], 3);
  disp.dispFloat(time.day, 2);
 if(10 >time.month){
  disp.set(LED_0F[0], 1);
  disp.dispFloat(time.month, 0);
}

else{ 
  disp.dispFloat(time.month, 0);
}
}
else{
  disp.dispFloat(time.day, 2);
}
if(10 >time.month){
  disp.set(LED_0F[0], 1);
  disp.dispFloat(time.month, 0);
}
else{ 
  disp.dispFloat(time.month, 0);
}
delay(1200);
//год
disp.clear() ;
disp.set(LED_0F[2], 3);
disp.set(LED_0F[0], 2);
disp.dispFloat(time.year,0 );
delay(1500);
//день недели 
  disp.clear() ;
  disp.set(LED_0F[13], 3);
  disp.set(LED_0F[29], 2);
  delay(50);
   disp.set(LED_0F[29], 1);
  delay(50);
  disp.dispFloat(time.weekday, 0);
  delay(1100);
disp.clear() ;
}
if (ANIM == 1 ) {
 disp.clear();
 //data 
 disp.set(LED_0F[28], 3);
  delay(50);
disp.set(LED_0F[13], 3);
disp.set(LED_0F[28], 2);
delay(50);
disp.set(LED_0F[10], 2);
  disp.set(LED_0F[28], 1);
  delay(50);
disp.set(LED_0F[18], 1);
disp.set(LED_0F[28], 0);
  delay(50);
disp.set(LED_0F[10], 0);
delay(1300);
disp.clear() ;
if (10 >time.day){
   disp.set(LED_0F[28], 3);
  delay(30);
  disp.set(LED_0F[0], 3);
   disp.set(LED_0F[28], 2);
  delay(30);
  disp.dispFloat(time.day, 2);
 if(10 >time.month){
  disp.set(LED_0F[28], 1);
  delay(30);
  disp.set(LED_0F[0], 1);
   disp.set(LED_0F[28], 0);
  delay(30);
  disp.dispFloat(time.month, 0);
}

else{
   
  disp.set(LED_0F[28], 1);
  delay(30);
   disp.set(LED_0F[28], 0);
  delay(30);
  disp.dispFloat(time.month, 0);
}
}
else{
    disp.set(LED_0F[28], 3);
  delay(30);
   disp.set(LED_0F[28], 2);
  delay(30);
  disp.dispFloat(time.day, 2);
}
if(10 >time.month){
  disp.set(LED_0F[28], 1);
  delay(30);
  disp.set(LED_0F[0], 1);
  disp.set(LED_0F[28], 0);
  delay(30);
  disp.dispFloat(time.month, 0);
}
else{ 
  disp.set(LED_0F[28], 1);
  delay(30);
   disp.set(LED_0F[28], 0);
  delay(30);
  disp.dispFloat(time.month, 0);
}
delay(1200);
//год
disp.clear() ;
 disp.set(LED_0F[28], 3);
  delay(50);
disp.set(LED_0F[2], 3);
 disp.set(LED_0F[28], 2);
  delay(50);
disp.set(LED_0F[0], 2);
 disp.set(LED_0F[28], 1);
  delay(50);
   disp.set(LED_0F[28], 0);
  delay(50);
disp.dispFloat(time.year,0 );
delay(1500);
//день недели 
  disp.clear() ;
   disp.set(LED_0F[28], 3);
  delay(50);
  disp.set(LED_0F[13], 3);
  disp.set(LED_0F[29], 2);
  delay(50);
  disp.set(LED_0F[29], 1);
  delay(50);
  disp.set(LED_0F[28], 0);
  delay(50);
  disp.dispFloat(time.weekday, 0);
  delay(1100);
disp.clear() ;
probeg();
}
if (ANIM == 2 ) {
disp.clear();
//data
  cr=3;
  crugif();
  disp.set(LED_0F[13], 3);
  cr=2;
  crugif();
  disp.set(LED_0F[10], 2);
  cr=1;
  crugif();  
  disp.set(LED_0F[18], 1);
  cr=0;
  crugif();  
  disp.set(LED_0F[10], 0);
  delay(1300);
  disp.clear() ;
if (10 >time.day){
  cr=3;
  crugif();  
  disp.set(LED_0F[0], 3);
  cr=2;
  crugif();
  disp.dispFloat(time.day, 2);
}
else{ 
  cr=3;
  crugif();
  cr=2;
  crugif();
  disp.dispFloat(time.day, 2);
}
if(10 >time.month){
  cr=1;
  crugif();
  disp.set(LED_0F[0], 1);
  cr=0;
  crugif();
  disp.dispFloat(time.month, 0);
}
else{ 
  cr=1;
  crugif();
  cr=0;
  crugif();
  disp.dispFloat(time.month, 0);
}
delay(1200);
//год
disp.clear() ;
cr=3;
crugif();
disp.set(LED_0F[2], 3);
cr=2;
crugif();
disp.set(LED_0F[0], 2);
cr=1;
crugif();
cr=0;
crugif();
disp.dispFloat(time.year,0 );
delay(1500);
//день недели 
  disp.clear() ;
  cr=3;
  crugif();
  disp.set(LED_0F[13], 3);
  cr=2;
  crugif();
  disp.set(LED_0F[29], 2);
  delay(50);
  cr=1;
  crugif();
  disp.set(LED_0F[29], 1);
  delay(50);
  cr=0;
  crugif();
  disp.dispFloat(time.weekday, 0);
  delay(1100);
disp.clear() ;
crug();
}
if (ANIM == 3 ) {
disp.clear();
//data
  cr=3;
  confetiif();
  disp.set(LED_0F[13], 3);
  cr=2;
  confetiif();
  disp.set(LED_0F[10], 2);
  cr=1;
  confetiif();  
  disp.set(LED_0F[18], 1);
  cr=0;
  confetiif();  
  disp.set(LED_0F[10], 0);
  delay(1300);
  disp.clear() ;
if (10 >time.day){
  cr=3;
  confetiif();  
  disp.set(LED_0F[0], 3);
  cr=2;
  confetiif();
  disp.dispFloat(time.day, 2);
}
else{
  cr=3;
  confetiif();
  cr=2;
  confetiif();
  disp.dispFloat(time.day, 2);
}
if(10 >time.month){
  cr=1;
  confetiif();
  disp.set(LED_0F[0], 1);
  cr=0;
  confetiif();
  disp.dispFloat(time.month, 0);
}
else{ 
  cr=1;
  confetiif();
  cr=0;
  confetiif();
  disp.dispFloat(time.month, 0);
}
delay(1200);
//год
disp.clear() ;
cr=3;
confetiif();
disp.set(LED_0F[2], 3);
cr=2;
confetiif();
disp.set(LED_0F[0], 2);
cr=1;
confetiif();
cr=0;
confetiif();
disp.dispFloat(time.year,0 );
delay(1500);
//день недели 
  disp.clear() ;
  cr=3;
  confetiif();
  disp.set(LED_0F[13], 3);
  cr=2;
  confetiif();
  disp.set(LED_0F[29], 2);
  delay(50);
  cr=1;
  confetiif();
  disp.set(LED_0F[29], 1);
  delay(50);
  cr=0;
  confetiif();
  disp.dispFloat(time.weekday, 0);
  delay(1100);
disp.clear() ;
confeti();
}
}
void hochka(){
 if (NOCH_CHAS_ON < time.Hours & time.Hours < NOCH_CHAS_OFF) {
  
    disp.clear();
    
  if(millis() - last_time > 1000*60){
    if (ANIM == 0 ){
  disp.set(LED_0F[12], 3);
  disp.set(LED_0F[22], 2);
  disp.set(LED_0F[17], 1);
  disp.set(LED_0F[28], 0);
  delay(500); 
  }
      if (ANIM == 1 ){
  disp.set(LED_0F[12], 3);
  disp.set(LED_0F[22], 2);
  disp.set(LED_0F[17], 1);
  disp.set(LED_0F[28], 0);
  delay(500); 
  }
      if (ANIM == 2 ){
  disp.set(LED_0F[12], 3);
  disp.set(LED_0F[22], 2);
  disp.set(LED_0F[17], 1);
  disp.set(LED_0F[28], 0);
  delay(500); 
  }
      if (ANIM == 3 ){
  disp.set(LED_0F[12], 3);
  disp.set(LED_0F[22], 2);
  disp.set(LED_0F[17], 1);
  disp.set(LED_0F[28], 0);
  delay(500); 
  }
  last_time = millis();
  }
  else{
  disp.clear();
}

 set_sleep_mode( SLEEP_MODE_IDLE) ;     // Режим энерго сбережения
  sleep_mode(); // Режим пониженого энергопотребления включается
  delay(10000);
}
} 
void para (){
  if( para_flag == 0){
 
    if (ANIM == 0 ){
  disp.set(LED_0F[23], 3);
  disp.set(LED_0F[29], 2);
  disp.set(LED_0F[44], 1);
  disp.set(LED_0F[45], 0);
  delay(1000);
  para_flag = 1;
    }
    if (ANIM == 1 ){
   disp.set(LED_0F[28], 3);
   delay(50); 
   disp.set(LED_0F[23], 3);
   disp.set(LED_0F[28], 2);
   delay(50); 
   disp.set(LED_0F[29], 2);
   disp.set(LED_0F[28], 1);
   delay(50); 
   disp.set(LED_0F[44], 1);
   disp.set(LED_0F[28], 0);
   delay(50); 
   disp.set(LED_0F[45], 0);
   delay(1000);
   para_flag = 1;
  }
    if (ANIM == 2 ){
      cr=3;
      crugif();
  disp.set(LED_0F[23], 3);
      cr=2;
      crugif();
  disp.set(LED_0F[29], 2);
      cr=1;
      crugif();
  disp.set(LED_0F[44], 1);
      cr=0;
      crugif();
  disp.set(LED_0F[45], 0);
  delay(1000);
  para_flag = 1;
    }
    if (ANIM == 3 ){
      cr=3;
      confetiif();
  disp.set(LED_0F[23], 3);
      cr=2;
      confetiif();
  disp.set(LED_0F[29], 2);
      cr=1;
      confetiif();
  disp.set(LED_0F[44], 1);
      cr=0;
      confetiif();
  disp.set(LED_0F[45], 0);
  delay(1000);
  para_flag = 1;
    }
  }
  if( para_flag == 1){
    if (ANIM == 0 ){
  if(time.Hours == 9){
      if(time.minutes == 00){
      disp.set(LED_0F[23], 3);
      disp.dispFloat(-1.0, 1); //индикация с точкой
      delay(1000);
      }
      }
      if(time.Hours == 9){
      if(time.minutes == 50){
      disp.set(LED_0F[23], 3);
      disp.dispFloat(-1.5, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 10){
      if(time.minutes == 45){
      disp.set(LED_0F[23], 3);
      disp.dispFloat(-2.0, 1); //индикация с точкой
      delay(1000);
      }
      }
      if(time.Hours == 11){
      if(time.minutes == 35){
      disp.set(LED_0F[23], 3);
      disp.dispFloat(-2.0, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 12){
      if(time.minutes == 50){
      disp.set(LED_0F[23], 3);
      disp.dispFloat(-3.0, 1); //индикация с точкой
      delay(1000);
      }
      }
      if(time.Hours == 13){
      if(time.minutes == 40){
      disp.set(LED_0F[23], 3);
      disp.dispFloat(-3.5, 1); //индикация с точкой
      delay(1000);
      }
      }
      if(time.Hours == 14){
      if(time.minutes == 25){
      disp.set(LED_0F[23], 3);
      disp.dispFloat(-4.0, 1); //индикация с точкой
      delay(1000);
      }
      }
      if(time.Hours == 15){
      if(time.minutes == 20){
      disp.set(LED_0F[23], 3);
      disp.dispFloat(-4.5, 1); //индикация с точкой
      }
      }
      if(time.Hours == 16){
      if(time.minutes == 5){
      disp.set(LED_0F[23], 3);
      disp.dispFloat(-4.5, 1); //индикация с точкой
      delay(1000);
      }
      }
      delay(1000);
    }
    
    if (ANIM == 1 ){
    if(time.Hours == 9){
      if(time.minutes == 00){
      disp.set(LED_0F[28], 3);
      delay(50); 
      disp.set(LED_0F[23], 3);
      disp.set(LED_0F[28], 2);
      delay(50);
      disp.set(LED_0F[28], 1);
      delay(50); 
      disp.set(LED_0F[28], 0);
      delay(50);  
      disp.dispFloat(-1.0, 1); //индикация с точкой
      delay(1000);
      }
      }
      if(time.Hours == 9){
      if(time.minutes == 50){
      disp.set(LED_0F[28], 3);
      delay(50); 
      disp.set(LED_0F[23], 3);
      disp.set(LED_0F[28], 2);
      delay(50);
      disp.set(LED_0F[28], 1);
      delay(50); 
      disp.set(LED_0F[28], 0);
      delay(50);  
      disp.dispFloat(-1.5, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 10){
      if(time.minutes == 45){
      disp.set(LED_0F[28], 3);
      delay(50); 
      disp.set(LED_0F[23], 3);
      disp.set(LED_0F[28], 2);
      delay(50);
      disp.set(LED_0F[28], 1);
      delay(50); 
      disp.set(LED_0F[28], 0);
      delay(50);  
      disp.dispFloat(-2.0, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 11){
      if(time.minutes == 35){
      disp.set(LED_0F[28], 3);
      delay(50); 
      disp.set(LED_0F[23], 3);
      disp.set(LED_0F[28], 2);
      delay(50);
      disp.set(LED_0F[28], 1);
      delay(50); 
      disp.set(LED_0F[28], 0);
      delay(50);  
      disp.dispFloat(-2.0, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 12){
      if(time.minutes == 50){
      disp.set(LED_0F[28], 3);
      delay(50); 
      disp.set(LED_0F[23], 3);
      disp.set(LED_0F[28], 2);
      delay(50);
      disp.set(LED_0F[28], 1);
      delay(50); 
      disp.set(LED_0F[28], 0);
      delay(50);  
      disp.dispFloat(-3.0, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 13){
      if(time.minutes == 40){
      disp.set(LED_0F[28], 3);
      delay(50); 
      disp.set(LED_0F[23], 3);
      disp.set(LED_0F[28], 2);
      delay(50);
      disp.set(LED_0F[28], 1);
      delay(50); 
      disp.set(LED_0F[28], 0);
      delay(50);  
      disp.dispFloat(-3.5, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 14){
      if(time.minutes == 25){
      disp.set(LED_0F[28], 3);
      delay(50); 
      disp.set(LED_0F[23], 3);
      disp.set(LED_0F[28], 2);
      delay(50);
      disp.set(LED_0F[28], 1);
      delay(50); 
      disp.set(LED_0F[28], 0);
      delay(50);  
      disp.dispFloat(-4.0, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 15){
      if(time.minutes == 20){
      disp.set(LED_0F[28], 3);
      delay(50); 
      disp.set(LED_0F[23], 3);
      disp.set(LED_0F[28], 2);
      delay(50);
      disp.set(LED_0F[28], 1);
      delay(50); 
      disp.set(LED_0F[28], 0);
      delay(50);  
      disp.dispFloat(-4.5, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 16){
      if(time.minutes == 5){
      disp.set(LED_0F[28], 3);
      delay(50); 
      disp.set(LED_0F[23], 3);
      disp.set(LED_0F[28], 2);
      delay(50);
      disp.set(LED_0F[28], 1);
      delay(50); 
      disp.set(LED_0F[28], 0);
      delay(50);  
      disp.dispFloat(-5.0, 1); 
      delay(1000);//индикация с точкой
      }
      }
      delay(1000);
    }
    if (ANIM == 2 ){

    if(time.Hours == 9){
      if(time.minutes == 00){
        cr=3;
      crugif();
      disp.set(LED_0F[23], 3);
       cr=2;
      crugif();
       cr=1;
     crugif();
       cr=0;
     crugif();
      disp.dispFloat(-1.0, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 9){
      if(time.minutes == 50){
       cr=3;
      crugif();
      disp.set(LED_0F[23], 3);
       cr=2;
      crugif();
       cr=1;
      crugif();
       cr=0;
      crugif();
      disp.dispFloat(-1.5, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 10){
      if(time.minutes == 45){
         cr=3;
      crugif();
      disp.set(LED_0F[23], 3);
       cr=2;
      crugif();
       cr=1;
      crugif();
       cr=0;
      crugif();
      disp.dispFloat(-2.0, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 11){
      if(time.minutes == 35){
         cr=3;
      crugif();
      disp.set(LED_0F[23], 3);
       cr=2;
      crugif();
       cr=1;
      crugif();
       cr=0;
      crugif();
      disp.dispFloat(-2.0, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 12){
      if(time.minutes == 50){
        cr=3;
      crugif();
      disp.set(LED_0F[23], 3);
       cr=2;
      crugif();
       cr=1;
      crugif();
       cr=0;
      crugif();
      disp.dispFloat(-3.0, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 13){
      if(time.minutes == 40){
         cr=3;
      crugif();
      disp.set(LED_0F[23], 3);
       cr=2;
     crugif();
       cr=1;
      crugif();
       cr=0;
     crugif();
      disp.dispFloat(-3.5, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 14){
      if(time.minutes == 25){
         cr=3;
      crugif();
      disp.set(LED_0F[23], 3);
       cr=2;
      crugif();
       cr=1;
      crugif();
       cr=0;
     crugif();
      disp.dispFloat(-4.0, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 15){
      if(time.minutes == 20){
         cr=3;
      crugif();
      disp.set(LED_0F[23], 3);
       cr=2;
      crugif();
       cr=1;
      crugif();
       cr=0;
      crugif();
      disp.dispFloat(-4.5, 1); 
      delay(1000);//индикация с точкой
      }
      }
       
      if(time.Hours == 16){
      if(time.minutes == 05){
        cr=3;
      crugif();
      disp.set(LED_0F[23], 3);
       cr=2;
      crugif();
       cr=1;
      crugif();
       cr=0;
      crugif();
      disp.dispFloat(-4.5, 1); 
      delay(1000);//индикация с точкой
      }
      }
    }
    if (ANIM == 3 ){
  if(time.Hours == 9){
      if(time.minutes == 00){
        cr=3;
      confetiif();
      disp.set(LED_0F[23], 3);
       cr=2;
      confetiif();
       cr=1;
      confetiif();
       cr=0;
      confetiif();
      disp.dispFloat(-1.0, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 9){
      if(time.minutes == 50){
       cr=3;
      confetiif();
      disp.set(LED_0F[23], 3);
       cr=2;
      confetiif();
       cr=1;
      confetiif();
       cr=0;
      confetiif();
      disp.dispFloat(-1.5, 1); 
      delay(1000);//индикация с точкой
      }
      }
      if(time.Hours == 10){
      if(time.minutes == 45){
         cr=3;
      confetiif();
      disp.set(LED_0F[23], 3);
       cr=2;
      confetiif();
       cr=1;
      confetiif();
       cr=0;
      confetiif();
      disp.dispFloat(-2.0, 1); //индикация с точкой
      delay(1000);}
      }
      if(time.Hours == 11){
      if(time.minutes == 35){
         cr=3;
      confetiif();
      disp.set(LED_0F[23], 3);
       cr=2;
      confetiif();
       cr=1;
      confetiif();
       cr=0;
      confetiif();
      disp.dispFloat(-2.0, 1); //индикация с точкой
      delay(1000);}
      }
      if(time.Hours == 12){
      if(time.minutes == 50){
        cr=3;
      confetiif();
      disp.set(LED_0F[23], 3);
       cr=2;
      confetiif();
       cr=1;
      confetiif();
       cr=0;
      confetiif();
      disp.dispFloat(-3.0, 1); //индикация с точкой
      delay(1000);}
      }
      if(time.Hours == 13){
      if(time.minutes == 40){
         cr=3;
      confetiif();
      disp.set(LED_0F[23], 3);
       cr=2;
      confetiif();
       cr=1;
      confetiif();
       cr=0;
      confetiif();
      disp.dispFloat(-3.5, 1); //индикация с точкой
      delay(1000);}
      }
      if(time.Hours == 14){
      if(time.minutes == 25){
         cr=3;
      confetiif();
      disp.set(LED_0F[23], 3);
       cr=2;
      confetiif();
       cr=1;
      confetiif();
       cr=0;
      confetiif();
      disp.dispFloat(-4.0, 1); //индикация с точкой
      delay(1000);}
      }
      if(time.Hours == 15){
      if(time.minutes == 20){
         cr=3;
      confetiif();
      disp.set(LED_0F[23], 3);
       cr=2;
      confetiif();
       cr=1;
      confetiif();
       cr=0;
      confetiif();
      disp.dispFloat(-4.5, 1); //индикация с точкой
      delay(1000);}
      }
       
      if(time.Hours == 16){
      if(time.minutes == 05){
        cr=3;
      confetiif();
      disp.set(LED_0F[23], 3);
       cr=2;
      confetiif();
       cr=1;
      confetiif();
       cr=0;
      confetiif();
      disp.dispFloat(-4.5, 1); //индикация с точкой
      delay(1000);}
      }
  }
}
}
  
 void timer(){
  if(TIMERT == 1){
  disp.set(LED_0F[18], 3);
  disp.set(LED_0F[28], 2);
  disp.set(LED_0F[22], 1);
  disp.set(LED_0F[21], 0);
  delay(1500);
  disp.clear();
   if(millis() - last_time > (unsigned long) 60*1000) // 60 количество секунд в минуте / 1000 = 1 секунда.
   {
    last_time = millis();
    Serial.println("ВЫШЛО ВРЕМЯ ТАЙМЕРА");
    Serial.println("ТАЙМЕР ЗАПУЩЁН СНОВА");
   }
 }
 }
//Будильник
void budilnik()
{
  
  if( time.Hours == BUD_CHAS && time.minutes == BUD_MIN && time.seconds == 00)
  {
    disp.clear();
for ( int x = 0; x<4; x++){
  disp.dispFloat(time.Hours, 2);
  disp.dispFloat(time.minutes, 0);
  delay(500);
  disp.clear();
}

    
  }

}



//***************************************************** АНИМАЦИИ *****************************
//Бегующие строчки
void probeg()
{
  disp.set(LED_0F[28], 3);
  delay(50);
   disp.set(LED_0F[28], 2);
  delay(50);
   disp.set(LED_0F[28], 1);
  delay(50);
   disp.set(LED_0F[28], 0);
  delay(50);
  disp.clear() ; 
      disp.set(LED_0F[29], 3);
  delay(50);
   disp.set(LED_0F[29], 2);
  delay(50);
   disp.set(LED_0F[29], 1);
  delay(50);
   disp.set(LED_0F[29], 0);
  delay(50);
  disp.clear() ;
     disp.set(LED_0F[30], 3);
  delay(50);
   disp.set(LED_0F[30], 2);
  delay(50);
   disp.set(LED_0F[30], 1);
  delay(50);
   disp.set(LED_0F[30], 0);
  delay(50);
  disp.clear() ;
       disp.set(LED_0F[31], 3);
  delay(50);
   disp.set(LED_0F[31], 2);
  delay(50);
   disp.set(LED_0F[31], 1);
  delay(50);
   disp.set(LED_0F[31], 0);
  delay(50); 
  disp.clear() ;
  }
void probegTest()
{
   int c=3;
    for( int x = 0; x<4; x++)
    {
  disp.set(LED_0F[28], (c));
  delay(50);
  c=c-1;
    }
    int c1=3;
    for( int x = 0; x<4; x++)
    {
  disp.set(LED_0F[29], (c1));
  delay(50);
  c1=c1-1;
    }
    int c2=3;
    for( int x = 0; x<4; x++)
    {
  disp.set(LED_0F[30], (c2));
  delay(50);
  c2=c2-1;
    }
    int c3=3;
    for( int x = 0; x<4; x++)
    {
   disp.set(LED_0F[31], (c3));
  delay(50); 
  c3=c3-1;
    }
}
//Вращение
void crug()
 {  
  int c=3;
    for( int x = 0; x<4; x++)
    {
  disp.set(LED_0F[(36)], (c));
  delay(50);
  disp.set(LED_0F[(37)], (c));
  delay(50);
  disp.set(LED_0F[(38)], (c));
  delay(50);
  disp.set(LED_0F[(39)], (c));
  delay(50);
  disp.set(LED_0F[(40)], (c));
  delay(50);
  disp.set(LED_0F[(41)], (c));
  delay(50);
  disp.set(LED_0F[(36)], (c));
  delay(50);
  c=c-1;
    }
  
  }
void crugif(){
  if (cr == 3){
    int c=3;
   disp.set(LED_0F[(36)], (c));
  delay(50);
  disp.set(LED_0F[(37)], (c));
  delay(50);
  disp.set(LED_0F[(38)], (c));
  delay(50);
  disp.set(LED_0F[(39)], (c));
  delay(50);
  disp.set(LED_0F[(40)], (c));
  delay(50);
  disp.set(LED_0F[(41)], (c));
  delay(50);
  disp.set(LED_0F[(36)], (c));
  delay(50); 
  }
  if (cr == 2){
    int c=2;
   disp.set(LED_0F[(36)], (c));
  delay(50);
  disp.set(LED_0F[(37)], (c));
  delay(50);
  disp.set(LED_0F[(38)], (c));
  delay(50);
  disp.set(LED_0F[(39)], (c));
  delay(50);
  disp.set(LED_0F[(40)], (c));
  delay(50);
  disp.set(LED_0F[(41)], (c));
  delay(50);
  disp.set(LED_0F[(36)], (c));
  delay(50); 
  }
  if (cr == 1){
    int c=1;
   disp.set(LED_0F[(36)], (c));
  delay(50);
  disp.set(LED_0F[(37)], (c));
  delay(50);
  disp.set(LED_0F[(38)], (c));
  delay(50);
  disp.set(LED_0F[(39)], (c));
  delay(50);
  disp.set(LED_0F[(40)], (c));
  delay(50);
  disp.set(LED_0F[(41)], (c));
  delay(50);
  disp.set(LED_0F[(36)], (c));
  delay(50); 
  }
  if (cr == 0){
    int c=0;
   disp.set(LED_0F[(36)], (c));
  delay(50);
  disp.set(LED_0F[(37)], (c));
  delay(50);
  disp.set(LED_0F[(38)], (c));
  delay(50);
  disp.set(LED_0F[(39)], (c));
  delay(50);
  disp.set(LED_0F[(40)], (c));
  delay(50);
  disp.set(LED_0F[(41)], (c));
  delay(50);
  disp.set(LED_0F[(36)], (c));
  delay(50); 
  }
}

//"Конфети"
void confeti(){
  //----____
for ( int x = 0; x<3; x++){
  
disp.set(LED_0F[28], 3);
delay(20);
disp.clear();
disp.set(LED_0F[29], 2);
delay(20);
disp.clear();
disp.set(LED_0F[30], 1);
delay(20);
disp.clear();
disp.set(LED_0F[31], 0);
delay(20);
disp.set(LED_0F[32], 3);
delay(20);
disp.clear();

disp.set(LED_0F[31], 3);
delay(20);
disp.clear();
disp.set(LED_0F[32], 2);
delay(20);
disp.clear();
disp.set(LED_0F[28], 1);
delay(20);
disp.clear();
disp.set(LED_0F[29], 0);
delay(20);
disp.clear();
disp.set(LED_0F[30], 3);
delay(20);
disp.clear();

disp.set(LED_0F[30], 3);
delay(20);
disp.clear();
disp.set(LED_0F[31], 2);
delay(20);
disp.clear();
disp.set(LED_0F[32], 1);
delay(20);
disp.clear();
disp.set(LED_0F[28], 0);
delay(20);
disp.clear();
disp.set(LED_0F[29], 3);
delay(20);
disp.clear();

disp.set(LED_0F[29], 3);
delay(20);
disp.clear();
disp.set(LED_0F[30], 2);
delay(20);
disp.clear();
disp.set(LED_0F[31], 1);
delay(20);
disp.clear();
disp.set(LED_0F[32], 0);
delay(20);
disp.clear();
disp.set(LED_0F[28], 3);
delay(20);
disp.clear();
}
}
void confetiTest(){
  long rn ;
  long rn1 ;
  long rn2 ;
  long rn3 ;
  long rn4 ;
  for ( int x = 0; x<5; x++)
  {
  rn  = random(28,32);
  rn1 = random(28,32);
  rn2 = random(28,32);
  rn3 = random(28,32);
  rn4 = random(28,32);
  
      disp.set(LED_0F[(rn)], 3);
      delay(20);
      disp.set(LED_0F[(rn1)], 2);
      delay(20);
      disp.set(LED_0F[(rn2)], 1);
      delay(20);
      disp.set(LED_0F[(rn3)], 0);
      delay(20);
      disp.set(LED_0F[(rn4)], 3);
      delay(20);
     
  }
}
void confetiif(){
if (cr == 3){
  for ( int x = 0; x<1; x++){
  int c=3;
disp.set(LED_0F[28], (c));
delay(20);
disp.set(LED_0F[29], (c));
delay(20);
disp.set(LED_0F[30], (c));
delay(20);
disp.set(LED_0F[31], (c));
delay(20);

disp.set(LED_0F[31], (c));
delay(20);
disp.set(LED_0F[28], (c));
delay(20);
disp.set(LED_0F[29], (c));
delay(20);
disp.set(LED_0F[30], (c));
delay(20);

disp.set(LED_0F[30], (c));
delay(20);
disp.set(LED_0F[31], (c));
delay(20);
disp.set(LED_0F[28], (c));
delay(20);
disp.set(LED_0F[29], (c));
delay(20);


disp.set(LED_0F[29], (c));
delay(20);
disp.set(LED_0F[30], (c));
delay(20);
disp.set(LED_0F[31], (c));
delay(20);
disp.set(LED_0F[28], (c));
delay(20);
}
}
if (cr == 2){
  for ( int x = 0; x<1; x++){
  int c=2;
disp.set(LED_0F[28], (c));
delay(20);
disp.set(LED_0F[29], (c));
delay(20);
disp.set(LED_0F[30], (c));
delay(20);
disp.set(LED_0F[31], (c));
delay(20);

disp.set(LED_0F[31], (c));
delay(20);
disp.set(LED_0F[28], (c));
delay(20);
disp.set(LED_0F[29], (c));
delay(20);
disp.set(LED_0F[30], (c));
delay(20);

disp.set(LED_0F[30], (c));
delay(20);
disp.set(LED_0F[31], (c));
delay(20);
disp.set(LED_0F[28], (c));
delay(20);
disp.set(LED_0F[29], (c));
delay(20);


disp.set(LED_0F[29], (c));
delay(20);
disp.set(LED_0F[30], (c));
delay(20);
disp.set(LED_0F[31], (c));
delay(20);
disp.set(LED_0F[28], (c));
delay(20);
}
}
if (cr == 1){
  for ( int x = 0; x<1; x++){
  int c=1;
disp.set(LED_0F[28], (c));
delay(20);
disp.set(LED_0F[29], (c));
delay(20);
disp.set(LED_0F[30], (c));
delay(20);
disp.set(LED_0F[31], (c));
delay(20);

disp.set(LED_0F[31], (c));
delay(20);
disp.set(LED_0F[28], (c));
delay(20);
disp.set(LED_0F[29], (c));
delay(20);
disp.set(LED_0F[30], (c));
delay(20);

disp.set(LED_0F[30], (c));
delay(20);
disp.set(LED_0F[31], (c));
delay(20);
disp.set(LED_0F[28], (c));
delay(20);
disp.set(LED_0F[29], (c));
delay(20);


disp.set(LED_0F[29], (c));
delay(20);
disp.set(LED_0F[30], (c));
delay(20);
disp.set(LED_0F[31], (c));
delay(20);
disp.set(LED_0F[28], (c));
delay(20);
}
}
if (cr == 0){
  for ( int x = 0; x<1; x++){
  int c=0;
disp.set(LED_0F[28], (c));
delay(20);
disp.set(LED_0F[29], (c));
delay(20);
disp.set(LED_0F[30], (c));
delay(20);
disp.set(LED_0F[31], (c));
delay(20);

disp.set(LED_0F[31], (c));
delay(20);
disp.set(LED_0F[28], (c));
delay(20);
disp.set(LED_0F[29], (c));
delay(20);
disp.set(LED_0F[30], (c));
delay(20);

disp.set(LED_0F[30], (c));
delay(20);
disp.set(LED_0F[31], (c));
delay(20);
disp.set(LED_0F[28], (c));
delay(20);
disp.set(LED_0F[29], (c));
delay(20);


disp.set(LED_0F[29], (c));
delay(20);
disp.set(LED_0F[30], (c));
delay(20);
disp.set(LED_0F[31], (c));
delay(20);
disp.set(LED_0F[28], (c));
delay(20);
}
}
}
//Герлянда
void gerlianda()
{
  long rn ;
  long rn1 ;
  long rn2 ;
  long rn3 ;
  for ( int x = 0; x<10; x++)
  {
    rn  = random(28,44);
    rn1 = random(28,44);
    rn2 = random(28,44);
    rn3 = random(28,44);
  
  
      disp.set(LED_0F[(rn)], 3);
      delay(30);
      disp.set(LED_0F[(rn1)], 2);
      delay(30);
      disp.set(LED_0F[(rn2)], 1);
      delay(30);
      disp.set(LED_0F[(rn3)], 0);
      delay(30);

   
 }
  
}
//Змейка на экране
void zmeika()
{
  long rn ;
  long rn1 ;
 rn = random(1, 4);
 rn1 = random(0, 1);
 Serial.println(rn);
 Serial.println(rn1);
 //rn=3;
// rn1 =0;
switch (rn){
  case 1 :
    disp.clear();
    disp.set(LED_0F[36], 3);
    delay(ZMEI);
    disp.set(LED_0F[36], 2);
    delay(ZMEI);
    disp.set(LED_0F[36], 1);
    delay(ZMEI);
    disp.set(LED_0F[36], 0);
    delay(ZMEI);
    disp.set(LED_0F[33], 3);
    delay(ZMEI);
    disp.set(LED_0F[60], 0);
    delay(ZMEI);
    disp.set(LED_0F[33], 2);
    delay(ZMEI);
    disp.set(LED_0F[61], 0);
    delay(ZMEI);
    disp.set(LED_0F[33], 1);
    delay(ZMEI);
    disp.set(LED_0F[62], 0);
    delay(ZMEI);
    disp.set(LED_0F[30], 1);
    delay(ZMEI);
    disp.set(LED_0F[63], 0);
    delay(ZMEI);
    disp.set(LED_0F[30], 2);
    delay(ZMEI);
    disp.set(LED_0F[64], 0);
    delay(ZMEI);
    disp.set(LED_0F[30], 3);
    delay(ZMEI);
    disp.set(LED_0F[30], 0);
    delay(ZMEI);
    disp.set(LED_0F[65], 3);
    delay(ZMEI);
    disp.set(LED_0F[33], 0);
    delay(ZMEI);
    disp.set(LED_0F[66], 3);
    delay(ZMEI);
    disp.set(LED_0F[33], 1);
    delay(ZMEI);
    disp.set(LED_0F[29], 2);
    delay(ZMEI);
    disp.set(LED_0F[67], 3);
    delay(ZMEI);
    disp.set(LED_0F[29], 1);
    delay(ZMEI);
    disp.set(LED_0F[29], 2);
    delay(ZMEI);
    disp.set(LED_0F[29], 3);
    delay(ZMEI);
    disp.set(LED_0F[29], 0);
    delay(ZMEI);
    disp.set(LED_0F[33], 3);
    delay(ZMEI);
    disp.set(LED_0F[33], 2);
    delay(ZMEI);
    disp.set(LED_0F[33], 1);
    delay(ZMEI);
    disp.set(LED_0F[33], 0);
    delay(ZMEI);
  break;

  
  case 2:
    disp.clear();
    disp.set(LED_0F[41], 3);
    delay(ZMEI);
    disp.set(LED_0F[68], 3);
    delay(ZMEI);
    disp.set(LED_0F[69], 3);
    delay(ZMEI);
    disp.set(LED_0F[28], 2);
    delay(ZMEI);
    disp.set(LED_0F[60], 2);
    delay(ZMEI);
    disp.set(LED_0F[70], 3);
    delay(ZMEI);
     disp.set(LED_0F[33], 3);
    delay(ZMEI);
    disp.set(LED_0F[71], 2);
    delay(ZMEI);
    disp.set(LED_0F[29], 1);
    delay(ZMEI);
    disp.set(LED_0F[70], 1);
    delay(ZMEI);
    disp.set(LED_0F[60], 2);
    delay(ZMEI);
    disp.set(LED_0F[28], 0);
    delay(ZMEI);
    disp.set(LED_0F[33], 2);
    delay(ZMEI);
    disp.set(LED_0F[69], 1);
    delay(ZMEI);
    disp.set(LED_0F[70], 1);
    delay(ZMEI);
    disp.set(LED_0F[60], 0);
    delay(ZMEI);
     disp.set(LED_0F[33], 1);
    delay(ZMEI);
    disp.set(LED_0F[72], 0);
    delay(ZMEI);
    disp.set(LED_0F[62], 0);
    delay(ZMEI);
    disp.set(LED_0F[73], 0);
    delay(ZMEI);
    disp.set(LED_0F[47], 0);
    delay(ZMEI);
    disp.set(LED_0F[29], 1);
    delay(ZMEI);
    disp.set(LED_0F[65], 0);
    delay(ZMEI);
    disp.set(LED_0F[67], 1);
    delay(ZMEI);
    disp.set(LED_0F[40], 0);
    delay(ZMEI);
    disp.set(LED_0F[30], 2);
    delay(ZMEI);
    disp.set(LED_0F[33], 0);
    delay(ZMEI);
    disp.set(LED_0F[65], 2);
    delay(ZMEI);
    disp.set(LED_0F[29], 3);
    delay(ZMEI);
    disp.set(LED_0F[40], 1);
    delay(ZMEI);
    disp.set(LED_0F[67], 3);
    delay(ZMEI);
    disp.set(LED_0F[33], 1);
    delay(ZMEI);
    disp.set(LED_0F[40], 2);
    delay(ZMEI);
    disp.set(LED_0F[33], 2);
    delay(ZMEI);
    disp.set(LED_0F[40], 3);
    delay(ZMEI);
    disp.set(LED_0F[33], 3);
    delay(ZMEI);
    delay(500);
   break;

  
 
    case 3:
    disp.clear();
    disp.set(LED_0F[36], 3);
    disp.set(LED_0F[39], 0);
    delay(ZMEI);
    disp.set(LED_0F[36], 2);
    disp.set(LED_0F[39], 1);
    delay(ZMEI);
    disp.set(LED_0F[31], 2);
    disp.set(LED_0F[31], 1);
    delay(ZMEI);
    disp.set(LED_0F[31], 0);
    disp.set(LED_0F[31], 3);
    delay(ZMEI);
  for( int x = 0; x<2; x++)
    {
    disp.set(LED_0F[60], 0);
    disp.set(LED_0F[65], 3);
    delay(ZMEI);
    disp.set(LED_0F[61], 0);
    disp.set(LED_0F[28], 1);
    disp.set(LED_0F[30], 2);
    disp.set(LED_0F[74], 3);
    delay(ZMEI);
    disp.set(LED_0F[62], 0); // C
    disp.set(LED_0F[33], 1);
    disp.set(LED_0F[33], 2);
    disp.set(LED_0F[12], 3); // C
    delay(ZMEI);
    disp.set(LED_0F[63], 0);
    disp.set(LED_0F[30], 1);
    disp.set(LED_0F[28], 2);
    disp.set(LED_0F[75], 3);
    delay(ZMEI);
    disp.set(LED_0F[64], 0);
    disp.set(LED_0F[31], 1);
    disp.set(LED_0F[31], 2);
    disp.set(LED_0F[76], 3);
    delay(ZMEI);
    disp.set(LED_0F[31], 0);
    disp.set(LED_0F[31], 1);
    disp.set(LED_0F[31], 2);
    disp.set(LED_0F[31], 3);
    delay(ZMEI);
    }
    
    disp.set(LED_0F[60], 0);
    disp.set(LED_0F[31], 1);
    disp.set(LED_0F[31], 2);
    disp.set(LED_0F[65], 3);
    delay(ZMEI);
    disp.set(LED_0F[61], 0);
    disp.set(LED_0F[28], 1);
    disp.set(LED_0F[30], 2);
    disp.set(LED_0F[74], 3);
    delay(ZMEI);
    disp.set(LED_0F[61], 0);
    disp.set(LED_0F[33], 1);
    disp.set(LED_0F[33], 2);
    disp.set(LED_0F[74], 3);
    delay(ZMEI);
    disp.set(LED_0F[1], 0);
    disp.set(LED_0F[33], 1);
    disp.set(LED_0F[33], 2);
    disp.set(LED_0F[53], 3);
    delay(ZMEI);
    disp.set(LED_0F[38], 0);
    disp.set(LED_0F[33], 1);
    disp.set(LED_0F[33], 2);
    disp.set(LED_0F[41], 3);
    delay(ZMEI);
    disp.set(LED_0F[33], 0);
    disp.set(LED_0F[33], 1);
    disp.set(LED_0F[33], 2);
    disp.set(LED_0F[33], 3);
    delay(ZMEI);
    disp.clear();
  break;
  
   case 4:
    //бробежка ПО СТРОЧКАМ 3 ЗМЕЙКИ
    disp.clear();
    disp.set(LED_0F[30], 3);
    disp.set(LED_0F[39], 0);
    delay(ZMEI); 
  break;

  //ПРОБЕЖКА ДРУГ ЗА ДРУГОМ 
  //ПРОБЕЖКА ВЕРТИКАЛЬНАЯ ПО СЕГМЕНТАМ 8 СТОЛБЦОВ 
 case 5:
    //ЗИГ-ЗАГ
break;

}
}
  
  
// Дождик
 void dogdik()
 {
  long rn ;
  long rn1 ;
  long rn2 ;
  long rn3 ;
  long crek;
  long time_r;
  for ( int x = 0; x<20; x++)
  {
   crek = random(0,4);
switch (crek) {
   case 1:
    rn  = random(50,57);
    rn1 = random(36,44);
    rn2 = random(50,57);
    rn3 = random(36,44);
  break;
   case 2:
    rn  = random(36,44);
    rn1 = random(50,57);
    rn2 = random(36,44);
    rn3 = random(50,57);
  break;
  case 3:
  rn  = random(50,57);
  rn1 = random(50,57);
  rn2 = random(50,57);
  rn3 = random(50,57);
  break;
  case 4:
    rn  = random(36,44);
    rn1 = random(36,44);
    rn2 = random(36,44);
    rn3 = random(36,44);
break;
}
time_r = random(50,150);

      disp.set(LED_0F[(rn)], 3);
      delay(time_r);
      disp.set(LED_0F[(rn1)], 2);
      delay(time_r);
      disp.set(LED_0F[(rn2)], 1);
      delay(time_r);
      disp.set(LED_0F[(rn3)], 0);
      delay(time_r);
  
 }
 }

void anima(){
  long rn ;
   rn  = random(1,8);
 // Serial.println(rn);
switch (rn){
   case 1:
    probeg();
   break;
   case 2:
    probegTest();
   break;
   case 3:
    confeti();
   break;
   case 4:
    confetiTest();
   break;
   case 5:
    gerlianda();
   break;
  case 6:
    zmeika();
   break;
    case 7:
    dogdik();
   break;
}
}
/******************************************************ФУНКЦИИ ТЕСТИРОВАНИЯ********************
 * Функция выводит на экран в каждый сегмент все возможные символы и воспроизводит все анимации.
 * Для проверки правильности подключения экрана лучше всего при первой прошивке включить эту функцию, 
 * а в последствие отключить при повторной прошивке, так как она замедляет запуск часов.
 */
  void test(){
    int x = 0;
    for( int x = 0; x<COLBIB; x++)
    {
  disp.set(LED_0F[(x)], 3);
  disp.set(LED_0F[(x)], 2);
  disp.set(LED_0F[(x)], 1);
  disp.set(LED_0F[(x)], 0);
  delay(TEST_T);
    }
  probeg();
  crug();
  confetiTest();
  crug();
  confeti();
  probegTest();
  dogdik();
  zmeika();
  gerlianda();
  para ();
  loading ();
  datadisp();
  }

 
