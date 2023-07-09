#include <Ethernet.h>
#define sensorPower 7
#define sensorPin A0

int val = 0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 20);  //192.168.1.101
EthernetServer server(80);

void setup() {
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);
  Serial.begin(9600);

  pinMode(6, INPUT);
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop() {
  int level = readSensor();
  int percentValue = ((level * 100) / 500);
  String pixelValue = String((percentValue * 600) / 100);
  

  Serial.print("Water level: ");
  Serial.println(level);
  Serial.print("percent value: ");
  Serial.println(percentValue);
  Serial.print("Pixel value: ");
  Serial.println(pixelValue);

  EthernetClient client = server.available();
  if (client) {
    boolean linhaEmBranco = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (c == '\n' && linhaEmBranco) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("<!DOCTYPE HTML>");
          client.println();
          client.println("<html lang=\"pt-br\">");
          client.println("<head>");
          client.println("<meta charset=\"UTF-8\">");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
          client.println("<title>Nivel da Água</title>");
          client.println("</head>");
          client.println("<body style=\"overflow: hidden;\">");
          client.println("<div  style=\"width: 100vw; height: 100vh; display: flex; flex-direction: column; justify-content: start; align-items: center;\">");
          client.println("<h1>Nivel da Água no Reservatorio</h1>");
          client.println("<div style=\"display: flex;\" >");
          client.println("<div style=\"display: flex; flex-direction: column; align-items: center;\" >");
          client.println("<div style=\"height: 10px; width: 100px; background-color: black; margin-right: 50px; display: flex; justify-content: center; align-items: center;\">");
          client.println("<span style=\"color: white; font-size: 10px;\">100%</span>");
          client.println("</div>");
          client.println("<div style=\"height: 290px; width: 10px; background-color: black; margin-right: 50px;\" ></div>");
          client.println("<div style=\"height: 10px; width: 100px; background-color: black; margin-right: 50px; display: flex; justify-content: center; align-items: center;\">");
          client.println("<span style=\"color: white; font-size: 10px;\">50%</span>");
          client.println("</div>");
          client.println("<div style=\"height: 290px; width: 10px; background-color: black; margin-right: 50px;\" ></div>");
          client.println("<div style=\"height: 10px; width: 100px; background-color: black; margin-right: 50px; display: flex; justify-content: center; align-items: center;\">");
          client.println("<span style=\"color: white; font-size: 10px;\">0%</span>");
          client.println("</div>");
          client.println("</div>");
          client.println("<div style=\"border: 2px solid black; width: 300px; height: 600px; border-bottom-left-radius: 50px; border-bottom-right-radius: 50px; overflow: hidden; display: flex; flex-direction: column; justify-content: end; align-items: center; \">");
          client.println("<div style='width: 300px; background-color: aqua; height: " + pixelValue + "px;'/>");
          client.println("</div>");
          client.println("</div>");
          client.println("</div>");
          // client.println("<script> document.addEventListener('DOMContentLoaded', function(event) { setTimeout(() => window.location.reload(), 1000) }) </script>");
          client.println("</body>");
          client.println("</html>");
          break;
        }

        if (c == '\n') {
          linhaEmBranco = true;
        } else if (c != '\r') {
          linhaEmBranco = false;
        }
      }
    }
  }

  delay(1000);
}

int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);                        // wait 10 milliseconds
  val = analogRead(sensorPin);      // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;                       // send current reading
}
