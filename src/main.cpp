#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <ESP32Ping.h>
#include "web_pages.h"

const char* ssid = "ARRIS-D539";
const char* password = "5G5344101973";

WebServer server(80);

String scanResults = "";
bool isScanning = false;
int scanProgress = 0;

// Function to check if a port is open
bool checkPort(String ip, int port, int timeout = 200) {
  WiFiClient client;
  bool isOpen = client.connect(ip.c_str(), port, timeout);
  client.stop();
  return isOpen;
}

// Function to identify device type based on open ports
String identifyDevice(String ip, IPAddress gateway) {
  String deviceType = "Unknown Device";
  
  if (ip == gateway.toString()) {
    return "Gateway/Router";
  }
  
  bool port80 = checkPort(ip, 80);
  bool port443 = checkPort(ip, 443);
  bool port22 = checkPort(ip, 22);
  bool port23 = checkPort(ip, 23);
  bool port445 = checkPort(ip, 445);
  bool port139 = checkPort(ip, 139);
  bool port631 = checkPort(ip, 631);
  bool port5353 = checkPort(ip, 5353);
  bool port8080 = checkPort(ip, 8080);
  bool port3389 = checkPort(ip, 3389);
  
  if (port631) {
    deviceType = "Printer";
  } else if (port445 || port139 || port3389) {
    deviceType = "Windows Computer";
  } else if (port22 && (port80 || port443)) {
    deviceType = "Linux Server/Computer";
  } else if (port22) {
    deviceType = "Linux Device/Raspberry Pi";
  } else if (port5353 && (port80 || port443)) {
    deviceType = "Apple Device/Smart Home";
  } else if ((port80 || port443 || port8080) && port23) {
    deviceType = "Router/Network Device";
  } else if (port80 || port443 || port8080) {
    deviceType = "Web Server/IoT Device";
  } else if (port23) {
    deviceType = "Network Device";
  }
  
  return deviceType;
}

String getOpenPorts(String ip) {
  String ports = "";
  int commonPorts[] = {22, 23, 80, 139, 443, 445, 631, 3389, 5353, 8080};
  int portCount = sizeof(commonPorts) / sizeof(commonPorts[0]);
  
  for (int i = 0; i < portCount; i++) {
    if (checkPort(ip, commonPorts[i], 150)) {
      if (ports.length() > 0) ports += ", ";
      ports += String(commonPorts[i]);
    }
  }
  
  return ports.length() > 0 ? ports : "None detected";
}

void scanNetwork() {
  isScanning = true;
  scanResults = "[";
  scanProgress = 0;
  
  Serial.println("Starting network scan with ICMP ping...");
  
  int devicesFound = 0;
  IPAddress localIP = WiFi.localIP();
  IPAddress gateway = WiFi.gatewayIP();
  
  String espPorts = "80";
  scanResults += "{\"ip\":\"" + localIP.toString() + "\",\"device\":\"ESP32-S3 (This Device)\",\"ports\":\"" + espPorts + "\",\"status\":\"online\",\"latency\":\"0ms\"}";
  devicesFound++;
  
  IPAddress subnet = localIP;
  for (int i = 1; i <= 254; i++) {
    subnet[3] = i;
    String targetIP = subnet.toString();
    
    if (targetIP == localIP.toString()) {
      scanProgress = (i * 100) / 254;
      continue;
    }
    
    bool success = Ping.ping(targetIP.c_str(), 1);
    
    if (success) {
      devicesFound++;
      
      if (devicesFound > 1) {
        scanResults += ",";
      }
      
      float avgTime = Ping.averageTime();
      
      Serial.print("Scanning ports for ");
      Serial.print(targetIP);
      Serial.println("...");
      
      String deviceType = identifyDevice(targetIP, gateway);
      String openPorts = getOpenPorts(targetIP);
      
      scanResults += "{";
      scanResults += "\"ip\":\"" + targetIP + "\",";
      scanResults += "\"device\":\"" + deviceType + "\",";
      scanResults += "\"ports\":\"" + openPorts + "\",";
      scanResults += "\"status\":\"online\",";
      scanResults += "\"latency\":\"" + String(avgTime) + "ms\"";
      scanResults += "}";
      
      Serial.print("Found: ");
      Serial.print(targetIP);
      Serial.print(" - ");
      Serial.print(deviceType);
      Serial.print(" (Ports: ");
      Serial.print(openPorts);
      Serial.print(") - ");
      Serial.print(avgTime);
      Serial.println("ms");
    }
    
    scanProgress = (i * 100) / 254;
    
    if (i % 25 == 0) {
      Serial.print("Progress: ");
      Serial.print(scanProgress);
      Serial.println("%");
    }
    
    yield();
  }
  
  scanResults += "]";
  
  Serial.println("=================================");
  Serial.print("Scan complete! Found ");
  Serial.print(devicesFound);
  Serial.println(" devices.");
  Serial.println("=================================");
  
  isScanning = false;
  scanProgress = 100;
}

void handleRoot() {
  server.send(200, "text/html", buildMainPage());
}

void handleScanner() {
  server.send(200, "text/html", buildScannerPage());
}

void handleNetworkInfo() {
  String json = "{";
  json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
  json += "\"gateway\":\"" + WiFi.gatewayIP().toString() + "\",";
  json += "\"subnet\":\"" + WiFi.subnetMask().toString() + "\",";
  json += "\"mac\":\"" + WiFi.macAddress() + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

void handleProgress() {
  String json = "{\"progress\":" + String(scanProgress) + "}";
  server.send(200, "application/json", json);
}

void handleScan() {
  Serial.println("Scan request received from web interface");
  scanNetwork();
  server.send(200, "application/json", scanResults);
}

void setup() {
  Serial.begin(115200);
  
  // Configure static IP
  IPAddress local_IP(192, 168, 0, 100);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress primaryDNS(8, 8, 8, 8);
  IPAddress secondaryDNS(8, 8, 4, 4);
  
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Static IP configuration failed");
  }
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected!");
  Serial.println("=================================");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Subnet: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.println("=================================");
  
  if (MDNS.begin("esp32")) {
    Serial.println("mDNS responder started");
    Serial.println("Access via: http://esp32.local");
  }
  
  server.on("/", handleRoot);
  server.on("/scanner", handleScanner);
  server.on("/scan", handleScan);
  server.on("/networkinfo", handleNetworkInfo);
  server.on("/progress", handleProgress);
  server.begin();
  
  Serial.println("ESP32 CyberMultiTool ready!");
  Serial.println("Open http://esp32.local in your browser");
}

void loop() {
  server.handleClient();
}