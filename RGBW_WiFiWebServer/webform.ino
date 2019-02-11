void answerClient(WiFiClient client) {
 int current = 0;
 Serial.println("new client");           // print a message out the serial port
  String currentLine = "";                // make a String to hold incoming data from the client
  while (client.connected()) {            // loop while the client's connected
    if (client.available()) {             // if there's bytes to read from the client,
      char c = client.read();             // read a byte, then
      Serial.write(c);                    // print it out the serial monitor
      if (c == '\n') {                    // if the byte is a newline character

        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0) {
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line:
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          client.print("<a href=\"/fullWhite\">Full White</a><br>");
          client.print("<a href=\"/fullRed\">Full Red</a><br>");
          client.print("<a href=\"/whiteOverRainbow\">White over rainbow</a><br>");
          client.print("<a href=\"/rainbowFade2White\">Rainbow fade to white</a><br>");
          client.print("<a href=\"/rainbow\">Rainbow</a><br>");
          client.print("<a href=\"/rainbowCycle\">Rainbow Cycle</a><br>");
          client.print("Click <a href=\"/C\">here</a> next pattern<br>");
          // The HTTP response ends with another blank line:
          client.println();
          // break out of the while loop:
          break;
        }
        else {      // if you got a newline, then clear currentLine:
          currentLine = "";
        }
      }
      else if (c != '\r') {    // if you got anything else but a carriage return character,
        currentLine += c;      // add it to the end of the currentLine
      }

      if (currentLine.endsWith("GET /C")) {
        currentPattern = (currentPattern+1)%NUM_PATTERNS;           // GET /C change the pattern
        Serial.println(currentPattern);
      } else if (currentLine.endsWith("GET /fullWhite")) {
        currentPattern = 0;
        Serial.println(currentPattern);
      } else if (currentLine.endsWith("GET /whiteOverRainbow")) {
        currentPattern = 1;
        Serial.println(currentPattern);
      } else if (currentLine.endsWith("GET /rainbowFade2White")) {
        currentPattern = 2;
        Serial.println(currentPattern);
      } else if (currentLine.endsWith("GET /rainbow")) {
        currentPattern = 3;
        Serial.println(currentPattern);
      } else if (currentLine.endsWith("GET /rainbowCycle")) {
        currentPattern = 4;
        Serial.println(currentPattern);
      } else if (currentLine.endsWith("GET /fullRed")) {
        currentPattern = 5;
        Serial.println(currentPattern);
      }
 
    }
  }
  // close the connection:
  client.stop();
  Serial.println("client disonnected");
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
