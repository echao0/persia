
//------------NODE-----------------------------
String type = "relay"; // switch; relay
String espName = "per2";
const char* mqttUser = "per2";
const char* mqttPassword = "per2";

//------------IP/TCP-----------------------------

IPAddress ip(192,168,3,172); // where xx is the desired IP Address
IPAddress gateway(0,0,0,0); // set gateway to match your network
IPAddress subnet(255,255,255,0); // set subnet mask to match your

//------------SERVER-----------------------------

const char* mqttServer = "192.168.3.181";
const int mqttPort = 1883;

WiFiServer server(5000); // internal server port
