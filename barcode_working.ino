#include <hidboot.h>
#include <hiduniversal.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

#define sensor 4
#define led 5

  
String DataBarcode;
String data = "";
bool x = false;
int inByte = 0; // for incoming serial data


class KbdRptParser : public KeyboardReportParser
{
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown  (uint8_t mod, uint8_t key);
    void OnKeyUp  (uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
  MODIFIERKEYS mod;
  *((uint8_t*)&mod) = m;
}

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  PrintKey(mod, key);
  uint8_t c = OemToAscii(mod, key);

  if (c)
    OnKeyPressed(c);
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {

  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  //Serial.print("UP ");
  //PrintKey(mod, key);
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
  //Serial.print((char)key);
  if (key == 0x0D){
    x = true;
  }else{
    DataBarcode += (char)key;
  }
}


USB Usb;
HIDUniversal Hid(&Usb);

KbdRptParser Prs;

void setup()
{
  
  Serial.begin( 115200 );
  mySerial.begin (115200);

  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay( 200 );

  Hid.SetReportParser(0, &Prs);

  //code for motion sensor
  pinMode(sensor, INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(sensor, LOW);
}

void loop()
{
  Usb.Task();
  if (x){
    //String barcodeValue = ""+DataBarcode;
    Serial.print(DataBarcode);
    //mySerial.print (DataBarcode);
    //Serial.write(';');
    mySerial.print(';');

    x = false;
    DataBarcode = "";
  }
  
  if(digitalRead(sensor)){
    //Serial.println("Movement Detected");
    digitalWrite(led, HIGH);

  }
  else{
    //Serial.println("No Movement Detected");
    digitalWrite(led, LOW);
  }
  
}
