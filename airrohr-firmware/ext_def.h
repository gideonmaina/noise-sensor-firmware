// Language config
#define CURRENT_LANG INTL_LANG

// Wifi config
const char WLANSSID[] PROGMEM = "Freifunk-disabled";
const char WLANPWD[] PROGMEM = "";

// BasicAuth config
const char WWW_USERNAME[] PROGMEM = "admin";
const char WWW_PASSWORD[] PROGMEM = "feinstaub";
#define WWW_BASICAUTH_ENABLED 0

// Sensor Wifi config (config mode)
#define FS_SSID "sensorsAFRICA-"
#define FS_PWD ""

// Where to send the data?
#define SEND2CFA 0
#define SSL_CFA 0
#define SEND2SENSORCOMMUNITY 0
#define SSL_SENSORCOMMUNITY 0
#define SEND2MADAVI 0
#define SSL_MADAVI 0
#define SEND2SENSEMAP 0
#define SEND2FSAPP 0
#define SEND2AIRCMS 0
#define SEND2MQTT 0
#define SEND2INFLUX 1
#define SEND2LORA 0
#define SEND2CSV 0
#define SEND2CUSTOM 0

// OpenSenseMap
#define SENSEBOXID ""

enum LoggerEntry
{
    LoggerCFA,
    LoggerSensorCommunity,
    LoggerMadavi,
    LoggerSensemap,
    LoggerFSapp,
    Loggeraircms,
    LoggerInflux,
    LoggerCustom,
    LoggerCount
};

struct LoggerConfig
{
    uint16_t destport;
    uint16_t _unused;
#if defined(ESP8266)
    BearSSL::Session *session;
#else
    void *session;
#endif
};

// IMPORTANT: NO MORE CHANGES TO VARIABLE NAMES NEEDED FOR EXTERNAL APIS
static const char HOST_CFA[] PROGMEM = "api.sensors.africa";
static const char URL_CFA[] PROGMEM = "/v1/push-sensor-data/";
#define PORT_CFA 80

static const char HOST_MADAVI[] PROGMEM = "api-rrd.madavi.de";
static const char URL_MADAVI[] PROGMEM = "/data.php";
#define PORT_MADAVI 80

static const char HOST_SENSORCOMMUNITY[] PROGMEM = "api.sensor.community";
static const char URL_SENSORCOMMUNITY[] PROGMEM = "/v1/push-sensor-data/";
#define PORT_SENSORCOMMUNITY 80

static const char HOST_SENSEMAP[] PROGMEM = "ingress.opensensemap.org";
static const char URL_SENSEMAP[] PROGMEM = "/boxes/{v}/data?luftdaten=1";
#define PORT_SENSEMAP 443

static const char HOST_FSAPP[] PROGMEM = "h2801469.stratoserver.net";
static const char URL_FSAPP[] PROGMEM = "/data.php";
#define PORT_FSAPP 443

static const char HOST_AIRCMS[] PROGMEM = "doiot.ru";
static const char URL_AIRCMS[] PROGMEM = "/php/sensors.php?h=";
// As of 2019/09 uses invalid certifiates on ssl/port 443 and does not support Maximum Fragment Length Negotiation (MFLN)
// So we can not use SSL
#define PORT_AIRCMS 80

static const char FW_DOWNLOAD_HOST[] PROGMEM = "firmware.sensor.community";
#define FW_DOWNLOAD_PORT 443

static const char FW_2ND_LOADER_URL[] PROGMEM = OTA_BASENAME "/loader-002.bin";

static const char NTP_SERVER_1[] PROGMEM = "0.pool.ntp.org";
static const char NTP_SERVER_2[] PROGMEM = "1.pool.ntp.org";

// define own API
static const char HOST_CUSTOM[] PROGMEM = "192.168.234.1";
static const char URL_CUSTOM[] PROGMEM = "/data.php";
#define PORT_CUSTOM 80
#define USER_CUSTOM ""
#define PWD_CUSTOM ""
#define SSL_CUSTOM 0

// define own InfluxDB
/* This settings can be updated on the web server

*/
// static const char HOST_INFLUX[] PROGMEM = "ec2-34-250-53-214.eu-west-1.compute.amazonaws.com";
// static const char URL_INFLUX[] PROGMEM = "/write?db=airquality";
static const char HOST_INFLUX[] PROGMEM = "192.168.0.100:8086";
static const char URL_INFLUX[] PROGMEM = "/write?db=sensors"; // ToDo update url with InfluxDB v2 api
#define PORT_INFLUX 8086
#define USER_INFLUX ""
#define PWD_INFLUX ""
static const char MEASUREMENT_NAME_INFLUX[] PROGMEM = "sensordata";
#define SSL_INFLUX 0

//  pin assignments for NodeMCU V2 board
#if defined(ESP8266)
// define pin for one wire sensors
#define ONEWIRE_PIN D0

// define pins for I2C
#define I2C_PIN_SCL D1
#define I2C_PIN_SDA D2

// define pins for GSM modules
// #define FONA_RX D1
// #define FONA_TX D2
// #define FONA_RST D3
#define FONA_RX 2
#define FONA_TX 0
#define FONA_RST 4

// define I2S pins for the SPH0645 MIC
#define I2SI_DATA 12 // I2S data on GPIO12
#define I2SI_BCK 13  // I2S clk on GPIO13
#define I2SI_WS 14   // I2S select on GPIO14
#endif

// pin assignments for Arduino SAMD Zero board
#if defined(ARDUINO_SAMD_ZERO)
#define ONEWIRE_PIN D7
#if defined(SERIAL_PORT_USBVIRTUAL)
#define RFM69_CS 8
#define RFM69_RST 4
#define RFM69_INT 3
#endif
#endif

// pin assignments for lolin_d32_pro board
#if defined(ARDUINO_LOLIN_D32_PRO)
#define ONEWIRE_PIN D32
#endif
// #define RFM69_CS D0
// #define RFM69_RST D2
// #define RFM69_INT D4

// pin assignments for heltec_wifi_lora_32_V2 board
#if defined(WIFI_LoRa_32_V2)
#define ONEWIRE_PIN D32
#define I2C_PIN_SCL D22
#define I2C_PIN_SDA D17_WROOM_ONLY
#define GPS_SERIAL_RX D13_JTAG_TCK
#define GPS_SERIAL_TX D0_STRAPPING
#endif

// pin assignments for heltec_wifi_lora_32 board
#if defined(WIFI_LoRa_32)
#define ONEWIRE_PIN D25 // TODO: this overlaps with LED, so it might not work
#define I2C_PIN_SCL D22
#define I2C_PIN_SDA D17_WROOM_ONLY
#define GPS_SERIAL_RX D13_JTAG_TCK
#define GPS_SERIAL_TX D0_STRAPPING
#endif

// Device is WiFi Enabled
#define WIFI_ENABLED 0

// SPH0645 MEMS Microphone
#define SPHO645_READ 1
#define SPH0645_API_PIN 15

// GSM
#define GSM_CAPABLE 1
#define GPRS_APN ""
#define GPRS_USERNAME ""
#define GPRS_PASSWORD ""
#define GSM_PIN ""

// DNMS Noise Measurement
#define DNMS_READ 0
#define DNMS_API_PIN 15
#define DNMS_CORRECTION "0.0"

// automatic firmware updates
#define AUTO_UPDATE 0

// use beta firmware
#define USE_BETA 0

// OLED Display SSD1306 connected?
#define HAS_DISPLAY 0

// OLED Display SH1106 connected?
#define HAS_SH1106 0

// OLED Display um 180° gedreht?
#define HAS_FLIPPED_DISPLAY 0

// LCD Display LCD1602 connected?
#define HAS_LCD1602 0

// LCD Display LCD1602 (0x27) connected?
#define HAS_LCD1602_27 0

// LCD Display LCD2004 connected?
#define HAS_LCD2004 0

// LCD Display LCD2004 (0x27) connected?
#define HAS_LCD2004_27 0

// Show wifi info on displays
#define DISPLAY_WIFI_INFO 1

// Show device info on displays
#define DISPLAY_DEVICE_INFO 1

// Set debug level for serial output?
#define DEBUG 3