#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LedControl.h>

int DIN = 5; // D1
int CS = 4; // D2
int CLK = 14; // clk - D5
LedControl matrizLed = LedControl(DIN, CLK, CS, 4);

const char *ssid     = "Tuff Gong";
const char *password = "hmmrsprcOUO";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "b.st1.ntp.br", -10800, 60000);

// côcozin
void resolveDigito2(int num, byte *arr);

byte d1[8] = {  B00001000,  B00011000,  B00001000,  B00001000,
  B00001000,  B00001000,  B00001000,  B00011100 };

byte d2[8] = {  B00011100,  B00100010,  B00000010,  B00000010,
  B00000100,  B00001000,  B00010000,  B00111110 };

byte d3[8] = {  B00011100,  B00100010,  B00000010,  B00001100,
  B00000010,  B00000010,  B00100010,  B00011100 };

byte d4[8] = {  B00000010,  B00000110,  B00001010,  B00010010,
  B00100010,  B00111110,  B00000010,  B00000010 };

byte d5[8] = {  B00111110,  B00100000,  B00100000,  B00111100,
  B00000010,  B00000010,  B00100010,  B00011100 };

byte d6[8] = {  B00011100,  B00100010,  B00100000,  B00111100,
  B00100010,  B00100010,  B00100010,  B00011100 };

byte d7[8] = {  B00111110,  B00000010,  B00000010,  B00000100,
  B00001000,  B00010000,  B00010000,  B00010000 };

byte d8[8] = {  B00011100,  B00100010,  B00100010,  B00011100,
  B00100010,  B00100010,  B00100010,  B00011100 };

byte d9[8] = {  B00011100,  B00100010,  B00100010,  B00100010,
  B00011110,  B00000010,  B00100010,  B00011100 };

byte d0[8] = {  B00011100,  B00100010,  B00100010,  B00100010,
  B00100010,  B00100010,  B00100010,  B00011100 };

byte c[8] = {  B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000 };

byte d_min_dez[8] = {  B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000 };

byte d_min_uni[8] = {  B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000 };

byte d_hora_dez[8] = {  B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000 };

byte d_hora_uni[8] = {  B00000000, B00000000, B00000000, B00000000,
               B00000000, B00000000, B00000000, B00000000 };

byte line_off = B00000000;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
   // Configura e inicializa cada bloco de Led da Matriz
  matrizLed.shutdown(0, false);
  matrizLed.setIntensity(0, 1);
  matrizLed.clearDisplay(0);

  matrizLed.shutdown(1, false);
  matrizLed.setIntensity(1, 1);
  matrizLed.clearDisplay(1);

  matrizLed.shutdown(2, false);
  matrizLed.setIntensity(2, 1);
  matrizLed.clearDisplay(2);

  matrizLed.shutdown(3, false);
  matrizLed.setIntensity(3, 1);
  matrizLed.clearDisplay(3);

  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

int last_min = 0;
int last_hour = 0;

int last_min_uni = 99;
int last_hora_uni = 99;
int last_min_dez = 99;
int last_hora_dez = 99;

byte min_uni_changed = 0;
byte hora_uni_changed = 0;
byte min_dez_changed = 0;
byte hora_dez_changed = 0;

byte min_uni_in[8];
byte min_uni_out[8];

byte min_dez_in[8];
byte min_dez_out[8];

byte hora_uni_in[8];
byte hora_uni_out[8];

byte hora_dez_in[8];
byte hora_dez_out[8];

byte blink = 0;

void loop() {
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  Serial.println(timeClient.getMinutes());

  int min = timeClient.getMinutes();
  int hora = timeClient.getHours();
  
  int min_dez = min/10;
  int min_uni = min%10;
  int hora_dez = hora/10;
  int hora_uni = hora%10;

  min_uni_changed = 0;
  min_dez_changed = 0;
  hora_uni_changed = 0;
  hora_dez_changed = 0;
  
  if (min != last_min){
//    // ponteiros
    resolveDigito(min_uni, min_uni_in);
    resolveDigito(min_uni==0? 9: min_uni-1, min_uni_out);
    
    if(min_dez != last_min_dez){
      resolveDigito(min_dez, min_dez_in);
      resolveDigito(min_dez==0? 5: min_dez-1, min_dez_out);
      min_dez_changed = 1;
    }

    if(hora_uni != last_hora_uni){
      resolveDigito(hora_uni, hora_uni_in);
      resolveDigito(hora_uni==0? 5: hora_uni-1, hora_uni_out);
      hora_uni_changed = 1;
    }

   if(hora_dez != last_hora_dez){
      resolveDigito(hora_dez, hora_dez_in);
      resolveDigito(hora_dez==0? 2: hora_dez-1, hora_dez_out);
      hora_dez_changed = 1;
    }
    
    // prepara 
    d_min_uni[0] = line_off;
    for(int x=1; x<7; x++){
      d_min_uni[x] = min_uni_out[x-1];
    }

    if(min_dez_changed){
      d_min_dez[0] = line_off;
      for(int x=1; x<7; x++){
        d_min_dez[x] = min_dez_out[x-1];
      }
    }

    if(hora_uni_changed){
      d_hora_uni[0] = line_off;
      for(int x=1; x<7; x++){
        d_hora_uni[x] = hora_uni_out[x-1];
      }
    }

   if(hora_dez_changed){
      d_hora_dez[0] = line_off;
      for(int x=1; x<7; x++){
        d_hora_dez[x] = hora_dez_out[x-1];
      }
    }
    
    // imprime primeira linha
    for(int i=0; i<8; i++){
      matrizLed.setRow(0, i, d_min_uni[i]);
      if(min_dez_changed) matrizLed.setRow(1, i, d_min_dez[i]);
      if(hora_uni_changed) matrizLed.setRow(2, i, d_hora_uni[i]);
      if(hora_dez_changed) matrizLed.setRow(3, i, d_hora_dez[i]);

      delay(5);
    }

  //  
    for(int x=0; x<8; x++){
    // atualiza array
      int i = 0;
      for (int z=0; z<8; z++){
        if (z<=x) d_min_uni[z] = min_uni_in[7-x+z];
        else d_min_uni[z] = min_uni_out[i++];
      }

      if(min_dez_changed){
        i = 0;
        for (int z=0; z<8; z++){
          if (z<=x) d_min_dez[z] = min_dez_in[7-x+z];
          else d_min_dez[z] = min_dez_out[i++];
        }
      }

       if(hora_uni_changed){
        i = 0;
        for (int z=0; z<8; z++){
          if (z<=x) d_hora_uni[z] = hora_uni_in[7-x+z];
          else d_hora_uni[z] = hora_uni_out[i++];
        }
      }

     if(hora_dez_changed){
        i = 0;
        for (int z=0; z<8; z++){
          if (z<=x) d_hora_dez[z] = hora_dez_in[7-x+z];
          else d_hora_dez[z] = hora_dez_out[i++];
        }
      }

    // imprime
    for (int i=0; i<8; i++){
      matrizLed.setRow(0, i, d_min_uni[i]);
      if(min_dez_changed) matrizLed.setRow(1, i, d_min_dez[i]);
      if(hora_uni_changed) matrizLed.setRow(2, i, d_hora_uni[i]);
      if(hora_dez_changed) matrizLed.setRow(3, i, d_hora_dez[i]);
      
      delay(5);
    }
  }

    
  last_min = min;
  last_min_dez = min_dez;
  last_min_uni = min_uni;
  last_hora_dez = hora_dez;
  last_hora_uni = hora_uni;
  }

  if(blink){
    blink = 0;
    matrizLed.setColumn(1, 0, line_off);
  } else {
    blink = 1;
    matrizLed.setColumn(1, 0, B00011000);
  }
  delay(500);
}

void resolveDigito(int num, byte arr[]){
  byte *p;
  
  switch(num){
    case 1: p = d1; break;
    case 2: p = d2; break;
    case 3: p = d3; break;
    case 4: p = d4; break;
    case 5: p = d5; break;
    case 6: p = d6; break;
    case 7: p = d7; break;
    case 8: p = d8; break;
    case 9: p = d9; break;
    default: p = d0; break;
  }

  for (int i=0; i<8; i++) arr[i] = p[i];
}
