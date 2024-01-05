/*
 *	airRohr firmware
 *	Copyright (C) 2016-2018  Code for Stuttgart a.o.
 *
 *  Luxembourgish translations
 *
 *	Texts should be as short as possible
 *	We only have 512 kB for the entire firmware
 */

const char INTL_LANG[] = "LU";
const char INTL_CONFIGURATION[] PROGMEM = "Konfiguratioun";
#define INTL_WIFI_SETTINGS "WiFi Daten"
#define INTL_WIFI_NETWORKS "WiFi Netzwierker lueden ..."
#define INTL_LANGUAGE "Sprooch"
const char INTL_NO_NETWORKS[] PROGMEM =  "Keng Netzwierker fonnt";
const char INTL_NETWORKS_FOUND[] PROGMEM = "Netzwierker fonnt: ";
const char INTL_AB_HIER_NUR_ANDERN[] PROGMEM = "Ab hei nëmmen änneren, wann der wësst wat dir maacht";
const char INTL_SAVE[] PROGMEM = "Späicheren";
const char INTL_SENSORS[] PROGMEM = "Sensoren";
const char INTL_GSM_CAPABLE[] PROGMEM = "GSM Befestegt";
const char INTL_GSM_PIN[] PROGMEM = "GSM PIN";
const char INTL_GPRS_APN[] PROGMEM = "GPRS APN";
const char INTL_GPRS_USERNAME[] PROGMEM = "GPRS Benotzernumm";
const char INTL_GPRS_PASSWORD[] PROGMEM = "GPRS PASSWORD";
const char INTL_MORE_SENSORS[] PROGMEM = "More Sensoren";
const char INTL_SPH0645[] PROGMEM = "SPH0645 (MIC)";
const char INTL_DNMS[] PROGMEM = "DNMS ({l_a})";
const char INTL_DNMS_CORRECTION[] PROGMEM ="Korrekturwärt&nbsp;an&nbsp;dB(A)";
const char INTL_BASICAUTH[] PROGMEM = "BasicAuth aktivéieren";

const char INTL_ENABLE_WIFI[] PROGMEM = "WIFI ENABLED";
const char INTL_FS_WIFI_DESCRIPTION[] PROGMEM = "WiFi Sensor Numm am Konfiguratiounsmodus";
const char INTL_FS_WIFI_NAME[] PROGMEM = "Numm";
const char INTL_MORE_SETTINGS[] PROGMEM ="Méi Astellungen";
const char INTL_AUTO_UPDATE[] PROGMEM = "Automateschen Update";
const char INTL_USE_BETA[] PROGMEM = "Beta Versiounen eroflueden";
const char INTL_DISPLAY[] PROGMEM = "OLED SSD1306";
const char INTL_SH1106[] PROGMEM = "OLED SH1106";
const char INTL_FLIP_DISPLAY[] PROGMEM = "OLED Display em 180° dréien";
const char INTL_LCD1602_27[] PROGMEM = "LCD 1602 (I2C: 0x27)";
const char INTL_LCD1602_3F[] PROGMEM = "LCD 1602 (I2C: 0x3F)";
const char INTL_LCD2004_27[] PROGMEM = "LCD 2004 (I2C: 0x27)";
const char INTL_LCD2004_3F[] PROGMEM = "LCD 2004 (I2C: 0x3F)";
const char INTL_DISPLAY_WIFI_INFO[] PROGMEM = "WiFi Info uweisen";
const char INTL_DISPLAY_DEVICE_INFO[] PROGMEM = "Apparat Info uweisen";
const char INTL_DEBUG_LEVEL[] PROGMEM = "Debug&nbsp;Niveau";
const char INTL_MEASUREMENT_INTERVAL[] PROGMEM = "Mooss-intervall";
const char INTL_DURATION_ROUTER_MODE[] PROGMEM = "Zait&nbsp;Routermodus";
const char INTL_MORE_APIS[] PROGMEM = "Aner APIs";
const char INTL_SEND_TO_OWN_API[] PROGMEM = "Un eegen API schécken";
const char INTL_SERVER[] PROGMEM = "Server";
const char INTL_PATH[] PROGMEM = "Wee";
const char INTL_PORT[] PROGMEM = "Port";
const char INTL_USER[] PROGMEM = "Benotzer";
const char INTL_PASSWORD[] PROGMEM = "Passwuert";
const char INTL_SEND_TO[] PROGMEM = "Schécken op {v}";
const char INTL_READ_FROM[] PROGMEM = "Liesen {v}";
const char INTL_SENSOR_IS_REBOOTING[] PROGMEM = "De Sensor gëtt neigestart.";
const char INTL_RESTART_DEVICE[] PROGMEM = "Apparat nei starten";
const char INTL_DELETE_CONFIG[] PROGMEM = "Config.json läschen";
const char INTL_RESTART_SENSOR[] PROGMEM = "Sensor nei starten";
#define INTL_HOME "Iwwersiicht"
#define INTL_BACK_TO_HOME "Zréck op d'Haaptsäit"
const char INTL_CURRENT_DATA[] PROGMEM = "Aktuell Wäerter";
const char INTL_DEVICE_STATUS[] PROGMEM = "Apparat Status";
#define INTL_ACTIVE_SENSORS_MAP "Kaart vun den aktive Sensoren (externen Link)"
#define INTL_CONFIGURATION_DELETE "Konfiguratioun läschen"
#define INTL_CONFIGURATION_REALLY_DELETE "Konfiguratioun wierklech läschen?"
#define INTL_DELETE "Läschen"
#define INTL_CANCEL "Ofbriechen"
#define INTL_REALLY_RESTART_SENSOR "Sensor wierklech nei starten?"
#define INTL_RESTART "Nei starten"
const char INTL_SAVE_AND_RESTART[] PROGMEM = "Späicheren a nei starten";
#define INTL_FIRMWARE "Firmware"
const char INTL_DEBUG_SETTING_TO[] PROGMEM = "Debug setzen op";
#define INTL_NONE "keng"
#define INTL_ERROR "fehler"
#define INTL_WARNING "warnung"
#define INTL_MIN_INFO "min. info"
#define INTL_MED_INFO "med. info"
#define INTL_MAX_INFO "max. info"
#define INTL_CONFIG_DELETED "Config.json geläscht"
#define INTL_CONFIG_CAN_NOT_BE_DELETED "Config.json konnt net geläscht ginn"
#define INTL_CONFIG_NOT_FOUND "Config.json net fonnt"
const char INTL_TIME_TO_FIRST_MEASUREMENT[] PROGMEM = "Nach {v} Sekonnen bis zur éischter Miessung.";
const char INTL_TIME_SINCE_LAST_MEASUREMENT[] PROGMEM = " Sekonnen zanter der leschter Miessung.";
const char INTL_PARTICLES_PER_LITER[] PROGMEM = "Partikel/Liter";
const char INTL_PARTICULATE_MATTER[] PROGMEM = "Reng Stëbspartikelen";
const char INTL_TEMPERATURE[] PROGMEM = "Temperatur";
const char INTL_HUMIDITY[] PROGMEM = "rel. Loftfiichtegkeet";
const char INTL_PRESSURE[] PROGMEM = "Loftdrock";
const char INTL_LEQ_A[] PROGMEM = "LAeq";
const char INTL_LA_MIN[] PROGMEM = "LA min";
const char INTL_LA_MAX[] PROGMEM = "LA max";
const char INTL_SIGNAL_STRENGTH[] PROGMEM = "Signal";
const char INTL_SIGNAL_QUALITY[] PROGMEM = "Qualitéit";
#define INTL_NUMBER_OF_MEASUREMENTS "Zuel vu Mesuren"
#define INTL_TIME_SENDING_MS "Dauer vu Mesureniwerdroung"
#define INTL_SENSOR "Sensor"
#define INTL_PARAMETER "Parameter"
#define INTL_VALUE "Wäert"

#include "./airrohr-logo-common.h"
