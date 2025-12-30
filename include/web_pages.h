#ifndef WEB_PAGES_H
#define WEB_PAGES_H

#include <Arduino.h>
#include "web_styles.h"

String buildMainPage() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP32 CyberMultiTool</title>";
  html += "<style>";
  html += String(CSS_COMMON);
  html += String(CSS_MAIN_PAGE);
  html += "</style></head><body>";

  html += "<div class='container'>";

  html += "<div class='header'>"
          "<h1>ESP32 CyberMultiTool</h1>"
          "<p>Your all-in-one network security and diagnostics toolkit</p>"
          "</div>";

  html += "<div class='tools-grid'>";

  html += "<div class='tool-card' onclick='window.location.href=\"/scanner\"'>"
          "<span class='tool-icon'></span>"
          "<h2 class='tool-title'>Network Scanner</h2>"
          "<p class='tool-description'>Discover all devices on your network using ICMP ping and port scanning.</p>"
          "</div>";

  html += "<div class='tool-card disabled'>"
          "<span class='tool-icon'></span>"
          "<h2 class='tool-title'>Port Scanner</h2>"
          "<p class='tool-description'>Deep port scanning for a single target.</p>"
          "<span class='coming-soon'>Coming Soon</span>"
          "</div>";

  html += "<div class='tool-card disabled'>"
          "<span class='tool-icon'></span>"
          "<h2 class='tool-title'>WiFi Analyzer</h2>"
          "<p class='tool-description'>Analyze nearby WiFi networks.</p>"
          "<span class='coming-soon'>Coming Soon</span>"
          "</div>";

  html += "<div class='tool-card disabled'>"
          "<span class='tool-icon'></span>"
          "<h2 class='tool-title'>Speed Test</h2>"
          "<p class='tool-description'>Test your network connection speed.</p>"
          "<span class='coming-soon'>Coming Soon</span>"
          "</div>";

  html += "<div class='tool-card disabled'>"
          "<span class='tool-icon'></span>"
          "<h2 class='tool-title'>DNS Lookup</h2>"
          "<p class='tool-description'>Perform DNS queries and lookups.</p>"
          "<span class='coming-soon'>Coming Soon</span>"
          "</div>";

  html += "<div class='tool-card disabled'>"
          "<span class='tool-icon'></span>"
          "<h2 class='tool-title'>Traffic Monitor</h2>"
          "<p class='tool-description'>Monitor network traffic in real-time.</p>"
          "<span class='coming-soon'>Coming Soon</span>"
          "</div>";

  html += "</div>";  // .tools-grid

  html += "<div class='info-box'>"
          "<h3>System Information</h3>"
          "<div class='info-item'><span>ESP32 IP Address:</span><span id='espIP'>Loading...</span></div>"
          "<div class='info-item'><span>Gateway:</span><span id='gateway'>Loading...</span></div>"
          "<div class='info-item'><span>MAC Address:</span><span id='mac'>Loading...</span></div>"
          "<div class='info-item'><span>Access URL:</span><span>http://esp32.local</span></div>"
          "</div>";

  html += "</div>";  // .container

  html += "<script>"
          "window.onload = async function() {"
          "  try {"
          "    const r = await fetch('/networkinfo');"
          "    const d = await r.json();"
          "    document.getElementById('espIP').textContent = d.ip;"
          "    document.getElementById('gateway').textContent = d.gateway;"
          "    document.getElementById('mac').textContent = d.mac;"
          "  } catch (e) {"
          "    console.error('Failed');"
          "  }"
          "};"
          "</script>";

  html += "</body></html>";

  return html;
}

String buildScannerPage() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>Network Scanner</title>";
  html += "<style>";
  html += String(CSS_COMMON);
  html += String(CSS_SCANNER_PAGE);
  html += "</style></head><body>";

  html += "<div class='container'>";

  html += "<a href='/' class='back-button'>Back to Menu</a>";

  html += "<div class='header'>"
          "<h1>Network Scanner</h1>"
          "<p>ICMP Ping-based device discovery</p>"
          "</div>";

  html += "<div class='info-box' id='networkInfo'>"
          "<h3>Network Information</h3>"
          "<div class='info-item'><span>Loading...</span></div>"
          "</div>";

  html += "<div class='info-badge'>This scanner uses ICMP ping and port scanning!</div>";

  html += "<div class='controls'>"
          "<button class='scan-btn' id='scanBtn' onclick='startScan()'>Start Network Scan</button>"
          "<div class='progress-bar' id='progressBar'>"
          "  <div class='progress-fill' id='progressFill'>0%</div>"
          "</div>"
          "<div class='status' id='status'></div>"
          "</div>";

  html += "<div class='results'>"
          "<div class='results-header'>"
          "  <h2>Discovered Devices</h2>"
          "  <button class='download-btn' id='downloadBtn' onclick='downloadResults()'>Download Results</button>"
          "</div>"
          "<div id='deviceList' class='device-list'>"
          "  <div class='empty'>Click Start to scan</div>"
          "</div>"
          "</div>";

  html += "</div>";  // .container

  html += "<script>";

  html += "let scanning = false;"
          "let progressInterval;"
          "let scannedDevices = [];";

  html += "window.onload = async function() {"
          "  try {"
          "    const r = await fetch('/networkinfo');"
          "    const d = await r.json();"
          "    document.getElementById('networkInfo').innerHTML = "
          "      '<h3>Network Information</h3>' +"
          "      '<div class=\"info-item\"><span>ESP32 IP:</span><span><strong>' + d.ip + '</strong></span></div>' +"
          "      '<div class=\"info-item\"><span>Gateway:</span><span><strong>' + d.gateway + '</strong></span></div>' +"
          "      '<div class=\"info-item\"><span>Subnet:</span><span><strong>' + d.subnet + '</strong></span></div>' +"
          "      '<div class=\"info-item\"><span>MAC:</span><span><strong>' + d.mac + '</strong></span></div>';"
          "  } catch (e) {"
          "  }"
          "};";

  html += "async function updateProgress() {"
          "  try {"
          "    const r = await fetch('/progress');"
          "    const d = await r.json();"
          "    document.getElementById('progressFill').style.width = d.progress + '%';"
          "    document.getElementById('progressFill').textContent = d.progress + '%';"
          "  } catch (e) {"
          "  }"
          "}";

  html += "async function startScan() {"
          "  if (scanning) return;"
          "  scanning = true;"
          "  const btn = document.getElementById('scanBtn');"
          "  const status = document.getElementById('status');"
          "  const deviceList = document.getElementById('deviceList');"
          "  const progressBar = document.getElementById('progressBar');"
          "  btn.disabled = true;"
          "  btn.textContent = 'Scanning...';"
          "  status.textContent = 'Starting...';"
          "  progressBar.style.display = 'block';"
          "  document.getElementById('progressFill').style.width = '0%';"
          "  deviceList.innerHTML = '<div class=\"loading\"><div class=\"spinner\"></div>Scanning...</div>';"
          "  progressInterval = setInterval(updateProgress, 500);"
          "  try {"
          "    const r = await fetch('/scan');"
          "    const d = await r.json();"
          "    scannedDevices = d;"
          "    clearInterval(progressInterval);"
          "    document.getElementById('progressFill').style.width = '100%';"
          "    displayResults(d);"
          "    status.textContent = 'Scan complete! Found ' + d.length + ' devices';"
          "    if (d.length > 0) document.getElementById('downloadBtn').style.display = 'block';"
          "    setTimeout(() => { progressBar.style.display = 'none'; }, 2000);"
          "  } catch (error) {"
          "    clearInterval(progressInterval);"
          "    status.textContent = 'Scan failed';"
          "    deviceList.innerHTML = '<div class=\"empty\">Error</div>';"
          "    progressBar.style.display = 'none';"
          "  }"
          "  btn.disabled = false;"
          "  btn.textContent = 'Start Network Scan';"
          "  scanning = false;"
          "}";

  html += "function displayResults(devices) {"
          "  const deviceList = document.getElementById('deviceList');"
          "  if (devices.length === 0) {"
          "    deviceList.innerHTML = '<div class=\"empty\">No devices found</div>';"
          "    return;"
          "  }"
          "  let h = '';"
          "  devices.forEach(device => {"
          "    const p = device.ports ? '<div class=\"device-ports\">Open Ports: ' + device.ports + '</div>' : '';"
          "    h += '<div class=\"device\">' +"
          "         '<div class=\"device-header\">' +"
          "           '<span class=\"device-ip\">' + device.ip + '</span>' +"
          "           '<span class=\"device-status\">' + device.status + '</span>' +"
          "         '</div>' +"
          "         '<div class=\"device-info\">' +"
          "           '<span>' + device.device + '</span>' +"
          "           '<span class=\"latency\">' + device.latency + '</span>' +"
          "         '</div>' +"
          "         p +"
          "       '</div>';"
          "  });"
          "  deviceList.innerHTML = h;"
          "}";

  html += "function downloadResults() {"
          "  if (scannedDevices.length === 0) return;"
          "  let txt = 'Network Scan Results\\n===================\\n';"
          "  txt += 'Scan Date: ' + new Date().toLocaleString() + '\\n';"
          "  txt += 'Total Devices: ' + scannedDevices.length + '\\n\\n';"
          "  scannedDevices.forEach((d, i) => {"
          "    txt += 'Device ' + (i + 1) + ':\\n';"
          "    txt += '  IP: ' + d.ip + '\\n  Type: ' + d.device + '\\n';"
          "    if (d.ports) txt += '  Ports: ' + d.ports + '\\n';"
          "    txt += '  Status: ' + d.status + '\\n  Latency: ' + d.latency + '\\n\\n';"
          "  });"
          "  const blob = new Blob([txt], { type: 'text/plain' });"
          "  const url = window.URL.createObjectURL(blob);"
          "  const a = document.createElement('a');"
          "  a.href = url;"
          "  a.download = 'scan_' + new Date().getTime() + '.txt';"
          "  document.body.appendChild(a);"
          "  a.click();"
          "  document.body.removeChild(a);"
          "  window.URL.revokeObjectURL(url);"
          "}";

  html += "</script></body></html>";

  return html;
}

#endif
