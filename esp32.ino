#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// wifi credentials
const char * ssid = "Galaxy M013154";
const char * password = "dfxz7900";

// aws iot core credentials
const char * mqtt_server = "a2vvufbiwbsbvo-ats.iot.ap-south-1.amazonaws.com";
const int mqtt_port = 8883;
const char * device_cert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAPFflgZLqCnOUgQkM/OoWratsZBPMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNTAxMTQwODE5
NThaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDw43XBmiyWVTa9L7OJ
45U6nZgrXztXJz9jrPPCntXvIR5B8YczZ/0rCPqHDVsC+ySvuB58xESu+YexADG6
fIC8KpZ3CdWocGzTMpRKhvIIP+0vzTI9+v5eoFpC/o9Kw2O2sBF7TNLgrCHJ5CbP
TXe+h1WN2eY6wY95kMvY0H3rSju/OinyVW43bsoNsG8DZfv4P6v/DSQ/mPXH2lfx
MoJYe5JX74etL3epLR1un+M0CmeSeotfuNz0kvef8pyM/Anz107LflC7LzwF+L56
Hudra8M8N0wD+nXD0rddSHaxgeamTTl3GOfVzoM+M+EkX9Txr51uijtRDvADGX7R
aNktAgMBAAGjYDBeMB8GA1UdIwQYMBaAFKKI0HpdplRhoZHvGwl2xw9Vc8BhMB0G
A1UdDgQWBBQzQRTDpQfW21hCqkmzz0jVGdjRzTAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAZKQoP5IW0iDLutB983R78yXB
ZVFfOPlpDOIl7N4QlRG5nYXyp3bg3izrsp7z2d2enC0QabxJVbckzJzjOM6xOP+M
A8phjwH+oiE+6QiAxsBbxPvxE0cxm7HZny0Vnxt+CtzkB8ft2Y5c7UEmu/u2RZFb
wS79Leyy5Bo986zBJieDUBRtwbKaSyAwJ0wJuqutO2CA5qTUoH6uYUdsRK3V9l1X
3QHDlcOdL3rI1bI+DIylA2OfXzI35ftUYcWGS42V9YbWH+gaJyQ6c/+AdaEMlOQL
egjg3aE8ZPfO3I1qR5vdBErbIab6AZNvFsj3HKVPP3FZqPf4DI8B3LTrCrlGoQ==
-----END CERTIFICATE-----
)EOF";;
const char * device_key = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEpgIBAAKCAQEA8ON1wZosllU2vS+zieOVOp2YK187Vyc/Y6zzwp7V7yEeQfGH
M2f9Kwj6hw1bAvskr7gefMRErvmHsQAxunyAvCqWdwnVqHBs0zKUSobyCD/tL80y
Pfr+XqBaQv6PSsNjtrARe0zS4KwhyeQmz013vodVjdnmOsGPeZDL2NB960o7vzop
8lVuN27KDbBvA2X7+D+r/w0kP5j1x9pX8TKCWHuSV++HrS93qS0dbp/jNApnknqL
X7jc9JL3n/KcjPwJ89dOy35Quy88Bfi+eh7na2vDPDdMA/p1w9K3XUh2sYHmpk05
dxjn1c6DPjPhJF/U8a+dboo7UQ7wAxl+0WjZLQIDAQABAoIBAQDE1SgQhVkvkfvD
hOnQn0z982426n8DjQdn/BfGjKFE36vouGIAaFVfR9vmZMxcWDi2+Ny4mGuDClSN
FhqMRlWu66G9LSUPmKXjwS2Y5EF66jl2WNZX/PLDAYYfvf/6WEPvMFlohfK+MQj+
/lWUCLnyzoTi/wz4L9d7vwxCYsKxcYYX1R6A0elpVaSxtozgPu9C4OwgbZogcR3i
WLChnQ6Yn1qMnPm+cv/QZwdiu0DWXjX8G2FsEfk3UzD42tEeTNrdRGh6m6//MqqS
sLo1UWTjAfz2X7pdgH/ZprLpBqjkU5n8XTXDwuo+1seGDSlq4quZHfCqOsxo06JO
bN2LWK6hAoGBAPvHWmtDFYUjsRXV34TKZnDgdRq+mr2M0f4Qzsq68Z6I5ZF5oVlJ
fcttr9Cfkm5pY2mWmfDgPAhEb97Yhsg2wEWb3IiHofLZ4kEM3tU3IR9ad5w9fHB7
Kwsgtq0aDBbo8NaKsYBQnsLf6iQEg44tN+EWPNUxB8XBMC+YTY6fydJVAoGBAPTt
XY+bSgPrG0qevl5cX+9IiEMQrnhbdd11Vt5K2DNw52BF1ER37lrlLHaFDCrkTxiq
xRMt5uJ1jhZnIjNR4Om4w5qjZLBli4UTLWbXFBYXaaqdUcFE8Te72I+QKSb2bt8R
2uuzB6MkiQKjO+IqL/eWRuJNfRLGZDLH0cGaJ7N5AoGBAMdpUwHVNfwnEqBarhd8
Prpz7Avsnp42JDN3mHEUsz1+62PW06XmAQ3XqAIyAEYWjjgYoYAX+TQLEBeNouJ8
CriXA4nW8rtnOj2PNgXlDgTJICbMw/xKcQL1O/nJwX17rRQe8QrRz4xE0CsKsC8x
PSq29CNeG+vSYJmxpOurPqupAoGBAN4Do8Ge0/LFVNHJH2rNdTCjoNCZ+GylCywZ
Rwrs8RmACFveMFF4HRlLj4hHhgYDOidz9Hw0KYuxX4CzL+8xEx0JHFW6MnUrNtMD
1S3ZXB3CT0gWnpsAcV9B6UYJOSRWUZ3mw3jPqeaaIY/Ag3G5KA3XrJ1J/Evehcj6
FArYP5CRAoGBAPnFslG9g77zEdMQRcMGHQk5a8Reyw2Bn9Vap3zb1pGtO9EX8b9E
zuSfOrYF0AQOttdajnQ8t9unp12tqc1h1XgOlBiJMZjrTpN/qWcbKId9eGinNDgI
qIpR7Da9LqBFYoym7aNmtk6zxX5HtkvsZNiJjhEthkz2W+KNRe3P7USS
-----END RSA PRIVATE KEY-----
)EOF";
const char * root_ca = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// mqtt topic
const char * topic = "helmet/cleaning/success";

WiFiClientSecure net;
PubSubClient client(net);

void setupWiFi() {
    delay(10);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
}

void connectAWS() {
    net.setCACert(root_ca);
    net.setCertificate(device_cert);
    net.setPrivateKey(device_key);

    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(messageReceived);

    while (!client.connected()) {
        Serial.print("Connecting to AWS IoT...");
        if (client.connect("ESP32Client")) {
            Serial.println("connected!");
            client.subscribe(topic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds...");
            delay(5000);
        }
    }
}


void messageReceived(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);

    String message;
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    Serial.println("Message payload: " + message);

    // Perform specific operations based on the message
    if (message.indexOf("payment_success") >= 0) {
        Serial.println("Payment success event received. Starting cleaning process...");
        // Add code for cleaning process here

    }
}

void setup() {
    delay(1000);
    Serial.begin(115200);

    // Connect to WiFi
    setupWiFi();

    // Synchronize time
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    while (!time(nullptr)) {
        delay(1000);
        Serial.println("Waiting for time...");
    }
    Serial.println("Time synchronized!");

    // Connect to AWS IoT
    connectAWS();
}


void loop() {
    delay(1000);
    if (!client.connected()) {
        connectAWS();
    }
    client.loop();
}

// #include <WiFi.h>

// // Replace with your WiFi credentials
// const char* ssid = "Ashmitha";
// const char* password = "88290799";

// void setup() {
//   delay(1000);
//   Serial.begin(115200);

//   // Connect to WiFi
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting to ");
//   Serial.println(ssid);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("\nWiFi connected!");
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());
// }

// void loop() {
//   // You can add your code here to do something after connecting to WiFi
//   delay(1000);
//   Serial.println("Hello from ESP32!");
// }
