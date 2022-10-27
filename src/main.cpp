#include <ESP8266WiFi.h>

const char* ssid            = "Telia-2G-A95121_2GEXT";
const char* password        = "94016037";

WiFiServer server(23);

#define DUMP_LENGTH (128 * 129)
char data[2][DUMP_LENGTH + 1] = {0};
char bank = 0;

// void build_data(void) {
//   for (size_t i = 0; i < 200; i++) {
//     unsigned int idx_start = i * 128;

//     unsigned char l = i % 10;
//     unsigned char m = (i / 10) % 10;
//     unsigned char h = (i / 100) % 10;

//     data[idx_start + 0] = '\r';
//     data[idx_start + 1] = '\n';
//     data[idx_start + 2] = '0' + h;
//     data[idx_start + 3] = '0' + m;
//     data[idx_start + 4] = '0' + l;
//     for (size_t j = 5; j < 128; j++) {
//       data[idx_start + j] = '.';
//     }

//     Serial.print((const char*) &data[idx_start]);
//     delay(100);
//   }
// }

void setup()
{
  Serial.begin(460800);
  Serial.setDebugOutput(true);
  Serial.println();
  // Reduce power to ensure USB has enough power
  WiFi.setOutputPower(0);

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESPsoftAP_01", "94016037", 2, false, 1) ? "Ready" : "Failed!");

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n",
                WiFi.localIP().toString().c_str());

  // build_data();
}

void loop()
{
  uint16_t counter = 0;

  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      if (counter >= (128 * 128)) {
        data[bank][counter] = 0;
        const char* pbuf = (const char*)data[bank];
        client.print(pbuf);

        bank = bank ? 0 : 1;
        counter = 0;
      }

      while (Serial.available() > 0) {
        data[bank][counter++] = Serial.read();
      }
    }

    // close the connection:
    client.stop();
    Serial.println("[Client disconnected]");
  }

}