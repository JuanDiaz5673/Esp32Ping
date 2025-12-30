#ifndef WEB_STYLES_H
#define WEB_STYLES_H

const char CSS_COMMON[] = R"rawliteral(
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  background: #110f18;
  min-height: 100vh;
  padding: 20px;
  color: white;
}

.container {
  max-width: 1000px;
  margin: 0 auto;
}

.header {
  text-align: center;
  padding: 60px 20px 40px;
  background: rgba(255, 255, 255, 0.05);
  backdrop-filter: blur(10px);
  border-radius: 20px;
  margin-bottom: 40px;
  box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3);
}

.header h1 {
  font-size: 10vw;
  margin-bottom: 15px;
  background: linear-gradient(135deg, #00d2ff 0%, #3a7bd5 100%);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
}

@media (min-width: 768px) {
  .header h1 {
    font-size: 3em;
  }
}

.header p {
  font-size: 1.2em;
  opacity: 0.8;
}

.info-box {
  background: rgba(255, 255, 255, 0.05);
  backdrop-filter: blur(10px);
  border-radius: 15px;
  padding: 20px;
  margin-top: 30px;
}

.info-box h3 {
  margin-bottom: 15px;
  background: linear-gradient(135deg, #00d2ff 0%, #3a7bd5 100%);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
}

.info-item {
  display: flex;
  justify-content: space-between;
  padding: 10px 0;
  border-bottom: 1px solid rgba(255, 255, 255, 0.1);
}

.info-item:last-child {
  border-bottom: none;
}
)rawliteral";

const char CSS_MAIN_PAGE[] = R"rawliteral(
.tools-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
  gap: 25px;
  margin-bottom: 30px;
}

.tool-card {
  background: rgba(255, 255, 255, 0.05);
  backdrop-filter: blur(10px);
  border-radius: 15px;
  padding: 30px;
  box-shadow: 0 8px 32px rgba(0, 0, 0, 0.2);
  transition: all 0.3s ease;
  cursor: pointer;
  border: 2px solid transparent;
}

.tool-card:hover {
  transform: translateY(-5px);
  box-shadow: 0 12px 40px rgba(0, 210, 255, 0.3);
  border-color: rgba(0, 210, 255, 0.5);
  background: rgba(255, 255, 255, 0.08);
}

.tool-card.disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

.tool-card.disabled:hover {
  transform: none;
  box-shadow: 0 8px 32px rgba(0, 0, 0, 0.2);
  border-color: transparent;
  background: rgba(255, 255, 255, 0.05);
}

.tool-icon {
  font-size: 3em;
  margin-bottom: 15px;
  display: block;
}

.tool-title {
  font-size: 1.5em;
  margin-bottom: 10px;
  background: linear-gradient(135deg, #00d2ff 0%, #3a7bd5 100%);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
}

.tool-description {
  opacity: 0.8;
  line-height: 1.6;
}

.coming-soon {
  display: inline-block;
  background: rgba(255, 200, 0, 0.2);
  color: #ffc800;
  padding: 5px 15px;
  border-radius: 20px;
  font-size: 0.8em;
  margin-top: 10px;
  font-weight: bold;
}
)rawliteral";

const char CSS_SCANNER_PAGE[] = R"rawliteral(
.back-button {
  display: inline-block;
  background: rgba(255, 255, 255, 0.1);
  padding: 10px 20px;
  border-radius: 10px;
  text-decoration: none;
  color: white;
  margin-bottom: 20px;
  transition: all 0.3s ease;
}

.back-button:hover {
  background: rgba(255, 255, 255, 0.2);
  transform: translateX(-5px);
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

.results-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
}

.results h2 {
  font-size: 1.8em;
  background: linear-gradient(135deg, #00d2ff 0%, #3a7bd5 100%);
  -webkit-background-clip: text;
  -webkit-text-fill-color: transparent;
  background-clip: text;
  display: inline-block;
}

.download-btn {
  background: linear-gradient(135deg, #00d2ff 0%, #3a7bd5 100%);
  border: none;
  color: white;
  padding: 10px 20px;
  font-size: 0.9em;
  border-radius: 8px;
  cursor: pointer;
  font-weight: bold;
  transition: transform 0.2s ease;
  box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);
  display: none;
}

.download-btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 6px 20px rgba(0, 0, 0, 0.3);
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

.device-ports {
  font-family: monospace;
  opacity: 0.7;
  font-size: 0.85em;
  margin-top: 3px;
  color: #00d2ff;
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
  0% {
    transform: rotate(0deg);
  }

  100% {
    transform: rotate(360deg);
  }
}

.empty {
  text-align: center;
  padding: 40px;
  font-size: 1.1em;
  opacity: 0.7;
}
)rawliteral";

#endif
