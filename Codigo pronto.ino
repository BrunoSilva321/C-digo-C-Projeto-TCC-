#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MQ135.h>

// Defina os pinos para os sensores e o relé
#define DHT_PIN 7
#define DHT_TYPE DHT22
#define MQ135_PIN A0
#define RELE_PIN 8

// Crie objetos para os sensores
DHT dht(DHT_PIN, DHT_TYPE);
MQ135 mq135(MQ135_PIN);

// Crie um objeto para o display I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço I2C 0x27, 16 colunas e 2 linhas

void setup() {
  // Inicialize o sensor DHT22, o sensor MQ135 e o display I2C
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Qualidade do Ar:");
  
  // Configure o pino do relé como saída
  pinMode(RELE_PIN, OUTPUT);
  // Desligue o umidificador inicialmente
  digitalWrite(RELE_PIN, LOW);
}

void loop() {
  // Leia a temperatura e umidade do sensor DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Leia a concentração de CO2 do sensor MQ135
  float co2ppm = mq135.getPPM();

  // Exiba os dados no display I2C
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print("C U:");
  lcd.print(humidity);
  lcd.print("%");

  lcd.setCursor(0, 0);
  lcd.print("CO2: ");
  lcd.print(co2ppm);
  lcd.print(" ppm    ");

  // Verifique a umidade e controle o umidificador com base nas condições
  if (humidity < 65) {
    // A umidade está abaixo de 65, ligue o umidificador
    digitalWrite(RELE_PIN, LOW);
    Serial.println("Umidificador ligado");
  } else if (humidity > 70) {
    // A umidade está acima de 70, desligue o umidificador
    digitalWrite(RELE_PIN, HIGH);
    Serial.println("Umidificador desligado");
  }

  delay(2000); // Espere por 2 segundos antes de ler novamente
}