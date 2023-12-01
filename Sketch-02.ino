#include <LiquidCrystal.h>

#define PIN_BUTTONS A0
#define PIN_SENSOR_MQ2 A15
#define PIN_BUZZER 49

int limit = 300;
bool buzzerOn = false;

unsigned long lastUpdateTime = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int sensorMQ2();
void buttonPressed();
void display();
void ringAlarm();

void setup()
{
   pinMode(PIN_BUTTONS, INPUT);
   pinMode(PIN_BUZZER, OUTPUT);
   digitalWrite(PIN_BUZZER, LOW);

   lcd.begin(16, 2);
   Serial.begin(9600);

   lcd.setCursor(14, 0);
   lcd.print(buzzerOn ? "1" : "0");
}

void loop()
{
   int buttonState = analogRead(PIN_BUTTONS);

   if (buttonState < 600)
   {

      buttonPressed();
   }

   if (millis() - lastUpdateTime >= 500)
   {
      display();
   }

   if (sensorMQ2() > limit && buzzerOn)
   {
      ringAlarm();
   }
}

int sensorMQ2()
{
   return analogRead(PIN_SENSOR_MQ2);
}

void buttonPressed()
{
   buzzerOn = !buzzerOn;
   lcd.clear();
   lcd.setCursor(14, 0);
   lcd.print(buzzerOn ? "1" : "0");
}

void display()
{
   int sensorValue = sensorMQ2();

   lcd.setCursor(0, 0);
   lcd.print("MQ-2: ");
   lcd.print(sensorValue);
   lcd.print(" ");
   lastUpdateTime = millis();

   lcd.setCursor(0, 1);
   lcd.print("Status: ");

   if (sensorMQ2() < limit)
      lcd.print("OK    ");
   else
      lcd.print("ALERTA");
}

void ringAlarm()
{
   tone(PIN_BUZZER, 404);
   delay(200);
   noTone(PIN_BUZZER);
}
