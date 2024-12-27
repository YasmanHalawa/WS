#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <FS.h>
#include <LittleFS.h>
#include <HTTPClient.h>
#include "WebIndex.h"
#define water1 34
#define water2 35
#define water3 32
#define water4 33
#define water5 39
#define URL "http://sigabut.iotcenter.cloud/api/sensor"
#define FORMAT_FS_IF_FAILED true
// const char* ssid = "TIK MIPA";
// const char* password = "1s@mp4i8";
const char* ssid = "Cendol Dawet";
const char* password = "4p@ya2024!";



//set static ip
IPAddress local_IP(10, 100, 0, 125);  // Set your desired static IP
IPAddress gateway(10, 100, 0, 1);     // Set your network gateway
IPAddress subnet(255, 255, 252, 0);   // Set your network subnet

// Initialize JSONVar
JSONVar JSON_All_Data_Received;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

int height;
int nodeOrder = 2;
int dataSendCount = 0;
unsigned long startTime, endTime;
unsigned long sendDataDelay = -1;
int prevHeight;
int copyHeight;
int currentHeight;

void setup() {

  Serial.begin(115200);
  delay(1000);
  if (LittleFS.begin(FORMAT_FS_IF_FAILED)) {
    if (LittleFS.exists("/data.txt")) {
      readFile(LittleFS, "/data.txt");
    } else {
      writeFile(LittleFS, "/data.txt", dataSendCount);
    }
  } else {
    Serial.println("Filesystem Mount Failed");
    return;
  }
  delay(500);

  pinMode(water1, INPUT_PULLUP);
  pinMode(water2, INPUT_PULLUP);
  pinMode(water3, INPUT_PULLUP);
  pinMode(water4, INPUT_PULLUP);
  pinMode(water5, INPUT_PULLUP);
  delay(1000);

  prevHeight = -1;
  copyHeight = -1;
  currentHeight = 1;
  //----------------------------------------
  delay(500);
  //---------------------------------------- Set Wifi to STA mode
  Serial.println();
  Serial.println("-------------");
  Serial.println("WIFI mode : STA");
  WiFi.mode(WIFI_STA);
  Serial.println("-------------");
  //----------------------------------------
  // Configure static IP
  // if (!WiFi.config(local_IP, gateway, subnet)) {
  //   Serial.println("STA Failed to configure");
  // }
  delay(100);

  //---------------------------------------- Connect to Wi-Fi (STA).
  Serial.println("------------");
  Serial.println("WIFI STA");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int connecting_process_timed_out = 20;  //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    if (connecting_process_timed_out > 0) connecting_process_timed_out--;
    if (connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("------------");


  delay(500);

  //---------------------------------------- Handle Web Server
  Serial.println();
  Serial.println("Setting Up the Main Page on the Server.");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", MAIN_page);
  });

  //---------------------------------------- Handle Web Server Events
  Serial.println();
  Serial.println("Setting up event sources on the Server.");
  events.onConnect([](AsyncEventSourceClient* client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 10 second
    client->send("hello!", NULL, millis(), 10000);
  });
  //----------------------------------------


  //---------------------------------------- Adding event sources on the Server.
  Serial.println();
  Serial.println("Adding event sources on the Server.");
  server.addHandler(&events);
  //----------------------------------------

  //---------------------------------------- Starting the Server.
  Serial.println();
  Serial.println("Starting the Server.");
  server.begin();
  //----------------------------------------


  Serial.println();
  Serial.println("------------");
  Serial.print("ESP32 IP address : ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.println("Visit the IP Address above in your browser to open the main page.");
  Serial.println("------------");
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  startTime = millis();
  
  currentHeight = waterHeigh(water1,water2,water3,water4,water5);
  copyHeight = currentHeight;
  if (currentHeight == prevHeight) {
    currentHeight = -1;
  }
  prevHeight = copyHeight;
  sendDataToWebsocket(nodeOrder, copyHeight);
  
  Serial.println(copyHeight);
  if(currentHeight != -1){
    dataSendCount++;
    //jika ada perubahan nilai sensor baru send data
    //pakcet loss = data pembacaan - data sampai ke cloud
    sendDataToCloud(dataSendCount);
    //penyimpanan data esp32  ketika mati espnya ketika hidup dia langsung lanjut 5x ke 6x bukan lagi mulai dari nol
    updateFileValue(LittleFS, "/data.txt", "DATA_SEND_COUNT", dataSendCount);
  }
  Serial.println("ini selesai");
  Serial.println(dataSendCount);
  
  //kirim ke cloud 5 detik sekali
  delay(5000);
}

//Fungsi perhitungan ketinggian air keseluruhan
int waterHeigh(int pinWaterHeigh1, int pinWaterHeigh2, int pinWaterHeigh3, int pinWaterHeigh4, int pinWaterHeigh5) {
  //pin 
  int pins[5] = { pinWaterHeigh1, pinWaterHeigh2, pinWaterHeigh3, pinWaterHeigh4, pinWaterHeigh5 };
  //nilai awal, pelampung naik = kurang 20
  int totalHeight = 100;

  //looping sebanyak 5x atau perulangan
  //kenapa loping 5x? karena nilai acak sensor, stabilnya d 5x dgn pembacaan analog
  //digital tidak stabil
  for (int i = 0; i < 5; i++) {
    int count = 5;
    Serial.print("ini pin ke : ");
    Serial.println(i);
    // Membaca nilai dari pin sebanyak 5 kali
    for (int j = 0; j < 5; j++) {
      int reading = analogRead(pins[i]);
      Serial.println(reading);

      if (reading < 4095) {
        count--;
      }


      delay(100);  // Delay 100 ms antara setiap pembacaan
    }

    Serial.println("===========");
    // Jika nilai pembacaan kurang dari 4000 sebanyak 1 kali, tambahkan 20 cm ke totalHeight (karena ketidakstabilan sensor)
    if (count <= 0) {
      totalHeight -= 20;
      // Serial.print("Pin ");
      // Serial.print(pins[i]);
      // Serial.println(" memenuhi kriteria, menambah 20 cm");
    } else {
      // Serial.print("Pin ");
      // Serial.print(pins[i]);
      // Serial.println(" tidak memenuhi kriteria, menghentikan pembacaan");
      break;  // Hentikan loop jika pin tidak memenuhi kriteria
    }
  }

  return totalHeight;
}

//fungsi proses dan kirim data websocket
//id node dan ketinggian air
void sendDataToWebsocket(int id, int height) {
  //:::::::::::::::::: Enter the received data into JSONVar(JSON_All_Data_Received).
  JSON_All_Data_Received["node_order"] = id;
  JSON_All_Data_Received["height"] = height;

  //::::::::::::::::::

  //:::::::::::::::::: Create a JSON String to hold all data received from the sender.
  String jsonString_Send_All_Data_received = JSON.stringify(JSON_All_Data_Received);
  Serial.println(jsonString_Send_All_Data_received);

  //::::::::::::::::::

  //:::::::::::::::::: Sends all data received from the sender to the browser as an event ('allDataJSON').
  events.send(jsonString_Send_All_Data_received.c_str(), "allDataJSON", millis());
  
  //::::::::::::::::::
}


//fungsi proses dan kirim data ke cloud
void sendDataToCloud(int dataSendCount) {

  JSON_All_Data_Received["data_send_count"] = dataSendCount;
  if (sendDataDelay != -1) {
    JSON_All_Data_Received["delay"] = sendDataDelay;
  }

  String jsonString = JSON.stringify(JSON_All_Data_Received);
  Serial.println(jsonString);

  HTTPClient http;
  http.begin(String(URL));
  http.addHeader("Content-Type", "application/json");
  
  //proses kirim secara keseluruhan
  int httpCode = http.POST(jsonString);

  if (httpCode == 201) {
    Serial.printf("POST code: %d\n", httpCode);
    endTime = millis();

    //delay pengiriman
    sendDataDelay = (endTime - startTime)-500;

    Serial.print("Delay time: ");
    Serial.print(sendDataDelay);
    Serial.println(" ms");
    // sendDelayData(sendDataDelay);
  } else if (httpCode > 0) {
    //waktu delay pengiriman data
    Serial.printf("POST code: %d\n", httpCode);
    Serial.println(httpCode);
  } else {
    Serial.println("Failed to send data!");
    Serial.println(httpCode);
  }

  http.end();
}


void writeFile(fs::FS& fs, const char* path, int dataSendCount) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }

  file.print("DATA_SEND_COUNT=");
  file.println(dataSendCount);


  file.close();
  Serial.println("");
}


void readFile(fs::FS& fs, const char* path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  String line;
  while (file.available()) {
    line = file.readStringUntil('\n');
    int separatorIndex = line.indexOf('=');
    if (separatorIndex != -1) {
      String name = line.substring(0, separatorIndex);
      String valueStr = line.substring(separatorIndex + 1);
      const char* valueChar = valueStr.c_str();            // Konversi String ke const char*
      unsigned long value = strtoul(valueChar, NULL, 10);  // Mengonversi const char* ke unsigned long
      if (name.equals("DATA_SEND_COUNT")) {
        dataSendCount = value;
        Serial.printf("DATA_SEND_COUNT = %d\n", value);
      }
    }
  }

  file.close();
  Serial.println("");
}

void updateFileValue(fs::FS& fs, const char* path, const char* variable, unsigned long newValue) {
  // Serial.printf("Update file: %s [%s]\n", path, variable);

  File file = fs.open(path, "r+");

  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for updating");
    return;
  }

  // Membuat string untuk mencocokkan dengan variabel yang akan diperbarui
  String variableString = String(variable) + "=";

  // Cari posisi variabel dalam file
  while (file.available()) {
    long startPos = file.position();  // Simpan posisi awal baris
    String line = file.readStringUntil('\n');
    if (line.startsWith(variableString)) {
      // Perbarui nilai variabel
      file.seek(startPos);  // Kembali ke awal baris
      file.print(variableString);
      file.println(newValue);
      file.close();
      // Serial.println("Value updated successfully");
      return;
    }
  }

  // Jika variabel tidak ditemukan dalam file, tambahkan variabel baru dengan nilai baru
  file.seek(file.size());  // Pindah ke akhir file
  file.println(variableString + newValue);
  file.close();
  // Serial.println("New value added successfully");
}
