#include <SD.h>
#include <Wire.h>
#include <RTClib.h>

const int mq2Pin = A15;
const int greenLedPin = 47;
const int redLedPin = 46;
const int buzzerPin = 49;
const int chipSelect = 53;

// Inicializando objetos
RTC_DS3231 rtc;

void setup(){

  Serial.begin(9600);

  if (!SD.begin(chipSelect)){
    Serial.println("Falha ao inicializar o cartão SD!");
    while (true)
      ;
  }

  Serial.println("Cartão SD inicializado.");

  if (!rtc.begin()){
    Serial.println("Não foi possível encontrar o RTC. Verifique a conexão!");
    while (true)
      ;
  }

  if (rtc.lostPower()){
    Serial.println("RTC perdeu energia. Definindo a hora!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pinMode(mq2Pin, INPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(greenLedPin, HIGH);
  digitalWrite(redLedPin, LOW);
  noTone(buzzerPin);
}

void loop(){

  int mq2Value = analogRead(mq2Pin);

  if (mq2Value > 300){

    digitalWrite(greenLedPin, LOW);

    digitalWrite(redLedPin, HIGH);
    tone(buzzerPin, 404);

    DateTime now = rtc.now();

    String timestamp = String(now.year()) + "-" +
                       zeroFill(now.month()) + "-" +
                       zeroFill(now.day()) + " " +
                       zeroFill(now.hour()) + ":" +
                       zeroFill(now.minute()) + ":" +
                       zeroFill(now.second());

    writeFile("log.txt", "Detecção de gás em: " + timestamp);

    delay(5000);

    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
    noTone(buzzerPin);
  }

  plotData(mq2Value, digitalRead(redLedPin), digitalRead(greenLedPin), 300);

  delay(500);
}

void writeFile(const char *filename, String data){

  File file = SD.open(filename, FILE_WRITE);

  if (file){
    file.println(data);
    file.close();
    Serial.println("Dados gravados com sucesso.");
  }
  else{
    Serial.println("Erro ao abrir o arquivo.");
  }
}

String zeroFill(int number){

  return (number < 10) ? "0" + String(number) : String(number);
}

void plotData(int sensorValue, int redLedState, int greenLedState, int limit){
  Serial.print("Sensor: ");
  Serial.print(sensorValue);
  Serial.print(", Red LED: ");
  Serial.print(redLedState);
  Serial.print(", Green LED: ");
  Serial.print(greenLedState);
  Serial.print(", Limite: ");
  Serial.println(limit);
}
