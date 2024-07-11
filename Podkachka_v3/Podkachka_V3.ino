#include <EEPROM.h>
#include "GyverTimer.h"
#include <SoftwareSerial.h>
#include <EncButton.h>
#include <HT1621.h>  // include our library
HT1621 lcd;          // create an "lcd" object

#define i_pressure 3    //PIN подключения датчика давления
#define red_led 9       //PIN подключения красного светодиода
#define green_led 10    //PIN подключения зеленого светодиода
#define LCD_CS_PIN 6    //PIN подключения экрана
#define LCD_WR_PIN 7    //PIN подключения экрана
#define LCD_DATA_PIN 8  //PIN подключения экрана
#define rele_plus 2     //PIN подключения реле 1
#define rele_minus 3     //PIN подключения реле 2
EncButton enc(5, 4, 11);  //PIN подключения енкодера

// Таймеры
GTimer Timer_izm(MS, 250);      // Время измерения давления
GTimer Timer_disp(MS, 250);     // Время обновления дисплея
GTimer Timer_Led(MS, 750);      // Время мигания светодиодом
GTimer Timer_plus(MS, 500);     // Время накачки шины
GTimer Timer_pause(MS, 5500);   // Время задержки измерения давления
GTimer Timer_stop(MS, 180000);  // Время отключения программы


float typVbg;                   //Переменная для измерения напряжения питания
float pressure;                 //Переменная для измерения давления
float sumP;                     //Переменная для измерения напряжения питания
float Ustavka;                  //Переменная для хранения уставки

int Status = 0;

bool st_green;
bool st_red;


void setup() {
  // Инициализация порта
  Serial.begin(115200);

  // Инициализация дисплея HT1621 
  lcd.begin(LCD_CS_PIN, LCD_WR_PIN, LCD_DATA_PIN);  // (cs, wr, Data, backlight)
  // if no backlight control is given, you can also use:
  // lcd.begin(13, 12, 8); // (cs, wr, Data)
  lcd.backlight();  // turn on the backlight led
  lcd.clear();      // clear the screen
                    // *********************************************************************************

  // Инициализация выводов
  pinMode(rele_plus, OUTPUT);
  pinMode(rele_minus, OUTPUT);
  digitalWrite(rele_plus, HIGH);
  digitalWrite(rele_minus, HIGH);
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  digitalWrite(red_led, HIGH);
  digitalWrite(green_led, st_green = LOW);
  
  Timer_plus.stop();
  Timer_pause.stop();
  Ustavka = eeprom_read_float(1);
}


// ***************************************************************************************************************************************************
// Главный цикл программы
// ***************************************************************************************************************************************************
void loop() {

  if (Status == 0) {
    enc.tick();
    if (enc.click()) {
      Serial.println("start");
      Start();
    }

    if (enc.left()) {
      if (Ustavka > 0.5) {
        Ustavka = Ustavka - 0.1;
      } else Ustavka = 0.5;
      eeprom_write_float(1, Ustavka);
    }

    if (enc.right()) {
      if (Ustavka < 8.0) {
        Ustavka = Ustavka + 0.1;
        //EEPROM.write(1, Ustavka);
        
      } else Ustavka = 8.0;
    eeprom_write_float(1, Ustavka);
    }
  }

/*
  // Button Stop
  if (Status != 0) {
    enc.tick();
    if (enc.hold()) {
      Status = 0;
    }
  }
*/
  // Режим работы в зависимости от статуса
  if (Status == 0) Stop();
  if (Status == 1) Start_plus();
  if (Status == 2) Start_minus();
  if (Status == 3) Start_test();

  //Отключение по времени
  if (Timer_stop.isReady()) Stop();

  // Измерение давления
  //if (Status != 0) {
  if (Timer_izm.isReady()) Izm();
  //}

  // Вывод на дисплей
  if (Timer_disp.isReady()) Display();

  // Мигание красным светодиодом
  if (Timer_Led.isReady()) {
    if (Status != 0) digitalWrite(red_led, st_red = !st_red);
  }

  // Выключение зеленого светодиода
  if (Status != 0) digitalWrite(green_led, LOW);
}
