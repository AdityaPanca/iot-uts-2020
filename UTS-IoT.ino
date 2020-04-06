#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>



LiquidCrystal_I2C lcd(0x27, 16, 2);


#define SS_PIN 2  //D2
#define RST_PIN 0 //D1
#define sensorLDR A0
#define LED 16
 
 int sensorSuhu = 3;
 int nilaiSensor;
 dht DHT; 


MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

void setup()
{
  Serial.begin(115200); // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  lcd.init();
  lcd.backlight();
  lcd.home();
  pinMode(LED, OUTPUT); 
  pinMode(LED, OUTPUT);
}

void loop()
{
  lcd.home();
  Serial.println("Waiting card...");
  digitalWrite(LED, HIGH);
  delay(1000);
  DHT.read11(sensorSuhu);
   nilaiSensor = analogRead(sensorLDR);
   Serial.print("Nilai Sensor : ");
   Serial.println(nilaiSensor); 
   Serial.println("Suhu : "); 
   Serial.print(DHT.temperature); 
  
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  delay(500);
  
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "A9 7B 99 98" && nilaiSensor <= 500 && DHT.temperature <= 28) //change here the UID of the card/cards that you want to give access
  { 
    Serial.println("Authorized access");
    Serial.println();   
    lcd.print("Silahkan Masuk");
    digitalWrite(LED, LOW);  
    delay(3000);
    lcd.clear();
    delay(200);
  }
 
 else{
   Serial.println(" Access denied");
   lcd.print("Dilarang Masuk");
   digitalWrite(LED, HIGH);
   delay(3000);
   lcd.clear();
   delay(200);
  }
}



// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void scrollText(int row, String message, int delayTime, int lcdColumns)
{
  for (int i = 0; i < lcdColumns; i++)
  {
    message = " " + message;
  }
  message = message + " ";
  for (int pos = 0; pos < message.length(); pos++)
  {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}
