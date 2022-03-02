#include <ESP8266WiFi.h>
#include <Firebase.h>  
#include <FirebaseArduino.h>  
#include <FirebaseCloudMessaging.h>  
#include <FirebaseError.h>  
#include <FirebaseHttpClient.h>  
#include <FirebaseObject.h>    
#include <SPI.h>                          
#include <MFRC522.h> 

int greenPin=0;
int redPin=2;
int RST_PIN = 5;                          
int SS_PIN = 4;  

MFRC522 rfid(SS_PIN, RST_PIN);            
 
 
#define FIREBASE_HOST "deneme2-dd869-default-rtdb.firebaseio.com"  
#define FIREBASE_AUTH "nJElVxEXnlhNqC7wWGG5COG8hM8ip9NEH2DlFhNy"  
#define WIFI_SSID "Gelmez17"  
#define WIFI_PASSWORD "istanbulcanakkale"  
  
void setup() {  
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  Serial.begin(9600); 
  SPI.begin();                            
  rfid.PCD_Init(); 
  
  // connect to wifi.  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  
  Serial.print("connecting");  
  while (WiFi.status() != WL_CONNECTED) {  
    Serial.print(".");  
    delay(500);  
  }  
  Serial.println();  
  Serial.print("connected: ");  
  Serial.println(WiFi.localIP());  
    
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  
  
}  
  

  
void loop() {

     if ( ! rfid.PICC_IsNewCardPresent())         
    return;

  if ( ! rfid.PICC_ReadCardSerial())     
    return;
  

  String ids=ekranaYazdir();
  String id=ids;
  String idgelen=Firebase.getString("/projenodemcu/kisi1/id"); 
  String idizin=Firebase.getString("/projenodemcu/kisi1/izin");   
  String idgelen2=Firebase.getString("/projenodemcu/kisi2/id"); 
  String idizin2=Firebase.getString("/projenodemcu/kisi2/izin");
  String izindeger="verildi";
  String izindegers='"'+izindeger+'"';
    
  
  if ('"'+id+'"'==idgelen) 
  {  
    if(idizin==izindegers)
    {    
      digitalWrite(greenPin,HIGH);
      digitalWrite(redPin,LOW);
    }
    else
    {
      digitalWrite(redPin,HIGH);
      digitalWrite(greenPin,LOW);
    }    
  }  
  else if ('"'+id+'"'==idgelen2) 
  {       
        if(idizin2==izindegers) 
        {   
          digitalWrite(greenPin,HIGH);
      digitalWrite(redPin,LOW);
        }
        else
        {
            digitalWrite(redPin,HIGH);
      digitalWrite(greenPin,LOW);
        } 
  }
    
    
    else{                                    
      Serial.println("Kaydınız Bulunmamaktadır");      
    }
  rfid.PICC_HaltA();
  
}  

  
String ekranaYazdir(){
  
  String kisi = "";
  
   for(int sayac = 0; sayac < 4; sayac++){
    kisi=kisi+String((rfid.uid.uidByte[sayac]));
    
  }
  Firebase.setString("/projenodemcu/okunanid",kisi);
  Serial.println("ID Numarasi: "+kisi);
  return kisi;
}
