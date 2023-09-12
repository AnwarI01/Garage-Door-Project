//Normally Closed, Writing high signal turns LED OFF
//LOW signal means LED ON

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include<WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

#define ON_Board_LED 2  //--> Defining an On Board LED (GPIO2 = D4), used for indicators when the process of connecting to a wifi router

#define LED_D6 12 //--> Defines an LED Pin. D6 = GPIO12

#define hallEffectPin 13 //Hall effect senosr to determine if garage is closed or open, D7 = GPIO13

#define garageSignalPin 15 //Signal garage to open or close, D8 = GPIO15


String httpGetRequest(String GetAddress, String LinkGet, String getData);
void httpPostRequest(String SendAddress, String LinkSend, String postData);
void connectToWifi();

//----------------------------------------SSID and Password of your WiFi router.

//For actual garage door
const char* ssid = ""; //--> Your wifi name or SSID.
const char* password = ""; //--> Your wifi password.
//----------------------------------------

//----------------------------------------Web Server address / IPv4
// If using IPv4, press Windows key + R then type cmd, then type ipconfig (If using Windows OS).
//const char *host = "http://IPv4/";

//For actual garage door
//Use your host name, below is just example used
const char *host = "http://5408garageproject.000webhostapp.com/";

//Not used
//const int httpsPort = 443;
//----------------------------------------

//Not used
//const char* fingerprint = "92 99 1E EC B2 E4 C2 E2 E3 D4 05 9D 5A 31 CB DB 3E 69 12 5C";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
  
  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board

  pinMode(LED_D6,OUTPUT); //--> LED port Direction output
  digitalWrite(LED_D6, LOW); //--> Turn off Led

  //NodeMCU connecting to Wifi Router
  connectToWifi();

  pinMode(hallEffectPin, INPUT);
  pinMode(garageSignalPin, OUTPUT);

  //Attempted with https
  /*WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if(!client.connect(host, httpsPort)){
    Serial.println("connection failed");
    return;
  }

  if(client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  }
  else{
    Serial.println("certificate doesn't match");
  }*/
}

void loop() {
   //digitalWrite(garageSignalPin, LOW); //Open garage
  
  // put your main code here, to run repeatedly:
  HTTPClient http; //--> Declare object of class HTTPClient

  //----------------------------------------Getting Data from MySQL Database
  //Declaring string variable for http get
  String GetAddress, LinkGet, getData,hallEffectStatus;
  int id = 0; //--> ID in Database
  GetAddress = "getGarageData.php"; 
  LinkGet = host + GetAddress; //--> Make a Specify request destination
  getData = "ID=" + String(id);

  //Calling httpGetRequest function to see if garage button is pressed
  String payloadGet = httpGetRequest(GetAddress, LinkGet, getData);

  if (payloadGet == "1") {
    digitalWrite(LED_D6, HIGH);
    digitalWrite(garageSignalPin, LOW); //Open/Close garage
    //digitalWrite(garageSignalPin, HIGH); //Open/Close garage


    //Resetting button status stored in database  `.
    
    String postData, LinkSend, SendAddress;
    SendAddress = "updateStatus.php";
    LinkSend = host + SendAddress;
    postData = "Stat=0";

    //Reseting button pressed in database to zero
    httpPostRequest(SendAddress, LinkSend, postData);
  }
  if (payloadGet == "0") {
    digitalWrite(LED_D6, LOW);
    digitalWrite(garageSignalPin, HIGH); //Reset Relay Board
  }
  //----------------------------------------



  //----------------- Sends LED status feedback data to server

  hallEffectStatus = digitalRead(hallEffectPin);
  Serial.println("hall effect status:");
  Serial.println(hallEffectStatus);

  
  Serial.println();
  Serial.println("Sending Hall Effect Sensor Status to Server");
  String postData, LinkSend, SendAddress;
  SendAddress = "getGarageStatus.php";
  LinkSend = host + SendAddress;
  postData = "getGarageStatus=" + hallEffectStatus;

  //Sending status of garage door
  httpPostRequest(SendAddress, LinkSend, postData);
  
  Serial.println("----------------Closing Connection----------------");
  http.end(); //--> Close connection
  Serial.println();
  Serial.println("Please wait 5 seconds for the next connection.");
  Serial.println();
  delay(5000); //--> GET Data at every 5 seconds
}

//Making a get request, recieving data
String httpGetRequest(String GetAddress, String LinkGet, String getData)
{
  // put your main code here, to run repeatedly:
  HTTPClient http; //--> Declare object of class HTTPClient
  
  Serial.println("----------------Connect to Server-----------------");
  Serial.println("Get LED Status from Server or Database");
  Serial.print("Request Link : ");
  Serial.println(LinkGet);
  http.begin(LinkGet); //--> Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
  int httpCodeGet = http.POST(getData); //--> Send the request
  String payloadGet = http.getString(); //--> Get the response payload from server
  Serial.print("Response Code : "); //--> If Response Code = 200 means Successful connection, if -1 means connection failed. For more information see here : https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
  Serial.println(httpCodeGet); //--> Print HTTP return code
  Serial.print("Returned data from Server : ");
  Serial.println(payloadGet); //--> Print request response payload

  return payloadGet;
}

//Making a post request, seding data
void httpPostRequest(String SendAddress, String LinkSend, String postData)
{
  // put your main code here, to run repeatedly:
  HTTPClient http; //--> Declare object of class HTTPClient
  
  Serial.print("Request Link : ");
  Serial.println(LinkSend);
  http.begin(LinkSend); //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
  int httpCodeSend = http.POST(postData); //--> Send the request
  String payloadSend = http.getString(); //--> Get the response payload from server
  Serial.print("Response Code : "); //--> If Response Code = 200 means Successful connection, if -1 means connection failed. For more information see here : https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
  Serial.println(httpCodeSend); //--> Print HTTP return code
  Serial.print("Returned data from Server : ");
  Serial.println(payloadSend); //--> Print request response payload
}

//NodeMCU connecting to the router
void connectToWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");

  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
    //----------------------------------------
  }

  
  //----------------------------------------
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If successfully connected to the wifi router, the IP Address that will be visited is displayed in the serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //----------------------------------------
}
