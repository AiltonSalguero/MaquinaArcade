// Service
#include <HTTPClient.h>

// TOUCH
#define ctsPin 4

// SERVO
#include <Servo.h>
Servo servo;
int angle = 10;
int pinServo = 7;

// Ethernet
#include <EtherCard.h>
#define APIKEY "QTRR4654FRE3"
#define STATIC 0

#if STATIC
static byte myip[] = { 192, 168, 1, 200 };
static byte gwip[] = { 192, 168, 1, 1 };
#endif
static byte mymac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 };

byte Ethernet::buffer[500];
BufferFiller bfill;

// Programa
bool pagado = false;


void setup() {
  Serial.begin(57600);
  // TOUCH
  pinMode(ctsPin, INPUT);

  // SERVO
  servo.attach(pinServo);
  servo.write(angle);

  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println( "Failed to access Ethernet controller");
#if STATIC
  ether.staticSetup(myip, gwip);
#else
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
#endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);
}


void loop() {
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

  if (pos) {
    ether.httpServerReply(homePage(pagado));
  }
  // TOUCH
  int ctsValue = digitalRead(ctsPin);
  if (ctsValue == HIGH) {
    Serial.println("\nTouched");
    pagado = true; // CAMBIA LA VISTA
  }
}


static word homePage(bool pagado) {
  long t = millis() / 1000;
  word h = t / 3600;
  byte m = (t / 60) % 60;
  byte s = t % 60;
  bfill = ether.tcpOffset();
  if (pagado) {
    bfill.emit_p(PSTR(
                   "HTTP/1.0 200 OK\r\n"
                   "Content-Type: text/html\r\n"
                   "Pragma: no-cache\r\n"
                   "\r\n"
                   "<meta http-equiv='refresh' content='1'/>"
                   "<head>"
                   "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/bulma/0.7.5/css/bulma.min.css\">"
                   "<title>Sistemas Electricos</title>"
                   "</head>"
                   "<body>"
                   "<section class=\"section\">"
                   "<div class=\"container\">"
                   "<button class=\"button\" onclick=\"window.location.href = 'http://www.friv.com/';\">Jugar</button>"
                   "<h1>Inicie su juego</h1>"
                   "</section>"
                   "</body>"
                 ));
  } else {
    bfill.emit_p(PSTR(
                   "HTTP/1.0 200 OK\r\n"
                   "Content-Type: text/html\r\n"
                   "Pragma: no-cache\r\n"
                   "\r\n"
                   "<meta http-equiv='refresh' content='1'/>"
                   "<head>"
                   "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/bulma/0.7.5/css/bulma.min.css\">"
                   "<title>Sistemas Electricos</title>"
                   "<h1>$D$D:$D$D:$D$D</h1>"
                   "</head>"
                   "<body>"
                   "<section class=\"section\">"
                   "<div class=\"container\">"
                   "<h1 class=\"title\">ARCADE GAMES ARDUINO</h1>"
                   "<p class=\"subtitle\">Inserte su moneda.<strong> Gracias</strong>!</p>"
                   "</div>"
                   "</section>"
                   "</body>"
                 ),
                 h / 10, h % 10, m / 10, m % 10, s / 10, s % 10);
  }
  return bfill.position();
}

static word linkToGame() {
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
                 "HTTP/1.0 200 OK\r\n"
                 "Content-Type: text/html\r\n"
                 "Pragma: no-cache\r\n"
                 "\r\n"
                 "<meta http-equiv='refresh' content='1'/>"
                 "<head>"
                 "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/bulma/0.7.5/css/bulma.min.css\">"
                 "<title>Sistemas Electricos</title>"
                 "</head>"
                 "<body>"
                 "<section class=\"section\">"
                 "<div class=\"container\">"

                 "<button class=\"button\" onclick=\"window.location.href = 'http://www.friv.com/';\">Jugar</button>"
                 "<h1>Inicie su juego</h1>"
                 "</div>"
                 "</section>"
                 "</body>"
               ));
  return bfill.position();
}

void obtenerDatos() {
  HTTPClient http;

  http.begin("http://localhost:9002/api/tasks"); //Specify the URL
  int httpCode = http.GET();                                        //Make the request

  if (httpCode > 0) { //Check for the returning code

    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
  }

  else {
    Serial.println("Error on HTTP request");
  }

  http.end(); //Free the resources
}

void moverMotor() {
  for (angle = 10; angle < 180; angle++)
  {
    servo.write(angle);
    delay(1);
  }
  // now scan back from 180 to 0 degrees
  for (angle = 180; angle > 10; angle--)
  {
    servo.write(angle);
    delay(1);
  }
}
