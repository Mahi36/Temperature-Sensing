#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "temperature-5945e.firebaseio.com"  //should not have http:// and the end /
#define FIREBASE_AUTH "y2IcXVGwlXdosmFSqwjUQZcIaUgnfSrqcJPOaMsN"  // go to settings in firebase,select project settings and service account, secrets auth key
#define WIFI_SSID "sdfg" //your wifi name
#define WIFI_PASSWORD "akash444" //your wifi password
#define LED D7 /// yellow led
int a;
int t;
int H_L;
int x;
int red_led = D5;
int green_led = D6;
void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  pinMode(green_led,OUTPUT);
  pinMode(red_led,OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Firebase.setString("activate","0");
}
void firebasereconnect()
  {
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  }

void loop() {
  if (Firebase.failed()) 
  {
    Serial.print("setting number failed:");
    Serial.println(Firebase.error());  
    firebasereconnect();
    return;
  }
 
  // Input from thermistor
   t = analogRead(A0);//input from thermistor
   a = map(t,0,1024,0,35);//mapped temperature value to be entered
   delay(1000);
   Serial.println(a);
   
  
 
  if (a<=25 && a>=10){
    H_L=1;//permissible
    digitalWrite(green_led,HIGH);
    digitalWrite(red_led,LOW);
  }
  else{
    H_L=0;// Not permissible
    digitalWrite(red_led,HIGH);
    digitalWrite(green_led,LOW);
  }
  //Serial.println(a);
  Firebase.set("temp",a);
  Firebase.set("perm",H_L); 
  if((Firebase.getString("activate")=="1")&& (H_L==0))
  {
    // led intensity blincking
    x = map(a,23,35,255,0);
    analogWrite(LED,x);// yellow led blinking
   
    
    //delay(x/100);
    //Serial.println(x);
  }
  if(H_L==1){
      Firebase.setString("activate","0");
      analogWrite(LED,0);
    }
}
