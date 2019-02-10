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
          client.print("Click <a href=\"/H\">here</a> turn the LED on pin 9 on<br>");
          client.print("Click <a href=\"/L\">here</a> turn the LED on pin 9 off<br>");
          client.print("Click <a href=\"/C\">here</a> next pattern<br>");
          client.print("Click <a href=\"/A\">here</a> ArtNet<br>");
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

      // Check to see if the client request was "GET /H" or "GET /L":
      if (currentLine.endsWith("GET /H")) {
        digitalWrite(13, HIGH);               // GET /H turns the LED on
      }
      if (currentLine.endsWith("GET /L")) {
        digitalWrite(13, LOW);                // GET /L turns the LED off
      }
      if (currentLine.endsWith("GET /C")) {
        current = (current+1)%3;           // GET /C change the pattern
        Serial.println(current);
      }
      if (currentLine.endsWith("GET /A")) {
        current = 2;           // GET /A change the pattern
        Serial.println(current);
      }      
    }
  }
  // close the connection:
  client.stop();
  Serial.println("client disonnected");
}

