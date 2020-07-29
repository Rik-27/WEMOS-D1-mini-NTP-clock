#include "LedControl.h"
LedControl lc=LedControl(D4,D2,D3,4);//DIN,CLK,CS
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>
#include <SPI.h>
#define STASSID "YourWiFi"
#define STAPSK  "YourWiFiPassword"
const char* ssid     = STASSID;
const char* password = STAPSK;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
unsigned long delaytime=100;
String formattedDate,formattedTime,hhs,mms,sss;
String dayStamp;
String timeStamp;
int h1,h2,m1,m2,s1,s2;
unsigned long ssss,t,t0,dt,ep,days,secs,delta;
int yyyy,hh,mm,ss,yy,mo,dd;
float rr,ang,ang0,ang1,ang2;
int r,d,dddd,mmmm,i,s;
int m[12]={31,28,31,30,31,30,31,31,30,31,30,31};
char *mon[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

void setup() {
  t = millis();
  Serial.begin(230400);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(-18000);

  for(i=0;i<4;i++){
    lc.shutdown(i,false);
    lc.setIntensity(i,1);/* Set the brightness to a min*/
  }
  for(i=0;i<4;i++){  lc.clearDisplay(i);}
}
//
//lc.setRow(0,row,B10100000);
//lc.setColumn(0,col,B10100000);
//lc.setLed(address,row,col,false);
//
void loop() {
  // put your main code here, to run repeatedly:
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedTime = timeClient.getFormattedTime();
  Serial.print("Network clock: ");
  Serial.print(formattedTime);
  hhs = formattedTime.substring(0,2);
  mms = formattedTime.substring(3,5);
  sss = formattedTime.substring(6,8);
  s1 = sss[0] - '0';
  s2 = sss[1] - '0';
  m1 = mms[0] - '0';
  m2 = mms[1] - '0';
  h1 = hhs[0] - '0';
  h2 = hhs[1] - '0';
//  for(i=0;i<4;i++){  lc.clearDisplay(i);}
  digit(h1,31);
  digit(h2,26);
  digit(m1,19);
  digit(m2,14);
  migit(s1, 8);
  migit(s2, 3);
  lc.setColumn(2,2,B01000010);
  t0 = millis();
  Serial.print(" delta:");
  Serial.println(t0-t);
  delay(960);//adjust to make delta 1000 while running
  t = t0;
}

/*
void one(int column){//0 to 63
  int row,col,adr;
    
  adr = int(column/8);
  col = column % 8;
  if(col>=2){
    lc.setColumn(adr,9-col,B00000001);
    lc.setColumn(adr,8-col,B11111111);
    lc.setColumn(adr,7-col,B01000001);
  }
  if(col==1){
    lc.setColumn(adr-1,0,B00000001);
    lc.setColumn(adr  ,7,B11111111);
    lc.setColumn(adr  ,6,B01000001);
  }
  if(col==0){
    lc.setColumn(adr-1,1,B00000001);
    lc.setColumn(adr-1,0,B11111111);
    lc.setColumn(adr  ,7,B01000001);
  }
       
}
int j=31;
/**/
//6 rows numbers
void migit(int num, int column){
  int c0,c1,c2,c3,a0,a1,a2,a3,adr,col;
  adr = int(column/8);
  col = column % 8;

  switch(col){
  case 3: case 4: case 5: case 6: case 7:
    a0 = adr;
    a1 = adr;
    a2 = adr;
    a3 = adr;
    c3 = 7-col;
    c2 = 8-col;
    c1 = 9-col;
    c0 =10-col;
    break;
  case 2:
    a0 = adr-1;
    a1 = adr;
    a2 = adr;
    a3 = adr;
    c3 = 5;
    c2 = 6;
    c1 = 7;
    c0 = 0;
    break;
  case 1:
    a0 = adr-1;
    a1 = adr-1;
    a2 = adr;
    a3 = adr;
    c3 = 6;
    c2 = 7;
    c1 = 0;
    c0 = 1;
    break;
  case 0:
    a0 = adr-1;
    a1 = adr-1;
    a2 = adr-1;
    a3 = adr;
    c3 = 7;
    c2 = 0;
    c1 = 1;
    c0 = 2;
    break;
  default:
    break;
  }
  switch(num){
    case 0:
      lc.setColumn(a0,c0,B00011110);
      lc.setColumn(a1,c1,B00100001);
      lc.setColumn(a2,c2,B00100001);
      lc.setColumn(a3,c3,B00011110);
      break;
    case 1:
      lc.setColumn(a0,c0,B00000001);
      lc.setColumn(a1,c1,B00111111);
      lc.setColumn(a2,c2,B00010001);
      lc.setColumn(a3,c3,B00001001);
      break;
    case 2:
      lc.setColumn(a0,c0,B00011001);
      lc.setColumn(a1,c1,B00100101);
      lc.setColumn(a2,c2,B00100101);
      lc.setColumn(a3,c3,B00010011);
      break;
    case 3:
      lc.setColumn(a0,c0,B00010110);
      lc.setColumn(a1,c1,B00101001);
      lc.setColumn(a2,c2,B00100001);
      lc.setColumn(a3,c3,B00010010);
      break;
    case 4:
      lc.setColumn(a0,c0,B00111111);
      lc.setColumn(a1,c1,B00001000);
      lc.setColumn(a2,c2,B00001000);
      lc.setColumn(a3,c3,B00111000);
      break;
    case 5:
      lc.setColumn(a0,c0,B00100110);
      lc.setColumn(a1,c1,B00101001);
      lc.setColumn(a2,c2,B00101001);
      lc.setColumn(a3,c3,B00111010);
      break;
    case 6:
      lc.setColumn(a0,c0,B00100110);
      lc.setColumn(a1,c1,B00101001);
      lc.setColumn(a2,c2,B00101001);
      lc.setColumn(a3,c3,B00011110);
      break;
    case 7:
      lc.setColumn(a0,c0,B00110000);
      lc.setColumn(a1,c1,B00101000);
      lc.setColumn(a2,c2,B00100111);
      lc.setColumn(a3,c3,B00100000);
      break;
    case 8:
      lc.setColumn(a0,c0,B00010110);
      lc.setColumn(a1,c1,B00101001);
      lc.setColumn(a2,c2,B00101001);
      lc.setColumn(a3,c3,B00010110);
      break;
    case 9:
      lc.setColumn(a0,c0,B00011110);
      lc.setColumn(a1,c1,B00101001);
      lc.setColumn(a2,c2,B00101001);
      lc.setColumn(a3,c3,B00010010);
      break;
    default:
      break;
  }
}

//8 rows numbers
void digit(int num, int column){
  int c0,c1,c2,c3,a0,a1,a2,a3,adr,col;
  adr = int(column/8);
  col = column % 8;
  switch(col){
  case 3: case 4: case 5: case 6: case 7:
    a0 = adr;
    a1 = adr;
    a2 = adr;
    a3 = adr;
    c3 = 7-col;
    c2 = 8-col;
    c1 = 9-col;
    c0 =10-col;
    break;
  case 2:
    a0 = adr-1;
    a1 = adr;
    a2 = adr;
    a3 = adr;
    c3 = 5;
    c2 = 6;
    c1 = 7;
    c0 = 0;
    break;
  case 1:
    a0 = adr-1;
    a1 = adr-1;
    a2 = adr;
    a3 = adr;
    c3 = 6;
    c2 = 7;
    c1 = 0;
    c0 = 1;
    break;
  case 0:
    a0 = adr-1;
    a1 = adr-1;
    a2 = adr-1;
    a3 = adr;
    c3 = 7;
    c2 = 0;
    c1 = 1;
    c0 = 2;
    break;
  default:
    break;
  }
  
  switch(num){
    case 0:
      lc.setColumn(a0,c0,B01111110);
      lc.setColumn(a1,c1,B10000001);
      lc.setColumn(a2,c2,B10000001);
      lc.setColumn(a3,c3,B01111110);
      break;
    case 1:
      lc.setColumn(a0,c0,B00000001);
      lc.setColumn(a1,c1,B11111111);
      lc.setColumn(a2,c2,B01000001);
      lc.setColumn(a3,c3,B00100001);
      break;
    case 2:
      lc.setColumn(a0,c0,B01100011);
      lc.setColumn(a1,c1,B10010001);
      lc.setColumn(a2,c2,B10001001);
      lc.setColumn(a3,c3,B01000111);
      break;
    case 3:
      lc.setColumn(a0,c0,B01101110);
      lc.setColumn(a1,c1,B10010001);
      lc.setColumn(a2,c2,B10010001);
      lc.setColumn(a3,c3,B01000010);
      break;
    case 4:
      lc.setColumn(a0,c0,B11111111);
      lc.setColumn(a1,c1,B00010000);
      lc.setColumn(a2,c2,B00010000);
      lc.setColumn(a3,c3,B11110000);
      break;
    case 5:
      lc.setColumn(a0,c0,B11001110);
      lc.setColumn(a1,c1,B10010001);
      lc.setColumn(a2,c2,B10010001);
      lc.setColumn(a3,c3,B11110010);
      break;
    case 6:
      lc.setColumn(a0,c0,B01001110);
      lc.setColumn(a1,c1,B10010001);
      lc.setColumn(a2,c2,B10010001);
      lc.setColumn(a3,c3,B01111110);
      break;
    case 7:
      lc.setColumn(a0,c0,B11100000);
      lc.setColumn(a1,c1,B10010000);
      lc.setColumn(a2,c2,B10001111);
      lc.setColumn(a3,c3,B11000000);
      break;
    case 8:
      lc.setColumn(a0,c0,B01101110);
      lc.setColumn(a1,c1,B10010001);
      lc.setColumn(a2,c2,B10010001);
      lc.setColumn(a3,c3,B01101110);
      break;
    case 9:
      lc.setColumn(a0,c0,B01111110);
      lc.setColumn(a1,c1,B10010001);
      lc.setColumn(a2,c2,B10010001);
      lc.setColumn(a3,c3,B01100010);
      break;
    default:
      break;
  }
}
