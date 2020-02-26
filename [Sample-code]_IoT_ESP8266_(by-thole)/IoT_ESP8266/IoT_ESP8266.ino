  
    #include <ThingSpeak.h>     //Thingspeak library
    #include <ESP8266WiFi.h>    //ESP8266 library
    #include "DHT.h"            //DHT sensor library

    const int DHTPIN = D4;       //Read data from pin D4
    const int DHTTYPE = DHT11;  //Declare DHT11 or DHT22

    DHT dht(DHTPIN, DHTTYPE);
    float t;
    float h;
//Setup network parameters
    const char* ssid     = "International University";  //ssid
// const char* password = "  ";  //password
   
//Setup Thingspeaks
    char thingSpeakAddress[]  = "api.thingspeak.com";
    unsigned long channelID   = 852480; //Your chanel ID
    char* writeAPIKey         = "E1MCGYU60JL72RLW";// Your write key
    char* readAPIKey          = "VZP2XVZDZJSSJ72L";//Your read key
//Setup field in Thingspeak
    unsigned int Led_01= 1;                        // LED Field
    unsigned int Temperature= 2;                // Temperature Field
    unsigned int Humidity= 3;                   // Humidity Field
    unsigned int Led_02= 4;                        // LED Field
/*Time update data*/
    const unsigned long Posting_Interval = 30L * 1000L; 
    long lastUpdateTime = 0; 
    WiFiClient client;
    const unsigned long Turn_off_Interval = 30L * 1000L;
    long lastUpdateTime_van1 = 0;
/*----------------------------------------------------------------------------------*/
    void setup()
      {
        pinMode(LED_BUILTIN, OUTPUT);
        pinMode(D5, OUTPUT);
        Serial.begin(9600);
        Serial.println("Start");
        connectWiFi();
        dht.begin();         // Start sensor
      }
/*----------------------------------------------------------------------------------*/
    void loop()
      {
        air_parameter();
        long rssi = WiFi.RSSI();
        if (millis() - lastUpdateTime >=  Posting_Interval) 
          {         
            lastUpdateTime = millis();
            write2TSData( channelID , Temperature , t , Humidity , h );
            Serial.print("-----------------WRITE DATA TO THINGSPEAK---------------------------");
            Serial.println();            
          }
          
          if ( readTSData( channelID, Led_01 ) == 1 )
          { 
             digitalWrite(LED_BUILTIN,LOW);               
          }
          else
          {
            digitalWrite(LED_BUILTIN,HIGH);
          }  
          
         if ( readTSData( channelID, Led_02 ) == 1 )
          { 
             digitalWrite(D5,HIGH);               
          }
          else
          {
            digitalWrite(D5,LOW);
          }  
           //Read data from pin D5
      }
/*----------------------------------------------------------------------------------*/
// Set up Wifi Connection       
    int connectWiFi()
        {   
          Serial.begin(9600);
          delay(10);
 
          // Connect WiFi
          Serial.println();
          Serial.println();
          Serial.print("Connecting to ");
          Serial.println(ssid);
          WiFi.hostname("Name");
          WiFi.begin(ssid);
          //  WiFi.begin(ssid, password);
 
          if (WiFi.status() != WL_CONNECTED) 
          {
            delay(500);
            Serial.print(".");
            Serial.println("Connecting to WiFi");
          }
          Serial.println("");
          Serial.println("WiFi connected");
          // Print the IP address
          Serial.print("IP address: ");
          Serial.print(WiFi.localIP());              
          ThingSpeak.begin( client );
        }
/*Use this function if you want to read from a single field*/    
    float readTSData( long TSChannel,unsigned int TSField )
      {        
        float data =  ThingSpeak.readFloatField( TSChannel, TSField, readAPIKey );
        Serial.println( " Data read from ThingSpeak: " + String( data, 9 ) );
        return data;
      }

      
/*Use this function if you want to write a single field*/
    int writeTSData( long TSChannel, unsigned int TSField, float data )
      {
        int  writeSuccess = ThingSpeak.writeField( TSChannel, TSField, data, writeAPIKey ); // Write the data to the channel
        if ( writeSuccess )
          {
            Serial.println( String(data) + " written to field " + String(TSField - 4) + " of Thingspeak." );
          }
        return writeSuccess;  
      }

      
//use this function if you want multiple fields simultaneously
    int write2TSData( long TSChannel, unsigned int TSField1, float field1Data , unsigned int TSField2, float field2Data)
      {
        ThingSpeak.setField( TSField1, field1Data );
        ThingSpeak.setField( TSField2, field2Data );
        int writeSuccess = ThingSpeak.writeFields( TSChannel, writeAPIKey );
        return writeSuccess;
      }

//Sensor
     void air_parameter()
       {
 
         h = dht.readHumidity();    //Read humidity
         t = dht.readTemperature(); //Read temperature
 
         Serial.print("Temperature: ");
         Serial.println(t);               //Print temperature
         Serial.print("Humidity: ");
         Serial.println(h);               //Print humidity
        delay(3000);
        }



    
