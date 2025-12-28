#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ESP32Ping.h>

const char* ssid = "ARRIS-D539";
const char* password = "5G5344101973";

WebServer server(80);

String scanResults = "";
bool isScanning = false;
int scanProgress = 0;

void scanNetwork() {
  isScanning = true;
  scanResults = "[";
  scanProgress = 0;
  
  Serial.println("Starting network scan with ICMP ping...");
  
  int devicesFound = 0;
  
  // Get local network info
  IPAddress localIP = WiFi.localIP();
  IPAddress gateway = WiFi.gatewayIP();
  
  // Add ESP32 itself
  scanResults += "{\"ip\":\"" + localIP.toString() + "\",\"device\":\"ESP32-S3 (This Device)\",\"status\":\"online\",\"latency\":\"0ms\"}";
  devicesFound++;
  
  // Scan the subnet
  IPAddress subnet = localIP;
  for (int i = 1; i <= 254; i++) {
    subnet[3] = i;
    String targetIP = subnet.toString();
    
    // Skip our own IP (already added)
    if (targetIP == localIP.toString()) {
      scanProgress = (i * 100) / 254;
      continue;
    }
    
    // Ping the IP address (timeout: 500ms, 1 attempt)
    bool success = Ping.ping(targetIP.c_str(), 1);
    
    if (success) {
      devicesFound++;
      
      if (devicesFound > 1) {
        scanResults += ",";
      }
      
      float avgTime = Ping.averageTime();
      String deviceType = "Unknown Device";
      
      // Try to identify device type based on IP
      if (targetIP == gateway.toString()) {
        deviceType = "Gateway/Router";
      }
      
      scanResults += "{";
      scanResults += "\"ip\":\"" + targetIP + "\",";
      scanResults += "\"device\":\"" + deviceType + "\",";
      scanResults += "\"status\":\"online\",";
      scanResults += "\"latency\":\"" + String(avgTime) + "ms\"";
      scanResults += "}";
      
      Serial.print("Found device at ");
      Serial.print(targetIP);
      Serial.print(" (");
      Serial.print(avgTime);
      Serial.println("ms)");
    }
    
    // Update progress
    scanProgress = (i * 100) / 254;
    
    // Progress indicator every 25 IPs
    if (i % 25 == 0) {
      Serial.print("Progress: ");
      Serial.print(scanProgress);
      Serial.println("%");
    }
    
    yield(); // Prevent watchdog timeout
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
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <title>Network Scanner</title>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }
    
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      min-height: 100vh;
      padding: 20px;
      color: white;
    }
    
    .container {
      max-width: 900px;
      margin: 0 auto;
    }
    
    .header {
      text-align: center;
      padding: 40px 20px;
      background: rgba(255, 255, 255, 0.1);
      backdrop-filter: blur(10px);
      border-radius: 20px;
      margin-bottom: 30px;
      box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
    }
    
    .header h1 {
      font-size: 2.5em;
      margin-bottom: 10px;
    }
    
    .info-box {
      background: rgba(255, 255, 255, 0.1);
      backdrop-filter: blur(10px);
      border-radius: 15px;
      padding: 20px;
      margin-bottom: 20px;
    }
    
    .info-box h3 {
      margin-bottom: 10px;
    }
    
    .info-item {
      display: flex;
      justify-content: space-between;
      padding: 8px 0;
      border-bottom: 1px solid rgba(255, 255, 255, 0.1);
    }
    
    .info-item:last-child {
      border-bottom: none;
    }
    
    .controls {
      background: rgba(255, 255, 255, 0.1);
      backdrop-filter: blur(10px);
      border-radius: 15px;
      padding: 20px;
      margin-bottom: 20px;
      text-align: center;
    }
    
    .scan-btn {
      background: linear-gradient(135deg, #00d2ff 0%, #3a7bd5 100%);
      border: none;
      color: white;
      padding: 15px 40px;
      font-size: 1.2em;
      border-radius: 10px;
      cursor: pointer;
      font-weight: bold;
      transition: transform 0.2s ease;
      box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);
    }
    
    .scan-btn:hover {
      transform: translateY(-2px);
      box-shadow: 0 6px 20px rgba(0, 0, 0, 0.3);
    }
    
    .scan-btn:active {
      transform: translateY(0);
    }
    
    .scan-btn:disabled {
      background: #666;
      cursor: not-allowed;
      transform: none;
    }
    
    .status {
      margin-top: 15px;
      font-size: 1.1em;
      min-height: 30px;
    }
    
    .progress-bar {
      width: 100%;
      height: 30px;
      background: rgba(0, 0, 0, 0.3);
      border-radius: 15px;
      overflow: hidden;
      margin-top: 15px;
      display: none;
    }
    
    .progress-fill {
      height: 100%;
      background: linear-gradient(90deg, #00d2ff 0%, #3a7bd5 100%);
      width: 0%;
      transition: width 0.3s ease;
      display: flex;
      align-items: center;
      justify-content: center;
      font-weight: bold;
    }
    
    .info-badge {
      background: rgba(0, 200, 255, 0.3);
      border-left: 4px solid #00d2ff;
      padding: 15px;
      border-radius: 5px;
      margin-bottom: 20px;
    }
    
    .results {
      background: rgba(255, 255, 255, 0.1);
      backdrop-filter: blur(10px);
      border-radius: 15px;
      padding: 20px;
      box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
    }
    
    .results h2 {
      margin-bottom: 20px;
      font-size: 1.8em;
    }
    
    .device-list {
      display: grid;
      gap: 10px;
    }
    
    .device {
      background: rgba(255, 255, 255, 0.15);
      padding: 15px 20px;
      border-radius: 10px;
      transition: transform 0.2s ease;
    }
    
    .device:hover {
      transform: translateX(5px);
      background: rgba(255, 255, 255, 0.2);
    }
    
    .device-header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin-bottom: 5px;
    }
    
    .device-ip {
      font-size: 1.2em;
      font-weight: bold;
      font-family: monospace;
    }
    
    .device-info {
      display: flex;
      justify-content: space-between;
      align-items: center;
      font-size: 0.9em;
      opacity: 0.8;
      margin-top: 5px;
    }
    
    .device-status {
      background: #00ff00;
      color: #000;
      padding: 5px 15px;
      border-radius: 20px;
      font-size: 0.9em;
      font-weight: bold;
    }
    
    .latency {
      background: rgba(255, 255, 255, 0.2);
      padding: 3px 10px;
      border-radius: 10px;
      font-family: monospace;
    }
    
    .loading {
      text-align: center;
      padding: 40px;
      font-size: 1.2em;
    }
    
    .spinner {
      border: 4px solid rgba(255, 255, 255, 0.3);
      border-top: 4px solid white;
      border-radius: 50%;
      width: 50px;
      height: 50px;
      animation: spin 1s linear infinite;
      margin: 20px auto;
    }
    
    @keyframes spin {
      0% { transform: rotate(0deg); }
      100% { transform: rotate(360deg); }
    }
    
    .empty {
      text-align: center;
      padding: 40px;
      font-size: 1.1em;
      opacity: 0.7;
    }
  </style>
</head>
<body>
  <div class='container'>
    <div class='header'>
      <h1>Network Scanner</h1>
      <p>ICMP Ping-based device discovery</p>
    </div>
    
    <div class='info-box' id='networkInfo'>
      <h3>Network Information</h3>
      <div class='info-item'>
        <span>Loading...</span>
      </div>
    </div>
    
    <div class='info-badge'>
      This scanner uses ICMP ping to detect ALL devices on your network!
    </div>
    
    <div class='controls'>
      <button class='scan-btn' id='scanBtn' onclick='startScan()'>Start Network Scan</button>
      <div class='progress-bar' id='progressBar'>
        <div class='progress-fill' id='progressFill'>0%</div>
      </div>
      <div class='status' id='status'></div>
    </div>
    
    <div class='results'>
      <h2>Discovered Devices</h2>
      <div id='deviceList' class='device-list'>
        <div class='empty'>Click "Start Network Scan" to discover devices</div>
      </div>
    </div>
  </div>
  
  <script>
    let scanning = false;
    let progressInterval;
    
    window.onload = async function() {
      try {
        const response = await fetch('/networkinfo');
        const data = await response.json();
        displayNetworkInfo(data);
      } catch (error) {
        console.error('Failed to load network info');
      }
    };
    
    function displayNetworkInfo(info) {
      const networkInfo = document.getElementById('networkInfo');
      networkInfo.innerHTML = `
        <h3>Network Information</h3>
        <div class='info-item'>
          <span>ESP32 IP Address:</span>
          <span><strong>${info.ip}</strong></span>
        </div>
        <div class='info-item'>
          <span>Gateway:</span>
          <span><strong>${info.gateway}</strong></span>
        </div>
        <div class='info-item'>
          <span>Subnet Mask:</span>
          <span><strong>${info.subnet}</strong></span>
        </div>
        <div class='info-item'>
          <span>MAC Address:</span>
          <span><strong>${info.mac}</strong></span>
        </div>
      `;
    }
    
    async function updateProgress() {
      try {
        const response = await fetch('/progress');
        const data = await response.json();
        const progressFill = document.getElementById('progressFill');
        progressFill.style.width = data.progress + '%';
        progressFill.textContent = data.progress + '%';
      } catch (error) {
        console.error('Failed to get progress');
      }
    }
    
    async function startScan() {
      if (scanning) return;
      
      scanning = true;
      const btn = document.getElementById('scanBtn');
      const status = document.getElementById('status');
      const deviceList = document.getElementById('deviceList');
      const progressBar = document.getElementById('progressBar');
      const progressFill = document.getElementById('progressFill');
      
      btn.disabled = true;
      btn.textContent = 'Scanning...';
      status.textContent = 'Starting scan...';
      progressBar.style.display = 'block';
      progressFill.style.width = '0%';
      progressFill.textContent = '0%';
      deviceList.innerHTML = '<div class="loading"><div class="spinner"></div>Scanning network with ICMP ping...</div>';
      
      // Update progress every 500ms
      progressInterval = setInterval(updateProgress, 500);
      
      try {
        const response = await fetch('/scan');
        const data = await response.json();
        
        clearInterval(progressInterval);
        progressFill.style.width = '100%';
        progressFill.textContent = '100%';
        
        displayResults(data);
        status.textContent = `Scan complete! Found ${data.length} device(s)`;
        
        setTimeout(() => {
          progressBar.style.display = 'none';
        }, 2000);
      } catch (error) {
        clearInterval(progressInterval);
        status.textContent = 'Scan failed. Please try again.';
        deviceList.innerHTML = '<div class="empty">Error during scan</div>';
        progressBar.style.display = 'none';
      }
      
      btn.disabled = false;
      btn.textContent = 'Start Network Scan';
      scanning = false;
    }
    
    function displayResults(devices) {
      const deviceList = document.getElementById('deviceList');
      
      if (devices.length === 0) {
        deviceList.innerHTML = '<div class="empty">No devices found</div>';
        return;
      }
      
      let html = '';
      devices.forEach(device => {
        html += `
          <div class='device'>
            <div class='device-header'>
              <span class='device-ip'>${device.ip}</span>
              <span class='device-status'>${device.status}</span>
            </div>
            <div class='device-info'>
              <span>${device.device}</span>
              <span class='latency'>${device.latency}</span>
            </div>
          </div>
        `;
      });
      
      deviceList.innerHTML = html;
    }
  </script>
</body>
</html>
)rawliteral";
  
  server.send(200, "text/html", html);
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
  
  // Start mDNS
  if (MDNS.begin("esp32")) {
    Serial.println("mDNS responder started");
    Serial.println("Access via: http://esp32.local");
  }
  
  server.on("/", handleRoot);
  server.on("/scan", handleScan);
  server.on("/networkinfo", handleNetworkInfo);
  server.on("/progress", handleProgress);
  server.begin();
  
  Serial.println("Network Scanner ready!");
  Serial.println("Open http://esp32.local in your browser");
}

void loop() {
  server.handleClient();
}