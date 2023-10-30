/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2023 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeSpan
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *  
 ********************************************************************************/

#include "HomeSpan.h"

struct LED_Service : Service::LightBulb {

  int ledPin;
  SpanCharacteristic *power;
  
  LED_Service(int ledPin) : Service::LightBulb(){
    power=new Characteristic::On();
    this->ledPin=ledPin;
    pinMode(ledPin,OUTPUT);    
  }

  boolean update(){            
    digitalWrite(ledPin,power->getNewVal());
    WEBLOG("Power = %s",power->getNewVal()?"ON":"OFF");
    return(true);  
  }

};

//////////////////////////////////////

void extraData(String &r){
  r+="<tr><td>Free DRAM:</td><td>" + String(esp_get_free_internal_heap_size()) + " bytes</td></tr>\n"; 
  r+="</table><p><a href=\"https://github.com/HomeSpan/HomeSpan\">Click Here to Access HomeSpan Repo</a><p>";
}

//////////////////////////////////////

void setup() {
  
  Serial.begin(115200);

//  homeSpan.setHostNameSuffix("");

//  homeSpan.setControlPin(21);
  
  homeSpan.setLogLevel(2).enableWebLog(20).setWebLogCallback(extraData);
//  homeSpan.reserveSocketConnections(10);
  
//  homeSpan.setApSSID("HS_Setup");
//  homeSpan.setApPassword("");

//          .setStatusPin(13);
//  homeSpan.setSerialInputDisable(true);
//  homeSpan.enableOTA();

  homeSpan.setWifiCallback(wifiCB);
  homeSpan.setWifiCallbackAll(wifiCB_ALL).setVerboseWifiReconnect(true);
  

  new SpanUserCommand('D', " - disconnect WiFi", [](const char *buf){WiFi.disconnect();});

  homeSpan.begin(Category::Lighting,"HomeSpan LED");

  new SpanAccessory();   
    new Service::AccessoryInformation(); 
      new Characteristic::Identify();
//    new LED_Service(13);

  for(int i=0;i<50;i++){
    new SpanAccessory();                            
      new Service::AccessoryInformation();                
        new Characteristic::Identify();                        
      new Service::LightBulb();                      
        new Characteristic::On(true);
      new Service::Fan();                             
        new Characteristic::Active();
        new Characteristic::RotationDirection();
        (new Characteristic::RotationSpeed(50))->setRange(0,100,25);
  }

}

//////////////////////////////////////

void loop(){ 
  homeSpan.poll();
}

//////////////////////////////////////

void wifiCB(){
  Serial.printf("\n\n****** IN WIFI CALLBACK *******\n\n");
}

//////////////////////////////////////

void wifiCB_ALL(int n){
  Serial.printf("\n\n****** IN WIFI CALLBACK ALL.  Count=%d *******\n\n",n);
}
