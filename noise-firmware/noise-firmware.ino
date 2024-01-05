/************************************************************************
 *                                                                      *
 *  This source code needs to be compiled for the board                 *
 *  NodeMCU 1.0 (ESP-12E Module)                                        *
 *                                                                      *
 ************************************************************************
 *                                                                      *
 *    airRohr firmware                                                  *
 *    Copyright (C) 2016-2020  Code for Stuttgart a.o.                  *
 *    Copyright (C) 2019-2020  Dirk Mueller                             *
 *                                                                      *
 * This program is free software: you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with this program. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                      *
 ************************************************************************
 * OK LAB Particulate Matter Sensor                                     *
 *      - nodemcu-LoLin board                                           *
 *                                                                      *
 *                                                                      *
 *                                                                      *
 * Wiring Instruction see included Readme.md                            *
 *                                                                      *
 ************************************************************************
 *                                                                      *
 * Alternative                                                          *
 *      - nodemcu-LoLin board                                           *
 *                                                                      *
 *                                                                      *
 *                                                                      *
 ************************************************************************
 *                                                                      *
 * Please check Readme.md for other sensors and hardware                *
 *                                                                      *
 ************************************************************************
 *
 * latest mit lib 2.6.1
 * DATA:    [====      ]  40.7% (used 33316 bytes from 81920 bytes)
 * PROGRAM: [=====     ]  49.3% (used 514788 bytes from 1044464 bytes)

 * latest mit lib 2.5.2
 * DATA:    [====      ]  39.4% (used 32304 bytes from 81920 bytes)
 * PROGRAM: [=====     ]  48.3% (used 504812 bytes from 1044464 bytes)
 *
 ************************************************************************/
#include <WString.h>
#include <pgmspace.h>

// increment on change
#define SOFTWARE_VERSION_STR "NRZ-2020-129"
String SOFTWARE_VERSION(SOFTWARE_VERSION_STR);

/*****************************************************************
 * Includes                                                      *
 *****************************************************************/

#if defined(ESP8266)
#include <FS.h> // must be first
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>
#include <Hash.h>
#include <ctime>
#include <coredecls.h>
#include <sntp.h>
#endif

#if defined(ESP32)
#define FORMAT_SPIFFS_IF_FAILED true
#include <FS.h>
#include <HTTPClient.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <HardwareSerial.h>
#include <hwcrypto/sha.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#endif

// includes common to ESP8266 and ESP32 (especially external libraries)
#include "commons.h"

#include <algorithm>			// Used for the Noise maximum filter
#include "languages/lang_def.h" // include language definitions
#include "defines.h"
#include "ext_def.h"
#include "html-content.h"
#include "ca-root.h"

/******************************************************************
 * Constants                                                      *
 ******************************************************************/
constexpr unsigned SMALL_STR = 64 - 1;
constexpr unsigned MED_STR = 256 - 1;
constexpr unsigned LARGE_STR = 512 - 1;
constexpr unsigned XLARGE_STR = 1024 - 1;

#define RESERVE_STRING(name, size)      \
	String name((const char *)nullptr); \
	name.reserve(size)

const unsigned long DISPLAY_UPDATE_INTERVAL_MS = 5000; // time between switching display to next "screen"
const unsigned long ONE_DAY_IN_MS = 24 * 60 * 60 * 1000;
const unsigned long PAUSE_BETWEEN_UPDATE_ATTEMPTS_MS = ONE_DAY_IN_MS;		// check for firmware updates once a day
const unsigned long DURATION_BEFORE_FORCED_RESTART_MS = ONE_DAY_IN_MS * 28; // force a reboot every ~4 weeks

/******************************************************************
 * The variables inside the cfg namespace are persistent          *
 * configuration values. They have defaults which can be          *
 * configured at compile-time via the ext_def.h file              *
 * They can be changed by the user via the web interface, the     *
 * changes are persisted to the flash and read back after reboot. *
 * Note that the names of these variables can't be easily changed *
 * as they are part of the json format used to persist the data.  *
 ******************************************************************/
namespace cfg
{
	unsigned debug = DEBUG;

	unsigned time_for_wifi_config = 60000;
	unsigned sending_intervall_ms = 30000;

	char current_lang[3];

	// credentials for basic auth of internal web server
	bool www_basicauth_enabled = WWW_BASICAUTH_ENABLED;
	char www_username[LEN_WWW_USERNAME];
	char www_password[LEN_CFG_PASSWORD];

	// wifi credentials
	char wlanssid[LEN_WLANSSID];
	char wlanpwd[LEN_CFG_PASSWORD];

	// credentials of the sensor in access point mode
	char fs_ssid[LEN_FS_SSID] = FS_SSID;
	char fs_pwd[LEN_CFG_PASSWORD] = FS_PWD;

	// (in)active sensors
	bool sph0645_read = SPHO645_READ;
	bool gsm_capable = GSM_CAPABLE;
	bool dnms_read = DNMS_READ;
	char dnms_correction[LEN_DNMS_CORRECTION] = DNMS_CORRECTION;

	// send to "APIs"
	bool send2cfa = SEND2CFA;
	bool send2dusti = SEND2SENSORCOMMUNITY;
	bool send2madavi = SEND2MADAVI;
	bool send2sensemap = SEND2SENSEMAP;
	bool send2fsapp = SEND2FSAPP;
	bool send2aircms = SEND2AIRCMS;
	bool send2custom = SEND2CUSTOM;
	bool send2influx = SEND2INFLUX;
	bool send2csv = SEND2CSV;

	bool auto_update = AUTO_UPDATE;
	bool use_beta = USE_BETA;

	bool wifi_enabled = WIFI_ENABLED;

	// (in)active displays
	bool has_display = HAS_DISPLAY; // OLED with SSD1306 and I2C
	bool has_sh1106 = HAS_SH1106;
	bool has_flipped_display = HAS_FLIPPED_DISPLAY;
	bool has_lcd1602 = HAS_LCD1602;
	bool has_lcd1602_27 = HAS_LCD1602_27;
	bool has_lcd2004 = HAS_LCD2004;
	bool has_lcd2004_27 = HAS_LCD2004_27;

	bool display_wifi_info = DISPLAY_WIFI_INFO;
	bool display_device_info = DISPLAY_DEVICE_INFO;

	// API settings
	bool ssl_cfa = SSL_CFA;
	bool ssl_madavi = SSL_MADAVI;
	bool ssl_dusti = SSL_SENSORCOMMUNITY;
	char senseboxid[LEN_SENSEBOXID] = SENSEBOXID;

	char host_influx[LEN_HOST_INFLUX];
	char url_influx[LEN_URL_INFLUX];
	unsigned port_influx = PORT_INFLUX;
	char user_influx[LEN_USER_INFLUX] = USER_INFLUX;
	char pwd_influx[LEN_CFG_PASSWORD] = PWD_INFLUX;
	char measurement_name_influx[LEN_MEASUREMENT_NAME_INFLUX];
	bool ssl_influx = SSL_INFLUX;

	char host_custom[LEN_HOST_CUSTOM];
	char url_custom[LEN_URL_CUSTOM];
	bool ssl_custom = SSL_CUSTOM;
	unsigned port_custom = PORT_CUSTOM;
	char user_custom[LEN_USER_CUSTOM] = USER_CUSTOM;
	char pwd_custom[LEN_CFG_PASSWORD] = PWD_CUSTOM;

	char gsm_pin[LEN_GSM_PIN] = GSM_PIN;
	char gprs_apn[LEN_GPRS_APN] = GPRS_APN;
	char gprs_username[LEN_GPRS_USERNAME] = GPRS_USERNAME;
	char gprs_password[LEN_GPRS_PASSWORD] = GPRS_PASSWORD;

	void initNonTrivials(const char *id)
	{
		strcpy(cfg::current_lang, CURRENT_LANG);
		strcpy_P(www_username, WWW_USERNAME);
		strcpy_P(www_password, WWW_PASSWORD);
		strcpy_P(wlanssid, WLANSSID);
		strcpy_P(wlanpwd, WLANPWD);
		strcpy_P(host_custom, HOST_CUSTOM);
		strcpy_P(url_custom, URL_CUSTOM);
		strcpy_P(host_influx, HOST_INFLUX);
		strcpy_P(url_influx, URL_INFLUX);
		strcpy_P(measurement_name_influx, MEASUREMENT_NAME_INFLUX);
		strcpy_P(gsm_pin, GSM_PIN);
		strcpy_P(gprs_apn, GPRS_APN);
		strcpy_P(gprs_username, GPRS_USERNAME);
		strcpy_P(gprs_password, GPRS_USERNAME);

		if (!*fs_ssid)
		{
			strcpy(fs_ssid, SSID_BASENAME);
			strcat(fs_ssid, id);
		}
		else
		{
			strcat(fs_ssid, id);
		}
	}
}

#define JSON_BUFFER_SIZE 2300

LoggerConfig loggerConfigs[LoggerCount];

long int sample_count = 0;
bool dnms_init_failed = false;
bool airrohr_selftest_failed = false;

#if defined(ESP8266)
ESP8266WebServer server(80);
#endif
#if defined(ESP32)
WebServer server(80);
#endif

#include "./airrohr-cfg.h"

/*****************************************************************
 * Variables for Noise Measurement DNMS                          *
 *****************************************************************/
float last_value_dnms_laeq = -1.0;
float last_value_dnms_la_min = -1.0;
float last_value_dnms_la_max = -1.0;

/*****************************************************************
/* GSM declaration                                               *
/*****************************************************************/
#if defined(ESP8266)
SoftwareSerial fonaSS(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t GSM_CONNECTED = 1;
uint8_t GPRS_CONNECTED = 1;

bool gsm_capable = 1;
char gsm_pin[5] = "";

char gprs_apn[100] = "internet";
char gprs_username[100] = "";
char gprs_password[100] = "";
#endif

/*****************************************************************
 * Display definitions                                           *
 *****************************************************************/
SSD1306 display(0x3c, I2C_PIN_SDA, I2C_PIN_SCL);
SH1106 display_sh1106(0x3c, I2C_PIN_SDA, I2C_PIN_SCL);
LiquidCrystal_I2C *lcd_1602 = nullptr;
LiquidCrystal_I2C *lcd_2004 = nullptr;

bool send_now = false;
unsigned long starttime;
unsigned long time_point_device_start_ms;
unsigned long act_micro;
unsigned long act_milli;
unsigned long last_micro = 0;
unsigned long min_micro = 1000000000;
unsigned long max_micro = 0;

unsigned long sending_time = 0;
unsigned long last_update_attempt;
int last_update_returncode;
int last_sendData_returncode;

// Variable to store SPH0645 Mic value
float value_SPH0645 = 0.0;

/* Variables for Sound Pressure Level Filtering */
#define SAMPLE_SIZE 200
float sample[SAMPLE_SIZE];

int last_signal_strength;

String esp_chipid;

unsigned long sendData_error_count;
unsigned long WiFi_error_count;

unsigned long last_page_load = millis();

bool wificonfig_loop = false;
uint8_t sntp_time_set;

unsigned long count_sends = 0;
unsigned long last_display_millis = 0;
uint8_t next_display_count = 0;

String last_data_string;

struct struct_wifiInfo
{
	char ssid[LEN_WLANSSID];
	uint8_t encryptionType;
	int32_t RSSI;
	int32_t channel;
#if defined(ESP8266)
	bool isHidden;
	uint8_t unused[3];
#endif
};

struct struct_wifiInfo *wifiInfo;
uint8_t count_wifiInfo;

template <typename T, std::size_t N>
constexpr std::size_t array_num_elements(const T (&)[N])
{
	return N;
}

#define msSince(timestamp_before) (act_milli - (timestamp_before))

const char data_first_part[] PROGMEM = "{\"software_version\": \"" SOFTWARE_VERSION_STR "\", \"sensordatavalues\":[";
const char JSON_SENSOR_DATA_VALUES[] PROGMEM = "sensordatavalues";

/*****************************************************************
 * Debug output                                                  *
 *****************************************************************/

#define debug_level_check(level) \
	{                            \
		if (level > cfg::debug)  \
			return;              \
	}

static void debug_out(const String &text, unsigned int level)
{
	debug_level_check(level);
	Serial.print(text);
}

static void debug_out(const __FlashStringHelper *text, unsigned int level)
{
	debug_level_check(level);
	Serial.print(text);
}

static void debug_outln(const String &text, unsigned int level)
{
	debug_level_check(level);
	Serial.println(text);
}

static void debug_outln_info(const String &text)
{
	debug_level_check(DEBUG_MIN_INFO);
	Serial.println(text);
}

static void debug_outln_verbose(const String &text)
{
	debug_level_check(DEBUG_MED_INFO);
	Serial.println(text);
}

static void debug_outln_error(const __FlashStringHelper *text)
{
	debug_level_check(DEBUG_ERROR);
	Serial.println(text);
}

static void debug_outln_info(const __FlashStringHelper *text)
{
	debug_level_check(DEBUG_MIN_INFO);
	Serial.println(text);
}

static void debug_outln_verbose(const __FlashStringHelper *text)
{
	debug_level_check(DEBUG_MED_INFO);
	Serial.println(text);
}

static void debug_outln_info(const __FlashStringHelper *text, const String &option)
{
	debug_level_check(DEBUG_MIN_INFO);
	Serial.print(text);
	Serial.println(option);
}

static void debug_outln_info(const __FlashStringHelper *text, float value)
{
	debug_outln_info(text, String(value));
}

static void debug_outln_verbose(const __FlashStringHelper *text, const String &option)
{
	debug_level_check(DEBUG_MED_INFO);
	Serial.print(text);
	Serial.println(option);
}

static void debug_outln_info_bool(const __FlashStringHelper *text, const bool option)
{
	debug_level_check(DEBUG_MIN_INFO);
	Serial.print(text);
	Serial.println(String(option));
}

#undef debug_level_check

/*****************************************************************
 * display values                                                *
 *****************************************************************/
static void display_debug(const String &text1, const String &text2)
{
	debug_outln_info(F("output debug text to displays..."));
	if (cfg::has_display)
	{
		display.clear();
		display.displayOn();
		display.setTextAlignment(TEXT_ALIGN_LEFT);
		display.drawString(0, 12, text1);
		display.drawString(0, 24, text2);
		display.display();
	}
	if (cfg::has_sh1106)
	{
		display_sh1106.clear();
		display_sh1106.displayOn();
		display_sh1106.setTextAlignment(TEXT_ALIGN_LEFT);
		display_sh1106.drawString(0, 12, text1);
		display_sh1106.drawString(0, 24, text2);
		display_sh1106.display();
	}
	if (lcd_1602)
	{
		lcd_1602->clear();
		lcd_1602->setCursor(0, 0);
		lcd_1602->print(text1);
		lcd_1602->setCursor(0, 1);
		lcd_1602->print(text2);
	}
	if (lcd_2004)
	{
		lcd_2004->clear();
		lcd_2004->setCursor(0, 0);
		lcd_2004->print(text1);
		lcd_2004->setCursor(0, 1);
		lcd_2004->print(text2);
	}
}

/*****************************************************************
/* flushSerial                                                   *
/*****************************************************************/
void flushSerial()
{
	while (fonaSS.available())
		fonaSS.read();
}

/*****************************************************************
 * check display values, return '-' if undefined                 *
 *****************************************************************/
static String check_display_value(double value, double undef, uint8_t len, uint8_t str_len)
{
	RESERVE_STRING(s, 15);
	s = (value != undef ? String(value, len) : String("-"));
	while (s.length() < str_len)
	{
		s = " " + s;
	}
	return s;
}

/*****************************************************************
 * add value to json string                                  *
 *****************************************************************/
static void add_Value2Json(String &res, const __FlashStringHelper *type, const String &value)
{
	RESERVE_STRING(s, SMALL_STR);

	s = F("{\"value_type\":\"{t}\",\"value\":\"{v}\"},");
	s.replace("{t}", String(type));
	s.replace("{v}", value);
	res += s;
}

static void add_Value2Json(String &res, const __FlashStringHelper *type, const __FlashStringHelper *debug_type, const float &value)
{
	debug_outln_info(FPSTR(debug_type), value);
	add_Value2Json(res, type, String(value));
}

/***************************************************************
	SPIFFS Config : init, read, write
***************************************************************/
#include "spiffs-config.h"

/*****************************************************************
 * Prepare information for data Loggers                          *
 *****************************************************************/
static void createLoggerConfigs()
{
#if defined(ESP8266)
	auto new_session = []()
	{ return new BearSSL::Session; };
#else
	auto new_session = []()
	{ return nullptr; };
#endif
	if (cfg::send2cfa)
	{
		loggerConfigs[LoggerCFA].destport = 80;
		if (cfg::ssl_cfa)
		{
			loggerConfigs[LoggerCFA].destport = 80;
			loggerConfigs[LoggerCFA].session = new_session();
		}
	}
	if (cfg::send2dusti)
	{
		loggerConfigs[LoggerSensorCommunity].destport = 80;
		if (cfg::ssl_dusti)
		{
			loggerConfigs[LoggerSensorCommunity].destport = 443;
			loggerConfigs[LoggerSensorCommunity].session = new_session();
		}
	}
	loggerConfigs[LoggerMadavi].destport = PORT_MADAVI;
	if (cfg::send2madavi && cfg::ssl_madavi)
	{
		loggerConfigs[LoggerMadavi].destport = 443;
		loggerConfigs[LoggerMadavi].session = new_session();
	}
	loggerConfigs[LoggerSensemap].destport = PORT_SENSEMAP;
	loggerConfigs[LoggerSensemap].session = new_session();
	loggerConfigs[LoggerFSapp].destport = PORT_FSAPP;
	loggerConfigs[LoggerFSapp].session = new_session();
	loggerConfigs[Loggeraircms].destport = PORT_AIRCMS;
	loggerConfigs[LoggerInflux].destport = cfg::port_influx;
	if (cfg::send2influx && cfg::ssl_influx)
	{
		loggerConfigs[LoggerInflux].session = new_session();
	}
	loggerConfigs[LoggerCustom].destport = cfg::port_custom;
	if (cfg::send2custom && (cfg::ssl_custom || (cfg::port_custom == 443)))
	{
		loggerConfigs[LoggerCustom].session = new_session();
	}
}

/*****************************************************************
 * aircms.online helper functions                                *
 *****************************************************************/
static String sha1Hex(const String &s)
{
	char sha1sum_output[20];

#if defined(ESP8266)
	br_sha1_context sc;

	br_sha1_init(&sc);
	br_sha1_update(&sc, s.c_str(), s.length());
	br_sha1_out(&sc, sha1sum_output);
#endif
#if defined(ESP32)
	esp_sha(SHA1, (const unsigned char *)s.c_str(), s.length(), (unsigned char *)sha1sum_output);
#endif
	String r;
	for (uint16_t i = 0; i < 20; i++)
	{
		char hex[3];
		snprintf(hex, sizeof(hex), "%02x", sha1sum_output[i]);
		r += hex;
	}
	return r;
}

static String hmac1(const String &secret, const String &s)
{
	String str = sha1Hex(s);
	str = secret + str;
	return sha1Hex(str);
}

// ToDo Refactor Webserver Functions
/***************************************************************
	Webserver Configurations
***************************************************************/
#include "webserver-config.h"

/*****************************************************************
 * WifiConfig                                                    *
 *****************************************************************/
static void wifiConfig()
{
	debug_outln_info(F("Starting WiFiManager"));
	debug_outln_info(F("AP ID: "), String(cfg::fs_ssid));
	debug_outln_info(F("Password: "), String(cfg::fs_pwd));

	wificonfig_loop = true;

	WiFi.disconnect(true);
	debug_outln_info(F("scan for wifi networks..."));
	count_wifiInfo = WiFi.scanNetworks(false /* scan async */, true /* show hidden networks */);
	delete[] wifiInfo;
	wifiInfo = new struct_wifiInfo[count_wifiInfo];

	for (int i = 0; i < count_wifiInfo; i++)
	{
		String SSID;
		uint8_t *BSSID;

		memset(&wifiInfo[i], 0, sizeof(struct_wifiInfo));
#if defined(ESP8266)
		WiFi.getNetworkInfo(i, SSID, wifiInfo[i].encryptionType,
							wifiInfo[i].RSSI, BSSID, wifiInfo[i].channel,
							wifiInfo[i].isHidden);
#else
		WiFi.getNetworkInfo(i, SSID, wifiInfo[i].encryptionType,
							wifiInfo[i].RSSI, BSSID, wifiInfo[i].channel);
#endif
		SSID.toCharArray(wifiInfo[i].ssid, sizeof(wifiInfo[0].ssid));
	}

	WiFi.mode(WIFI_AP);
	const IPAddress apIP(192, 168, 4, 1);
	WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
	WiFi.softAP(cfg::fs_ssid, cfg::fs_pwd, selectChannelForAp());
	// In case we create a unique password at first start
	debug_outln_info(F("AP Password is: "), cfg::fs_pwd);

	DNSServer dnsServer;
	// Ensure we don't poison the client DNS cache
	dnsServer.setTTL(0);
	dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
	dnsServer.start(53, "*", apIP); // 53 is port for DNS server

	setup_webserver();

	// 10 minutes timeout for wifi config
	last_page_load = millis();
	while ((millis() - last_page_load) < cfg::time_for_wifi_config + 500)
	{
		dnsServer.processNextRequest();
		server.handleClient();
#if defined(ESP8266)
		wdt_reset(); // nodemcu is alive
		MDNS.update();
#endif
		yield();
	}

	WiFi.softAPdisconnect(true);
	WiFi.mode(WIFI_STA);

	dnsServer.stop();
	delay(100);

	debug_outln_info(FPSTR(DBG_TXT_CONNECTING_TO), cfg::wlanssid);

	WiFi.begin(cfg::wlanssid, cfg::wlanpwd);

	debug_outln_info(F("---- Result Webconfig ----"));
	debug_outln_info(F("WLANSSID: "), cfg::wlanssid);
	debug_outln_info(FPSTR(DBG_TXT_SEP));
	debug_outln_info_bool(F("GSM: "), cfg::gsm_capable);
	debug_outln_info_bool(F("DNMS: "), cfg::dnms_read);
	debug_outln_info(FPSTR(DBG_TXT_SEP));
	debug_outln_info_bool(F("SensorCommunity: "), cfg::send2dusti);
	debug_outln_info_bool(F("Madavi: "), cfg::send2madavi);
	debug_outln_info_bool(F("CSV: "), cfg::send2csv);
	debug_outln_info(FPSTR(DBG_TXT_SEP));
	debug_outln_info_bool(F("Autoupdate: "), cfg::auto_update);
	debug_outln_info_bool(F("Display: "), cfg::has_display);
	debug_outln_info_bool(F("LCD 1602: "), !!lcd_1602);
	debug_outln_info(F("Debug: "), String(cfg::debug));
	wificonfig_loop = false;
}

static void waitForWifiToConnect(int maxRetries)
{
	int retryCount = 0;
	while ((WiFi.status() != WL_CONNECTED) && (retryCount < maxRetries))
	{
		delay(500);
		debug_out(".", DEBUG_MIN_INFO);
		++retryCount;
	}
}

/*****************************************************************
 * WiFi auto connecting script                                   *
 *****************************************************************/
static void connectWifi()
{
	display_debug(F("Connecting to"), String(cfg::wlanssid));
#if defined(ESP8266)
	// Enforce Rx/Tx calibration
	system_phy_set_powerup_option(1);
	// 20dBM == 100mW == max tx power allowed in europe
	WiFi.setOutputPower(20.0f);
	WiFi.setSleepMode(WIFI_NONE_SLEEP);
	WiFi.setPhyMode(WIFI_PHY_MODE_11N);
	delay(100);
#endif
	if (WiFi.getAutoConnect())
	{
		WiFi.setAutoConnect(false);
	}
	if (!WiFi.getAutoReconnect())
	{
		WiFi.setAutoReconnect(true);
	}
	WiFi.mode(WIFI_STA);
	WiFi.hostname(cfg::fs_ssid);
	WiFi.begin(cfg::wlanssid, cfg::wlanpwd); // Start WiFI

	debug_outln_info(FPSTR(DBG_TXT_CONNECTING_TO), cfg::wlanssid);

	waitForWifiToConnect(40);
	debug_outln_info(emptyString);
	if (WiFi.status() != WL_CONNECTED)
	{
		String fss(cfg::fs_ssid);
		display_debug(fss.substring(0, 16), fss.substring(16));
		wifiConfig();
		if (WiFi.status() != WL_CONNECTED)
		{
			waitForWifiToConnect(20);
			debug_outln_info(emptyString);
		}
	}
	debug_outln_info(F("WiFi connected, IP is: "), WiFi.localIP().toString());
	last_signal_strength = WiFi.RSSI();

	if (MDNS.begin(cfg::fs_ssid))
	{
		MDNS.addService("http", "tcp", 80);
		MDNS.addServiceTxt("http", "tcp", "PATH", "/config");
	}
}

#if defined(ESP8266)
BearSSL::X509List x509_dst_root_ca(dst_root_ca_x3);

static void configureCACertTrustAnchor(WiFiClientSecure *client)
{
	constexpr time_t fw_built_year = (__DATE__[7] - '0') * 1000 +
									 (__DATE__[8] - '0') * 100 +
									 (__DATE__[9] - '0') * 10 +
									 (__DATE__[10] - '0');
	if (time(nullptr) < (fw_built_year - 1970) * 365 * 24 * 3600)
	{
		debug_outln_info(F("Time incorrect; Disabling CA verification."));
		client->setInsecure();
	}
	else
	{
		client->setTrustAnchors(&x509_dst_root_ca);
	}
}
#endif

static WiFiClient *getNewLoggerWiFiClient(const LoggerEntry logger)
{

	WiFiClient *_client;
	if (loggerConfigs[logger].session)
	{
		_client = new WiFiClientSecure;
#if defined(ESP8266)
		static_cast<WiFiClientSecure *>(_client)->setSession(loggerConfigs[logger].session);
		static_cast<WiFiClientSecure *>(_client)->setBufferSizes(1024, TCP_MSS > 1024 ? 2048 : 1024);
		switch (logger)
		{
		case Loggeraircms:
		case LoggerInflux:
		case LoggerCustom:
		case LoggerFSapp:
			static_cast<WiFiClientSecure *>(_client)->setInsecure();
			break;
		default:
			configureCACertTrustAnchor(static_cast<WiFiClientSecure *>(_client));
		}
#endif
	}
	else
	{
		_client = new WiFiClient;
	}
	_client->setTimeout(20000);
	return _client;
}

/*****************************************************************
/* GSM auto connecting script                                   *
/*****************************************************************/
void connectGSM()
{

	int retry_count = 0;

	fonaSerial->begin(4800);
	if (!fona.begin(*fonaSerial))
	{
		debug_outln(F("Couldn't find FONA"), DEBUG_MIN_INFO);

		debug_outln(F("Switching to Wifi"), DEBUG_MIN_INFO);
		gsm_capable = 0;
		connectWifi();
	}
	else
	{
		debug_outln(F("FONA is OK"), DEBUG_MIN_INFO);

		unlock_pin();

		char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
		uint8_t imeiLen = fona.getIMEI(imei);
		if (imeiLen > 0)
		{
			debug_outln(F("Module IMEI: "), DEBUG_MIN_INFO);
			debug_out(String(imei), DEBUG_MIN_INFO);
		}

		fona.setGPRSNetworkSettings(F("internet"), F(""), F(""));

		while ((fona.getNetworkStatus() != GSM_CONNECTED) && (retry_count < 40))
		{
			Serial.println("Not registered on network");
			delay(5000);
			retry_count++;

			if (retry_count > 30)
			{
				delay(5000);
				restart_GSM();
			}

			flushSerial();
		}

		if (fona.getNetworkStatus() != GSM_CONNECTED)
		{
			String fss(cfg::fs_ssid);
			display_debug(fss.substring(0, 16), fss.substring(16));

			wifiConfig();
			if (fona.getNetworkStatus() != GSM_CONNECTED)
			{
				retry_count = 0;
				while ((fona.getNetworkStatus() != GSM_CONNECTED) && (retry_count < 20))
				{
					delay(500);
					debug_outln(".", DEBUG_MIN_INFO);
					retry_count++;
				}
				debug_outln("", DEBUG_MIN_INFO);
			}
		}
		else
		{
			enableGPRS();
			Serial.println("GPRS ENABLED");
		}
	}
}

void enableGPRS()
{
	// fona.setGPRSNetworkSettings(FONAFlashStringPtr(gprs_apn), FONAFlashStringPtr(gprs_username), FONAFlashStringPtr(gprs_password));

	int retry_count = 0;
	while ((fona.GPRSstate() != GPRS_CONNECTED) && (retry_count < 40))
	{
		delay(3000);
		fona.enableGPRS(true);
		retry_count++;
	}

	fona.enableGPRS(true);
}

void disableGPRS()
{
	fona.enableGPRS(false);
	delay(3000);
}

void restart_GSM()
{

	flushSerial();

	fonaSerial->begin(4800);
	if (!fona.begin(*fonaSerial))
	{
		debug_outln(F("Couldn't find FONA"), DEBUG_MIN_INFO);
		// while (1);
	}

	unlock_pin();

	enableGPRS();
}

static void unlock_pin()
{
	flushSerial();
	if (strlen(gsm_pin) > 1)
	{
		debug_outln(F("\nAttempting to Unlock SIM please wait: "), DEBUG_MIN_INFO);
		delay(10000);
		if (!fona.unlockSIM(gsm_pin))
		{
			debug_outln(F("Failed to Unlock SIM card with pin: "), DEBUG_MIN_INFO);
			debug_outln(gsm_pin, DEBUG_MIN_INFO);
			delay(10000);
		}
	}
}

/*****************************************************************
 * send data to rest api                                         *
 *****************************************************************/
static unsigned long sendData(const LoggerEntry logger, const String &data, const int pin, const char *host, const char *url)
{
#if defined(ESP8266)
	unsigned long start_send = millis();
	const __FlashStringHelper *contentType;
	int result = 0;
	int port;

	String s_Host(FPSTR(host));
	String s_url(FPSTR(url));

	switch (logger)
	{
	case Loggeraircms:
		contentType = FPSTR(TXT_CONTENT_TYPE_TEXT_PLAIN);
		break;
	case LoggerInflux:
		contentType = FPSTR(TXT_CONTENT_TYPE_INFLUXDB);
		break;
	default:
		contentType = FPSTR(TXT_CONTENT_TYPE_JSON);
		break;
	}

	std::unique_ptr<WiFiClient> client(getNewLoggerWiFiClient(logger));

	String request_head = F("POST ");
	request_head += String(s_url);
	request_head += F(" HTTP/1.1\r\n");
	request_head += F("Host: ");
	request_head += String(s_Host) + "\r\n";
	request_head += F("Content-Type: ");
	request_head += contentType;
	request_head += F("\r\n");
	request_head += F("X-PIN: ");
	request_head += String(pin) + "\r\n";
	request_head += F("X-Sensor: esp8266-");
	request_head += esp_chipid + "\r\n";
	request_head += F("Content-Length: ");
	request_head += String(data.length(), DEC) + "\r\n";
	request_head += F("Connection: close\r\n\r\n");

	if (gsm_capable)
	{
		delay(3000);
		int retry_count = 0;
		uint16_t statuscode;
		int16_t length;

		String gprs_request_head = F("X-PIN: ");
		gprs_request_head += String(pin) + "\\r\\n";
		gprs_request_head += F("X-Sensor: esp8266-");
		gprs_request_head += esp_chipid + "\\r\\n";
		gprs_request_head += F("Authorization: Token ZgZLujgGw4KouWnUc7n24MR_IqXwpfoDi0VP_7VoGQ6yGeWcNR6Xeepuo_BKtcH18AakupI6rejejbNLtecHPg=="); // InfluxDB v2 requires an Authorization header // ToDO update token for production env

		debug_out(F("Start connecting via GPRS \n"), DEBUG_MIN_INFO);
		debug_out(F("HOST "), DEBUG_MIN_INFO);
		debug_out(s_Host, DEBUG_MIN_INFO);
		debug_out(F("\t URL "), DEBUG_MIN_INFO);
		debug_out(s_url, DEBUG_MIN_INFO);
		debug_out(gprs_request_head, DEBUG_MIN_INFO);

		const char *data_copy = data.c_str();
		char gprs_data[strlen(data_copy)];
		strcpy(gprs_data, data_copy);

		String post_url = String(s_Host);
		post_url += String(s_url);
		const char *url_copy = post_url.c_str();
		char gprs_url[strlen(url_copy)];
		strcpy(gprs_url, url_copy);

		Serial.println("\t POST URL  " + String(gprs_url));

		debug_out(F("Sending data via gsm \n"), DEBUG_MIN_INFO);
		debug_out(F("http://"), DEBUG_MIN_INFO);
		debug_out(gprs_url, DEBUG_MIN_INFO);
		debug_out(gprs_data, DEBUG_MIN_INFO);
		debug_out(F("\n"), DEBUG_MIN_INFO);

		if (fona.GPRSstate() != GPRS_CONNECTED)
		{
			debug_out(F("************* Reconnect GPRS *************"), DEBUG_MIN_INFO);
			enableGPRS();
		}

		flushSerial();
		debug_out(F("## Sending via gsm\n\n"), DEBUG_MIN_INFO);

		if (!fona.HTTP_POST_start((char *)gprs_url, F("application/json"), gprs_request_head, (uint8_t *)gprs_data, strlen(gprs_data), &statuscode, (uint16_t *)&length))
		{
			debug_outln_error(F("Failed with status code "));
			debug_out(String(statuscode), DEBUG_ERROR);
			restart_GSM();
			return true;
		}
		while (length > 0)
		{
			while (fona.available())
			{
				char c = fona.read();
// Serial.write is too slow, we'll write directly to Serial register!
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
				loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
				UDR0 = c;
#else
				Serial.write(c);
// debug_out(String(c), DEBUG_MAX_INFO, 0);
#endif
				length--;
				if (!length)
					break;
			}
		}
		debug_out(F("\n\n## End sending via gsm \n\n"), DEBUG_MIN_INFO);
		fona.HTTP_POST_end();
	}
	else if (WiFi.status() == WL_CONNECTED)
	{
		HTTPClient http;
		http.setTimeout(20 * 1000);
		http.setUserAgent(SOFTWARE_VERSION + '/' + esp_chipid);
		http.setReuse(false);
		bool send_success = false;
		if (logger == LoggerCustom && (*cfg::user_custom || *cfg::pwd_custom))
		{
			http.setAuthorization(cfg::user_custom, cfg::pwd_custom);
		}

		if (logger == LoggerInflux && (*cfg::user_influx || *cfg::pwd_influx))
		{
			// http.setAuthorization(cfg::user_influx, cfg::pwd_influx);
			http.addHeader(F("Authorization: "), F("Token ZgZLujgGw4KouWnUc7n24MR_IqXwpfoDi0VP_7VoGQ6yGeWcNR6Xeepuo_BKtcH18AakupI6rejejbNLtecHPg==")); // InfluxDB v2 requires an Authorization header // ToDO update token for production env
		}
		if (http.begin(*client, s_Host, loggerConfigs[logger].destport, s_url, !!loggerConfigs[logger].session))
		{
			http.addHeader(F("Content-Type"), contentType);
			http.addHeader(F("X-Sensor"), String(F(SENSOR_BASENAME)) + esp_chipid);
			if (pin)
			{
				http.addHeader(F("X-PIN"), String(pin));
			}

			result = http.POST(data);

			if (result >= HTTP_CODE_OK && result <= HTTP_CODE_ALREADY_REPORTED)
			{
				debug_outln_info(F("Succeeded - "), s_Host);
				send_success = true;
			}
			else if (result >= HTTP_CODE_BAD_REQUEST)
			{
				debug_outln_info(F("Request failed with error: "), String(result));
				debug_outln_info(F("Details:"), http.getString());
			}
			http.end();
		}
	}
	else
	{
		debug_outln_info(F("Failed connecting to "), s_Host);
	}

	wdt_reset();
	yield();
	return millis() - start_send;
#endif
}

/*****************************************************************
 * send single sensor data to sensors.AFRICA api                  *
 *****************************************************************/
static unsigned long sendCFA(const String &data, const int pin, const __FlashStringHelper *sensorname, const char *replace_str)
{
	unsigned long sum_send_time = 0;

	if (cfg::send2cfa && data.length())
	{
		RESERVE_STRING(data_CFA, LARGE_STR);
		data_CFA = FPSTR(data_first_part);

		debug_outln_info(F("## Sending to sensors.AFRICA - "), sensorname);
		data_CFA += data;
		data_CFA.remove(data_CFA.length() - 1);
		data_CFA.replace(replace_str, emptyString);
		data_CFA += "]}";
		Serial.println(data_CFA);

		sum_send_time = sendData(LoggerCFA, data_CFA, pin, HOST_CFA, URL_CFA);
	}

	return sum_send_time;
}

/*****************************************************************
 * send single sensor data to sensor.community api                *
 *****************************************************************/
static unsigned long sendSensorCommunity(const String &data, const int pin, const __FlashStringHelper *sensorname, const char *replace_str)
{
	unsigned long sum_send_time = 0;

	if (cfg::send2dusti && data.length())
	{
		RESERVE_STRING(data_sensorcommunity, LARGE_STR);
		data_sensorcommunity = FPSTR(data_first_part);

		debug_outln_info(F("## Sending to sensor.community - "), sensorname);
		data_sensorcommunity += data;
		data_sensorcommunity.remove(data_sensorcommunity.length() - 1);
		data_sensorcommunity.replace(replace_str, emptyString);
		data_sensorcommunity += "]}";
		sum_send_time = sendData(LoggerSensorCommunity, data_sensorcommunity, pin, HOST_SENSORCOMMUNITY, URL_SENSORCOMMUNITY);
	}

	return sum_send_time;
}

/*****************************************************************
 * send data to mqtt api                                         *
 *****************************************************************/
// rejected (see issue #33)

/*****************************************************************
 * send data to influxdb                                         *
 *****************************************************************/
static void create_influxdb_string_from_data(String &data_4_influxdb, const String &data)
{
	debug_outln_verbose(F("Parse JSON for influx DB: "), data);
	DynamicJsonDocument json2data(JSON_BUFFER_SIZE);
	DeserializationError err = deserializeJson(json2data, data);
	if (!err)
	{
		data_4_influxdb += cfg::measurement_name_influx;
		data_4_influxdb += F(",node=" SENSOR_BASENAME);
		data_4_influxdb += esp_chipid + " ";
		for (JsonObject measurement : json2data[FPSTR(JSON_SENSOR_DATA_VALUES)].as<JsonArray>())
		{
			data_4_influxdb += measurement["value_type"].as<char *>();
			data_4_influxdb += '=';
			data_4_influxdb += measurement["value"].as<char *>();
			data_4_influxdb += ',';
		}
		if ((unsigned)(data_4_influxdb.lastIndexOf(',') + 1) == data_4_influxdb.length())
		{
			data_4_influxdb.remove(data_4_influxdb.length() - 1);
		}

		data_4_influxdb += '\n';
	}
	else
	{
		debug_outln_error(FPSTR(DBG_TXT_DATA_READ_FAILED));
	}
}

/*****************************************************************
 * send data as csv to serial out                                *
 *****************************************************************/
static void send_csv(const String &data)
{
	DynamicJsonDocument json2data(JSON_BUFFER_SIZE);
	DeserializationError err = deserializeJson(json2data, data);
	debug_outln_info(F("CSV Output: "), data);
	if (!err)
	{
		String headline = F("Timestamp_ms;");
		String valueline(act_milli);
		valueline += ';';
		for (JsonObject measurement : json2data[FPSTR(JSON_SENSOR_DATA_VALUES)].as<JsonArray>())
		{
			headline += measurement["value_type"].as<char *>();
			headline += ';';
			valueline += measurement["value"].as<char *>();
			valueline += ';';
		}
		static bool first_csv_line = true;
		if (first_csv_line)
		{
			if (headline.length() > 0)
			{
				headline.remove(headline.length() - 1);
			}
			Serial.println(headline);
			first_csv_line = false;
		}
		if (valueline.length() > 0)
		{
			valueline.remove(valueline.length() - 1);
		}
		Serial.println(valueline);
	}
	else
	{
		debug_outln_error(FPSTR(DBG_TXT_DATA_READ_FAILED));
	}
}

/*****************************************************************
   read DNMS values
 *****************************************************************/

static float readDNMScorrection()
{
	char *pEnd = nullptr;
	// Avoiding atof() here as this adds a lot (~ 9kb) of code size
	float r = float(strtol(cfg::dnms_correction, &pEnd, 10));
	if (pEnd && pEnd[0] == '.' && pEnd[1] >= '0' && pEnd[1] <= '9')
	{
		r += (r >= 0 ? 1.0 : -1.0) * ((pEnd[1] - '0') / 10.0);
	}
	return r;
}

static void fetchSensorDNMS(String &s)
{
	static bool dnms_error = false;
	debug_outln_verbose(FPSTR(DBG_TXT_START_READING), FPSTR(SENSORS_DNMS));
	last_value_dnms_laeq = -1.0;
	last_value_dnms_la_min = -1.0;
	last_value_dnms_la_max = -1.0;

	if (dnms_calculate_leq() != 0)
	{
		// error
		dnms_error = true;
	}
	uint16_t data_ready = 0;
	dnms_error = true;
	for (unsigned i = 0; i < 20; i++)
	{
		delay(2);
		int16_t ret_dnms = dnms_read_data_ready(&data_ready);
		if ((ret_dnms == 0) && (data_ready != 0))
		{
			dnms_error = false;
			break;
		}
	}
	if (!dnms_error)
	{
		struct dnms_measurements dnms_values;
		if (dnms_read_leq(&dnms_values) == 0)
		{
			float dnms_corr_value = readDNMScorrection();
			last_value_dnms_laeq = dnms_values.leq_a + dnms_corr_value;
			last_value_dnms_la_min = dnms_values.leq_a_min + dnms_corr_value;
			last_value_dnms_la_max = dnms_values.leq_a_max + dnms_corr_value;
		}
		else
		{
			// error
			dnms_error = true;
		}
	}
	if (dnms_error)
	{
		// es gab einen Fehler
		dnms_reset(); // try to reset dnms
		debug_outln_error(F("DNMS read failed"));
	}
	else
	{
		add_Value2Json(s, F("DNMS_noise_LAeq"), F("LAeq: "), last_value_dnms_laeq);
		add_Value2Json(s, F("DNMS_noise_LA_min"), F("LA_MIN: "), last_value_dnms_la_min);
		add_Value2Json(s, F("DNMS_noise_LA_max"), F("LA_MAX: "), last_value_dnms_la_max);
	}
	debug_outln_info(FPSTR(DBG_TXT_SEP));
	debug_outln_verbose(FPSTR(DBG_TXT_END_READING), FPSTR(SENSORS_DNMS));
}

/****************************************************************
 * INITIALIZE SPH0645 MICROPHONE
 * **************************************************************/
void init_SPH0645()
{
	rx_buf_cnt = 0;
	pinMode(I2SI_WS, OUTPUT);
	pinMode(I2SI_BCK, OUTPUT);
	pinMode(I2SI_DATA, INPUT);

	slc_init();
	i2s_init();
}

/****************************************************************
 * RE-INITIALIZE SPH0645 MICROPHONE
 * **************************************************************/
void Reinit_SPH0645()
{
	pinMode(I2SI_WS, OUTPUT);
	pinMode(I2SI_BCK, OUTPUT);
	pinMode(I2SI_DATA, INPUT);

	i2s_init();
}

/****************************************************************
 * OBTAIN SPH0645 MIC VALUE
 * **************************************************************/

void fetchSensorSPH0645(String &s)
{
	if (send_now)
	{
		if (rx_buf_flag)
		{
			for (int i = 0; i < SAMPLE_SIZE; i++)
			{
				for (int x = 0; x < SLC_BUF_LEN; x++)
				{
					if (i2s_slc_buf_pntr[rx_buf_idx][x] > 0)
					{
						float sensor_value = convert(i2s_slc_buf_pntr[rx_buf_idx][x]);
						float dBs = convert_to_dB(sensor_value);
						sample[i] = dBs;
					}
					else
					{
						debug_outln_error(F("No Mic Value available"));
						Reinit_SPH0645(); // Give SPI bus pins back to the MIC
						delay(1000);
					}
				}
				rx_buf_flag = false;
			}
			/* Find the maximum value in the sample array and set as current value */
			float *max_SPL_sample = std::max_element(sample, sample + SAMPLE_SIZE);
			value_SPH0645 = *max_SPL_sample;
		}

		debug_outln_info(F("noise_Leq: "), String(value_SPH0645));
		add_Value2Json(s, F("noise_Leq"), String(value_SPH0645));
	}
}

/*****************************************************************
 * OTAUpdate                                                     *
 *****************************************************************/

static bool fwDownloadStream(WiFiClientSecure &client, const String &url, Stream *ostream)
{

	HTTPClient http;
	int bytes_written = -1;

	http.setTimeout(20 * 1000);
	http.setUserAgent(SOFTWARE_VERSION + ' ' + esp_chipid + ' ' +
					  String(cfg::current_lang) + ' ' + String(CURRENT_LANG) + ' ' +
					  String(cfg::use_beta ? F("BETA") : F("")));
	http.setReuse(false);

	debug_outln_verbose(F("HTTP GET: "), String(FPSTR(FW_DOWNLOAD_HOST)) + ':' + String(FW_DOWNLOAD_PORT) + url);

	if (http.begin(client, FPSTR(FW_DOWNLOAD_HOST), FW_DOWNLOAD_PORT, url))
	{
		int r = http.GET();
		debug_outln_verbose(F("GET r: "), String(r));
		last_update_returncode = r;
		if (r == HTTP_CODE_OK)
		{
			bytes_written = http.writeToStream(ostream);
		}
		http.end();
	}

	if (bytes_written > 0)
		return true;

	return false;
}

static bool fwDownloadStreamFile(WiFiClientSecure &client, const String &url, const String &fname)
{

	String fname_new(fname);
	fname_new += F(".new");
	bool downloadSuccess = false;

	File fwFile = SPIFFS.open(fname_new, "w");
	if (fwFile)
	{
		downloadSuccess = fwDownloadStream(client, url, &fwFile);
		fwFile.close();
		if (downloadSuccess)
		{
			SPIFFS.remove(fname);
			SPIFFS.rename(fname_new, fname);
			debug_outln_info(F("Success downloading: "), url);
		}
	}

	if (downloadSuccess)
		return true;

	SPIFFS.remove(fname_new);
	return false;
}

#if defined(ESP8266)
static bool launchUpdateLoader(const String &md5)
{

	File loaderFile = SPIFFS.open(F("/loader.bin"), "r");
	if (!loaderFile)
	{
		return false;
	}

	if (!Update.begin(loaderFile.size(), U_FLASH))
	{
		return false;
	}

	if (md5.length() && !Update.setMD5(md5.c_str()))
	{
		return false;
	}

	if (Update.writeStream(loaderFile) != loaderFile.size())
	{
		return false;
	}
	loaderFile.close();

	if (!Update.end())
	{
		return false;
	}

	debug_outln_info(F("Erasing SDK config."));
	ESP.eraseConfig();

	sensor_restart();
	return true;
}
#endif

static void twoStageOTAUpdate()
{

	if (!cfg::auto_update)
		return;

#if defined(ESP8266)
	debug_outln_info(F("twoStageOTAUpdate"));

	String lang_variant(cfg::current_lang);
	if (lang_variant.length() != 2)
	{
		lang_variant = CURRENT_LANG;
	}
	lang_variant.toLowerCase();

	String fetch_name(F(OTA_BASENAME "/update/latest_"));
	if (cfg::use_beta)
	{
		fetch_name = F(OTA_BASENAME "/beta/latest_");
	}
	fetch_name += lang_variant;
	fetch_name += F(".bin");

	WiFiClientSecure client;
	BearSSL::Session clientSession;

	client.setBufferSizes(1024, TCP_MSS > 1024 ? 2048 : 1024);
	client.setSession(&clientSession);
	configureCACertTrustAnchor(&client);

	String fetch_md5_name(fetch_name);
	fetch_md5_name += F(".md5");

	StreamString newFwmd5;
	if (!fwDownloadStream(client, fetch_md5_name, &newFwmd5))
		return;

	newFwmd5.trim();
	if (newFwmd5 == ESP.getSketchMD5())
	{
		display_debug(FPSTR(DBG_TXT_UPDATE), FPSTR(DBG_TXT_UPDATE_NO_UPDATE));
		debug_outln_verbose(F("No newer version available."));
		return;
	}

	debug_outln_info(F("Update md5: "), newFwmd5);
	debug_outln_info(F("Sketch md5: "), ESP.getSketchMD5());

	// We're entering update phase, kill off everything else
	WiFiUDP::stopAll();
	WiFiClient::stopAllExcept(&client);
	delay(100);

	String firmware_name(F("/firmware.bin"));
	String firmware_md5(F("/firmware.bin.md5"));
	String loader_name(F("/loader.bin"));
	if (!fwDownloadStreamFile(client, fetch_name, firmware_name))
		return;
	if (!fwDownloadStreamFile(client, fetch_md5_name, firmware_md5))
		return;
	if (!fwDownloadStreamFile(client, FPSTR(FW_2ND_LOADER_URL), loader_name))
		return;

	File fwFile = SPIFFS.open(firmware_name, "r");
	if (!fwFile)
	{
		SPIFFS.remove(firmware_name);
		SPIFFS.remove(firmware_md5);
		debug_outln_error(F("Failed reopening fw file.."));
		return;
	}
	size_t fwSize = fwFile.size();
	MD5Builder md5;
	md5.begin();
	md5.addStream(fwFile, fwSize);
	md5.calculate();
	fwFile.close();
	String md5String = md5.toString();

	// Firmware is always at least 128 kB and padded to 16 bytes
	if (fwSize < (1 << 17) || (fwSize % 16 != 0) || newFwmd5 != md5String)
	{
		debug_outln_info(F("FW download failed validation.. deleting"));
		SPIFFS.remove(firmware_name);
		SPIFFS.remove(firmware_md5);
		return;
	}

	StreamString loaderMD5;
	if (!fwDownloadStream(client, String(FPSTR(FW_2ND_LOADER_URL)) + F(".md5"), &loaderMD5))
		return;

	loaderMD5.trim();

	debug_outln_info(F("launching 2nd stage"));
	if (!launchUpdateLoader(loaderMD5))
	{
		debug_outln_error(FPSTR(DBG_TXT_UPDATE_FAILED));
		display_debug(FPSTR(DBG_TXT_UPDATE), FPSTR(DBG_TXT_UPDATE_FAILED));
		SPIFFS.remove(firmware_name);
		SPIFFS.remove(firmware_md5);
		return;
	}
#endif
}

static String displayGenerateFooter(unsigned int screen_count)
{
	String display_footer;
	for (unsigned int i = 0; i < screen_count; ++i)
	{
		display_footer += (i != (next_display_count % screen_count)) ? " . " : " o ";
	}
	return display_footer;
}

/**************************************
	add display functionality
**************************************/
#include "displays.h"

/*****************************************************************
   Init DNMS - Digital Noise Measurement Sensor
 *****************************************************************/
static void initDNMS()
{
	char dnms_version[DNMS_MAX_VERSION_LEN + 1];

	debug_out(F("Trying DNMS sensor on 0x55H "), DEBUG_MIN_INFO);
	dnms_reset();
	delay(1000);
	if (dnms_read_version(dnms_version) != 0)
	{
		debug_outln_info(FPSTR(DBG_TXT_NOT_FOUND));
		debug_outln_error(F("Check DNMS wiring"));
		dnms_init_failed = true;
	}
	else
	{
		dnms_version[DNMS_MAX_VERSION_LEN] = 0;
		debug_outln_info(FPSTR(DBG_TXT_FOUND), String(": ") + String(dnms_version));
	}
}

static void powerOnTestSensors()
{
	if (cfg::wifi_enabled)
	{
		debug_outln_info(F("WIFI ENABLED..."));
	}
	else
	{
		debug_outln_info(F("WIFI DISABLED..."));
	}

	if (cfg::dnms_read)
	{
		debug_outln_info(F("Read DNMS..."));
		initDNMS();
	}

	if (cfg::sph0645_read)
	{
		debug_outln_info(F("Read SPH0645..."));
		init_SPH0645();
	}
}

static void logEnabledAPIs()
{
	debug_outln_info(F("Send to :"));
	if (cfg::send2dusti)
	{
		debug_outln_info(F("sensor.community"));
	}
	if (cfg::send2cfa)
	{
		debug_outln_info(F("CFA"));
	}

	if (cfg::send2fsapp)
	{
		debug_outln_info(F("Feinstaub-App"));
	}

	if (cfg::send2madavi)
	{
		debug_outln_info(F("Madavi.de"));
	}

	if (cfg::send2csv)
	{
		debug_outln_info(F("Serial as CSV"));
	}

	if (cfg::send2custom)
	{
		debug_outln_info(F("custom API"));
	}

	if (cfg::send2aircms)
	{
		debug_outln_info(F("aircms API"));
	}

	if (cfg::send2influx)
	{
		debug_outln_info(F("custom influx DB"));
	}
	debug_outln_info(FPSTR(DBG_TXT_SEP));
	if (cfg::auto_update)
	{
		debug_outln_info(F("Auto-Update active..."));
	}
}

static void logEnabledDisplays()
{
	if (cfg::has_display || cfg::has_sh1106)
	{
		debug_outln_info(F("Show on OLED..."));
	}
	if (lcd_1602)
	{
		debug_outln_info(F("Show on LCD 1602 ..."));
	}
	if (lcd_2004)
	{
		debug_outln_info(F("Show on LCD 2004 ..."));
	}
}

/**************************************************
	Set network time from ntp.org server time
**************************************************/

#include "set-network-time.h"

static unsigned long sendDataToOptionalApis(const String &data)
{
	unsigned long sum_send_time = 0;

	if (cfg::send2madavi)
	{
		debug_outln_info(FPSTR(DBG_TXT_SENDING_TO), F("madavi.de: "));
		sum_send_time += sendData(LoggerMadavi, data, 0, HOST_MADAVI, URL_MADAVI);
	}

	if (cfg::send2sensemap && (cfg::senseboxid[0] != '\0'))
	{
		debug_outln_info(FPSTR(DBG_TXT_SENDING_TO), F("opensensemap: "));
		String sensemap_path(tmpl(FPSTR(URL_SENSEMAP), cfg::senseboxid));
		sum_send_time += sendData(LoggerSensemap, data, 0, HOST_SENSEMAP, sensemap_path.c_str());
	}

	if (cfg::send2fsapp)
	{
		debug_outln_info(FPSTR(DBG_TXT_SENDING_TO), F("Server FS App: "));
		sum_send_time += sendData(LoggerFSapp, data, 0, HOST_FSAPP, URL_FSAPP);
	}

	if (cfg::send2aircms)
	{
		debug_outln_info(FPSTR(DBG_TXT_SENDING_TO), F("aircms.online: "));
		unsigned long ts = millis() / 1000;
		String token = WiFi.macAddress();
		String aircms_data("L=");
		aircms_data += esp_chipid;
		aircms_data += "&t=";
		aircms_data += String(ts, DEC);
		aircms_data += F("&airrohr=");
		aircms_data += data;
		String aircms_url(FPSTR(URL_AIRCMS));
		aircms_url += hmac1(sha1Hex(token), aircms_data + token);

		sum_send_time += sendData(Loggeraircms, aircms_data, 0, HOST_AIRCMS, aircms_url.c_str());
	}

	if (cfg::send2influx)
	{
		Serial.println("Testing sending to local influx DB");
		debug_outln_info(FPSTR(DBG_TXT_SENDING_TO), F("custom influx db: "));
		RESERVE_STRING(data_4_influxdb, LARGE_STR);
		create_influxdb_string_from_data(data_4_influxdb, data);
		sum_send_time += sendData(LoggerInflux, data_4_influxdb, 0, cfg::host_influx, cfg::url_influx);
	}

	if (cfg::send2custom)
	{
		String data_to_send = data;
		data_to_send.remove(0, 1);
		String data_4_custom(F("{\"esp8266id\": \""));
		data_4_custom += esp_chipid;
		data_4_custom += "\", ";
		data_4_custom += data_to_send;
		debug_outln_info(FPSTR(DBG_TXT_SENDING_TO), F("custom api: "));
		sum_send_time += sendData(LoggerCustom, data_4_custom, 0, cfg::host_custom, cfg::url_custom);
	}

	if (cfg::send2csv)
	{
		debug_outln_info(F("## Sending as csv: "));
		send_csv(data);
	}

	return sum_send_time;
}

/*****************************************************************
 * The Setup                                                     *
 *****************************************************************/

void setup(void)
{
	Serial.begin(9600); // Output to Serial at 9600 baud

#if defined(WIFI_LoRa_32_V2)
	// reset the OLED display, e.g. of the heltec_wifi_lora_32 board
	pinMode(RST_OLED, OUTPUT);
	digitalWrite(RST_OLED, LOW);
	delay(50);
	digitalWrite(RST_OLED, HIGH);
#endif
	Wire.begin(I2C_PIN_SDA, I2C_PIN_SCL);

#if defined(ESP8266)
	esp_chipid = std::move(String(ESP.getChipId()));
#endif
#if defined(ESP32)
	uint64_t chipid_num;
	chipid_num = ESP.getEfuseMac();
	esp_chipid = String((uint16_t)(chipid_num >> 32), HEX);
	esp_chipid += String((uint32_t)chipid_num, HEX);
#endif
	cfg::initNonTrivials(esp_chipid.c_str());
	WiFi.persistent(false);

	debug_outln_info(F("airRohr: " SOFTWARE_VERSION_STR "/"), String(CURRENT_LANG));
	if ((airrohr_selftest_failed = !ESP.checkFlashConfig(true) /* after 2.7.0 update: || !ESP.checkFlashCRC() */))
	{
		debug_outln_error(F("ERROR: SELF TEST FAILED!"));
		SOFTWARE_VERSION += F("-STF");
	}

	init_config();
	init_display();
	init_lcd();
	setupNetworkTime();
	connectWifi();
	setup_webserver();
	debug_outln_info(F("\nChipId: "), esp_chipid);

	if (gsm_capable)
	{
		connectGSM();
	}
	else
	{
		connectWifi();
	}

	powerOnTestSensors();
	logEnabledAPIs();
	logEnabledDisplays();

	delay(50);

	// sometimes parallel sending data and web page will stop nodemcu, watchdogtimer set to 120 seconds
#if defined(ESP8266)
	wdt_disable();
#if defined(NDEBUG)
	wdt_enable(120000);
#endif
#endif
	starttime = millis(); // store the start time
	last_update_attempt = time_point_device_start_ms = starttime;
	last_display_millis = starttime;
}

/*****************************************************************
 * And action                                                    *
 *****************************************************************/
void loop(void)
{

	String result_DNMS, result_SPH0645;

	unsigned sum_send_time = 0;

	act_micro = micros();
	act_milli = millis();
	send_now = msSince(starttime) > cfg::sending_intervall_ms;
	// Wait at least 30s for each NTP server to sync

	if (!sntp_time_set && send_now &&
		msSince(time_point_device_start_ms) < 1000 * 2 * 30 + 5000)
	{
		debug_outln_info(F("NTP sync not finished yet, skipping send"));
		send_now = false;
		starttime = act_milli;
	}

	sample_count++;

#if defined(ESP8266)
	wdt_reset(); // nodemcu is alive
#endif

	if (last_micro != 0)
	{
		unsigned long diff_micro = act_micro - last_micro;
		if (max_micro < diff_micro)
		{
			max_micro = diff_micro;
		}
		if (min_micro > diff_micro)
		{
			min_micro = diff_micro;
		}
	}
	last_micro = act_micro;

	if (msSince(time_point_device_start_ms) > DURATION_BEFORE_FORCED_RESTART_MS)
	{
		sensor_restart();
	}

	if (msSince(last_update_attempt) > PAUSE_BETWEEN_UPDATE_ATTEMPTS_MS)
	{
		twoStageOTAUpdate();
		last_update_attempt = act_milli;
	}

	if (cfg::sph0645_read)
	{
		fetchSensorSPH0645(result_SPH0645);
	}

	if ((msSince(last_display_millis) > DISPLAY_UPDATE_INTERVAL_MS) &&
		(cfg::has_display || cfg::has_sh1106 || lcd_1602 || lcd_2004))
	{
		display_values();
		last_display_millis = act_milli;
	}

	server.handleClient();
	if (!cfg::wifi_enabled)
	{
#if defined(ESP8266)
		wdt_reset(); // nodemcu is alive
#endif
	}
	yield();

	if (send_now)
	{
		last_signal_strength = WiFi.RSSI();
		RESERVE_STRING(data, LARGE_STR);
		data = FPSTR(data_first_part);
		RESERVE_STRING(result, MED_STR);

		if (cfg::sph0645_read)
		{
			data += result_SPH0645;
			if (cfg::wifi_enabled || cfg::gsm_capable)
			{
				sum_send_time += sendCFA(result_SPH0645, SPH0645_API_PIN, FPSTR(SENSORS_SPH0645), "SPH0645_");
				sum_send_time += sendSensorCommunity(result_SPH0645, SPH0645_API_PIN, FPSTR(SENSORS_SPH0645), "SHP0645_");
			}
		}
		if (cfg::dnms_read && (!dnms_init_failed))
		{
			// getting noise measurement values from dnms (optional)
			fetchSensorDNMS(result);
			data += result;
			if (cfg::wifi_enabled || cfg::gsm_capable)
			{
				sum_send_time += sendCFA(result, DNMS_API_PIN, FPSTR(SENSORS_DNMS), "DNMS_");
				sum_send_time += sendSensorCommunity(result, DNMS_API_PIN, FPSTR(SENSORS_DNMS), "DNMS_");
			}
			result = emptyString;
		}
		add_Value2Json(data, F("samples"), String(sample_count));
		add_Value2Json(data, F("min_micro"), String(min_micro));
		add_Value2Json(data, F("max_micro"), String(max_micro));
		add_Value2Json(data, F("signal"), String(last_signal_strength));

		if ((unsigned)(data.lastIndexOf(',') + 1) == data.length())
		{
			data.remove(data.length() - 1);
		}
		data += "]}";

		yield();

		if (cfg::gsm_capable)
		{
			sum_send_time += sendDataToOptionalApis(data);
		}

		// https://en.wikipedia.org/wiki/Moving_average#Cumulative_moving_average
		sending_time = (3 * sending_time + sum_send_time) / 4;
		if (sum_send_time > 0)
		{
			debug_outln_info(F("Time for Sending (ms): "), String(sending_time));
		}

		// reconnect to WiFi if disconnected
		if ((WiFi.status() != WL_CONNECTED && cfg::wifi_enabled))
		{
			debug_outln_info(F("Connection lost, reconnecting "));
			WiFi_error_count++;
			WiFi.reconnect();
			waitForWifiToConnect(20);
		}

		// Resetting for next sampling
		last_data_string = std::move(data);
		sample_count = 0;
		last_micro = 0;
		min_micro = 1000000000;
		max_micro = 0;
		sum_send_time = 0;
		starttime = millis(); // store the start time
		count_sends++;
	}
	yield();
#if defined(ESP8266)
	MDNS.update();
#endif

	if (sample_count % 500 == 0)
	{
		//		Serial.println(ESP.getFreeHeap(),DEC);
	}
}
