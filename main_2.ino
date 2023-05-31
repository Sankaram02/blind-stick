#define BLYNK_TEMPLATE_ID "TMPL2AXXpIDd"
#define BLYNK_TEMPLATE_NAME "BLIND STICK"
#define BLYNK_AUTH_TOKEN "W1L1FVu1yMyT2RIlk91PnD5eN7Mg__oN"
#define BLYNK_PRINT Serial 

#include <WiFi.h> 
#include <WiFiClient.h> 
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h> 
#include <LiquidCrystal_I2C.h>

char auth[] = BLYNK_AUTH_TOKEN; 
char ssid[] = "SANKAR"; 
char pass[] = "00000000"; 
String serverName = "https://maker.ifttt.com/trigger/SOS_PRESSED/with/key/dGeiTBzoJH0voFz-ZCl_cB";
LiquidCrystal_I2C lcd (0x27, 16,2); 

BlynkTimer timer; 

WidgetLED LEDdanger(V0);

const int trigPin = 4;
const int echoPin = 15;
const int buzzer = 5;
const int pushbut = 18;
const int led = 19;
const int touch= 13;
const int vib =23;
// SCL OF I2C 22
//SDA OF I2C 21
const int mqan=12;
const int mqdi=14;

int blynkbuzzer;
int flag=0;
int flag2=0;
int distance;
int safetyDistance;
int custom_msg=0;


int x = 0;
bool text = false;
String textIn = "";
String text_speed = "";

BLYNK_WRITE(V3)
{
  textIn = param.asStr();
  if (textIn) // working for text Input Widget
  { 
    lcd.clear();
    text = true;
    x = 0;
    Serial2.print(String("!") + textIn + String("\r\n"));
    Serial.print(String("!") + textIn + String("\r\n"));
    text = false;
    Serial2.flush();
  }
}


BLYNK_WRITE(V1) 
{ 
  blynkbuzzer=param.asInt(); 
  
} 
BLYNK_WRITE(V2) 
{ 
  safetyDistance=param.asInt(); 
} 
BLYNK_WRITE(V4) 
{ 
  custom_msg=param.asInt(); 
} 


void setup()
{   
    Serial.begin(115200); 
    Serial2.begin(115200, SERIAL_8N1, 16, 17);
    lcd. begin ();
    Blynk.begin(auth, ssid, pass); 
    pinMode(trigPin, OUTPUT);     
    pinMode(echoPin, INPUT); 
    pinMode(vib, OUTPUT); 
    pinMode(buzzer, OUTPUT);    
    pinMode(pushbut, INPUT);
    pinMode(touch, INPUT);
    pinMode(mqan, INPUT);
    pinMode(mqdi, INPUT);
    pinMode(led, OUTPUT); 
    pinMode(2, OUTPUT); 
    LEDdanger.off();  
    digitalWrite(buzzer, HIGH);
    digitalWrite(2,1);
    delay(500);
    digitalWrite(buzzer, LOW);
    digitalWrite(2,0);
    safetyDistance=50;
    lcd. print ( " E STICK " );

    
}
void loop() 
{   

   // lcd.clear();
   lcd. print ( " E STICK " );     
    HTTPClient http;
    Blynk.run(); 
    //lcd.clear();
    //lcd. setCursor (0, 1);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);     
    digitalWrite(trigPin, HIGH);   
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);    
    int duration = pulseIn(echoPin, HIGH);
    distance= duration*0.034/2; 
       
  
    if ((distance<=safetyDistance)&&(digitalRead(touch)==1))   
    {
        // digitalWrite(buzzer, HIGH); 
        // digitalWrite(vib, HIGH); 
         flag2=1;    
    }
    else    
    {
        //digitalWrite(buzzer, LOW);   
       // digitalWrite(vib, LOW); 
        flag2=0;   
    }
    Serial.print("Distance: "); 
    Serial.println(analogRead(mqan));
     Serial.println(digitalRead(mqdi));
    
    if(digitalRead(pushbut)==1) // used to send msg to telegram bot when button pressed 
      {       
        //digitalWrite(led,1);
         

        String serverPath = serverName ;
        http.begin(serverPath.c_str());
        int httpResponseCode = http.GET();
        flag=1;
        delayMicroseconds(10);
      }      
      else
      {       
        digitalWrite(led,0);
      } 
        
      if(flag==1)  
      {
        LEDdanger.on();
      } 
      else
      {
        LEDdanger.off();
        }
        
      if((blynkbuzzer==1)||(flag2==1))
      {
        digitalWrite(buzzer,1);
        digitalWrite(vib, HIGH);
        digitalWrite(2,1);
        delay(300);
      } 
       else
      {
         digitalWrite(buzzer,0);
          digitalWrite(vib, 0);
         digitalWrite(2,0);
      } 
      if(custom_msg==1)
      {
        
         lcd. print ( " E STICK " );
         lcd.clear();
         lcd. setCursor (0, 0);
         lcd.print(String("!") + textIn + String("\r\n"));
         lcd.scrollDisplayLeft();
         delay(300);
      }  
      else
      {
         //lcd.clear();
         lcd. print ( "________E STICK________ " );
         lcd. setCursor (0, 1);
         
         lcd.print("name of the patient age gender locality ");
         lcd.scrollDisplayLeft();
         delay(300);
      } 
      int mq=analogRead(mqan);
      Blynk.virtualWrite(V7,mq);
}



//lcd.print(String("!") + textIn + String("\r\n"));
