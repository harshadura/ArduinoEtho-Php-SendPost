#include <Ethernet.h>
    #include <SPI.h>
 
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    byte ip[] = {  192, 168, 0, 2};
    byte gateway[] = { 192, 168, 0, 1 };			
  
  unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 1000;  // delay between updates, in milliseconds


    EthernetClient client;
   
void setup()
    {
    Ethernet.begin(mac, ip, gateway);
    Serial.begin(9600);
    Serial.println(Ethernet.localIP());
    }

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest();
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

void httpRequest() {
  // if there's a successful connection:
   
    int sensorValue = analogRead(A0);
    Serial.println(sensorValue);
    
    String temp= "temp=";
    temp += sensorValue;
      

    Serial.println("connecting...");

    if (client.connect("192.168.0.1",82))
    {                                 
    Serial.println("Sending to Server: ");                    
    client.println("POST /ArduinoEtho/arduino.php HTTP/1.1");           
    Serial.print("POST /ArduinoEtho/arduino.php HTTP/1.1");           
    client.println("Host: 192.168.0.1");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Connection: close");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println(temp.length());
    client.println();
    client.print(temp);
    client.println();        

    // note the time that the connection was made:
    lastConnectionTime = millis();    
    }

    else
    {
    Serial.println("Cannot connect to Server");               
    }
    

}
