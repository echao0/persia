
//------------NODE-----------------------------
String type = "relay"; // switch; relay
String espName = "taller";
const char* mqttUser = "taller";
const char* mqttPassword = "taller";

//------------IP/TCP-----------------------------

IPAddress ip(192,168,89,176); // where xx is the desired IP Address
IPAddress gateway(192,168,89,2); // set gateway to match your network
IPAddress subnet(255,255,255,0); // set subnet mask to match your

//------------SERVER-----------------------------

const char* mqttServer = "192.168.3.181";
const int mqttPort = 1883;

WiFiServer server(5000); // internal server port
