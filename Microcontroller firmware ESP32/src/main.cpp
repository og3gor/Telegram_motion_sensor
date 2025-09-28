#include <Arduino.h>
#include <stdio.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WIFI.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "Picture.h"
#include "CalibriFont.h"
#include "config.h"
#include "EEManager.h"

struct Data {
    char WIFI_SSID[15] = "hidden";
    char WIFI_PASSWORD[20] = "hidden";

    char BOT_TOKEN[50] = "hidden";
    char CHAT_ID[15] = "hidden";
};

Data startData;
Data data;
// Передаём переменную в менеджер для выделения ей памяти и тайминг обновления данных
EEManager memory(data,2);

#define Calibri_Font_Medium CalibriFont21

// Инициализация дисплея 
TFT_eSPI tft = TFT_eSPI();  

// Инициализация датчика
Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

const unsigned long BOT_MTBS = 1000; // Время сканирования чатов ботом

WiFiClientSecure secured_client;
UniversalTelegramBot bot(data.BOT_TOKEN, secured_client);
unsigned long bot_lasttime;          // last time messages' scan has been done
bool Start = false;

// Таймер задержки для датчика движения (с опросом в 5 секунд)
unsigned long lastTrigger = 0;
unsigned long lastTriggerSend = 0;

// Таймер задержки обновления темпретуры на экране
unsigned long lastTriggerTemp = 0;

// Тригеры для вкл\выкл функций
boolean sendMotion = false;

void handleNewMessages(int numNewMessages)
{
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Гость";

    if (text == "/motion_sensor_on")
    {
      bot.sendMessage(chat_id, "Отслеживание движений включено!");
      sendMotion = true;
    }

    if (text == "/motion_sensor_off")
    {
      bot.sendMessage(chat_id, "Отслеживание движений отключено!");
      sendMotion = false;
    }

    if (text == "/temp")
    {
      bot.sendChatAction(chat_id, "typing");
      String temp = "Текущая температура: "; 
      temp += bmp.readTemperature();
      temp += " *C";
      bot.sendMessage(chat_id, temp);
    }

    if (text == "/restart")
    {
      bot.sendMessage(chat_id, "Перезагрузка устройства!");
      ESP.restart();
    }



    if (text == "/start")
    {
      String welcome = "Здравствуте, " + from_name + ".\n";
      welcome += "Это крусовой проект Худкова Егор из К3-56Б под названием: Telegram датчик движения.\n\n";
      welcome += "Официальное название курсовой работы: \"Разработка программы прошивки микроконтроллера прибора обнаружения движения в помещение\"\n";
      welcome += "Команды для управлением устройством:\n";
      welcome += "/restart - Перезагрузка ESP32\n";
      welcome += "/temp - Получение текущей информации о температуре\n";
      welcome += "/motion_sensor_on - Включение сенсора движения\n";
      welcome += "/motion_sensor_off - Выключение сенсора движения";
      bot.sendMessage(chat_id, welcome);
    }
  }
}

void setup() {
  Serial.begin(115200);
// Инициализация памяти
EEPROM.begin(memory.blockSize());
// Запускаем менеджер с параметрами адреса и ключа запуска
memory.begin(0,'p'); 

// Инициализачия дисплея
  tft.begin();
  tft.setRotation(1); 
  tft.fillScreen(TFT_WHITE);
  tft.setSwapBytes(true); // Порядок передачи байтов в дисплей
  tft.pushImage(0,0,240,135,lestex_logo);
  delay(1500);
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_BLACK);
  tft.loadFont(Calibri_Font_Medium);
  tft.println("Здравствуйте!");
  tft.unloadFont();

// Инициализация датчика температуры
  bmp.begin(0x76);
  /* Настройки датчика по умолчанию */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Режим работы. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Темпрература. Подсчёт. */
                  Adafruit_BMP280::SAMPLING_X16,    /* Избыточная дискретизация давления. */
                  Adafruit_BMP280::FILTER_X16,      /* Фильтрация. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Время ожидания. */

  bmp_temp->printSensorDetails(); // Вывод тест данных в Serial

// Подклюсчение к интернету и к телеграм боту
  tft.loadFont(Calibri_Font_Medium);
  tft.print("Подключение к "); tft.println(data.WIFI_SSID);
  WiFi.begin(data.WIFI_SSID, data.WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    if (i < 20) {
    delay(500);
    i++;
    } else { 
      tft.setTextColor(TFT_RED);
      tft.println("Нет интернета!");
      tft.setTextColor(TFT_BLACK);
      tft.println("Для настройки,\nподключите\nустройство к ПК");
      // Алгоритм работы с ПК
      // Цикл, в течение 15 секунд можно запрограммировать устройство
      Serial.begin(115200);
      EEPROM.begin(memory.blockSize());
      memory.begin(0,'p'); //удали проверку на инициализацию чтение 
      

      int varCounter = 0;
      int counter = 0;
      unsigned int bootloadTriger = millis();
      
      while (millis() - bootloadTriger < (15*1000)) {
        if (Serial.read() == '\'') {
          tft.fillScreen(TFT_WHITE);
          tft.setCursor(0, 0);
          tft.println("Режим настройки.");
          Serial.println(" Режим настройки. ");
          Serial.println(" Текущие параметры устройства: ");
          Serial.print(" Логин WiFi = ");
          Serial.println(data.WIFI_SSID);
          Serial.print(" Пароль Wi-Fi = ");
          Serial.println(data.WIFI_PASSWORD);
          Serial.print(" Telegram токен = ");
          Serial.println(data.BOT_TOKEN);
          Serial.print(" Чат id = ");
          Serial.println(data.CHAT_ID);

          memset(&data, 0, sizeof(data));
          char buffer;
          
          for (;;) {
            if (Serial.available()) {
            buffer = Serial.read();
            if (buffer == '\n') {
              data = startData;
              Serial.println(" Применены настройки по умолчанию. ");
              break;
            } else if (buffer == '\t') {
              break;
            } else if (buffer == '\r') {
              counter = 0;
              varCounter++;
            } else {
            switch (varCounter) {
                case 0:
                  data.WIFI_SSID[counter] = buffer;
                  break;         
                case 1:
                  data.WIFI_PASSWORD[counter] = buffer;
                  break;
                case 2:
                  data.BOT_TOKEN[counter] = buffer;  
                  break;
                case 3:
                  data.CHAT_ID[counter] = buffer;
                  break;
                }
            counter++;
              }
            }

          }
          memory.update();

          Serial.println(" Изменённые параметры: ");
          Serial.print(" Логин WiFi = ");
          Serial.println(data.WIFI_SSID);
          Serial.print("Пароль Wi-Fi = ");
          Serial.println(data.WIFI_PASSWORD);
          Serial.print(" Telegram токен = ");
          Serial.println(data.BOT_TOKEN);
          Serial.print("Чат id = ");
          Serial.println(data.CHAT_ID);
          Serial.println("Выход из режима настройки.");
        }
      }
      memory.tick();memory.tick();memory.tick();
      delay(100);
      ESP.restart();
    }
  }
  tft.print("Подкдючение успешно!\nip = ");
  tft.println(WiFi.localIP());

  tft.print("Время подключения\nк сети: ");
  configTime(0, 0, "pool.ntp.org"); // Получение время UTC через NTP
  while (time(nullptr) < 24 * 3600) {
    delay(100);
  }

// В случае если время не получиться получить - вывести ошибку и перезагрузить устройство
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    tft.println("ОШИБКА ВРЕМЕНИ");
    ESP.restart();
    return;
  }
  timeinfo.tm_hour += 3;
  char timeString[20];
  strftime(timeString, sizeof(timeString), "%H:%M", &timeinfo);
  tft.println(timeString);

  tft.printf("Текущая темп.: %.2f *C", bmp.readTemperature());

  bot.sendMessage(data.CHAT_ID,"Устройство запущено и готово к работе."); 

//Инициализация датчика движения 
  pinMode(motionSensorPin, INPUT);  // Установка пина датчика на вход
}

unsigned int timeSecondsMotion = 1;
unsigned int timeSecondsMotionSend = 10;

unsigned int timeSecondsTemperatureReading = 3;

void loop() {
// Проверка интернет соединения
if (WiFi.status() != WL_CONNECTED) {
    tft.fillScreen(TFT_WHITE);
    tft.setCursor(0, 0);
    tft.println("Нет интернета!");
    tft.println("Перезагрузка через");
    tft.println("5 секунд");
    delay(5000);
    ESP.restart();
}

// Алгорим отображения термометра на экран устройства
if (millis() - lastTriggerTemp > (timeSecondsTemperatureReading*1000)) {
tft.pushImage(0,0,240,125,lestex_logo);
tft.setCursor(0, 0);
tft.printf("Текущая темп.: %.2f *C", bmp.readTemperature());
lastTriggerTemp = millis();
}

// Алгоритм сичтывание движений
int motionDetected = digitalRead(motionSensorPin);
if (millis() - lastTrigger > (timeSecondsMotion*1000)) {
        if(motionDetected == HIGH){
            //Serial.println("Motion Detected");
            tft.fillRect(0, 125, 240, 10, TFT_RED);
            Serial.println(bmp.readTemperature());
            if (millis() - lastTriggerSend > (timeSecondsMotionSend*1000) && sendMotion == true) {
              bot.sendMessage(data.CHAT_ID, "Обнаруженно движение!");
              lastTriggerSend = millis();
            }
            timeSecondsMotionSend = 30;
        } else {
        tft.fillRect(0, 125, 240, 10, TFT_GREEN);
        //Serial.println("No motion."); //элем отладки 
        timeSecondsMotionSend = 10;
        }    
    lastTrigger = millis();
}

// Алгоритм считывание отправленных устройству сообщений
if (millis() - bot_lasttime > BOT_MTBS)
          {
            int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
            while (numNewMessages)
            {
              handleNewMessages(numNewMessages);
              numNewMessages = bot.getUpdates(bot.last_message_received + 1);
            }
            bot_lasttime = millis();
          }
}



