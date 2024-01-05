/*
 *	airRohr firmware
 *	Copyright (C) 2016-2018  Code for Stuttgart a.o.
 *
 *  English translations
 *
 *	Texts should be as short as possible
 *	We only have 512 kB for the entire firmware
 */

const char INTL_LANG[] = "EN";
const char INTL_CONFIGURATION[] PROGMEM = "Configuration";
#define INTL_WIFI_SETTINGS "WiFi Settings"
#define INTL_WIFI_NETWORKS "Loading wifi networks ..."
#define INTL_LANGUAGE "Language"
const char INTL_NO_NETWORKS[] PROGMEM =  "No WiFi Network Found";
const char INTL_NETWORKS_FOUND[] PROGMEM = "Found Networks: ";
const char INTL_AB_HIER_NUR_ANDERN[] PROGMEM = "Advanced settings (оnly if you know what you are doing)";
const char INTL_SAVE[] PROGMEM = "Save";
const char INTL_SENSORS[] PROGMEM = "Sensors";
const char INTL_MORE_SENSORS[] PROGMEM = "More Sensors";
const char INTL_GSM_CAPABLE[] PROGMEM = "GSM Attached";
const char INTL_GSM_PIN[] PROGMEM = "GSM PIN";
const char INTL_GPRS_APN[] PROGMEM = "GPRS APN";
const char INTL_GPRS_USERNAME[] PROGMEM = "GPRS USERNAME";
const char INTL_GPRS_PASSWORD[] PROGMEM = "GPRS PASSWORD";
const char INTL_SPH0645[] PROGMEM = "SPH0645 (MIC)";
const char INTL_DNMS[] PROGMEM = "DNMS ({l_a})";
const char INTL_DNMS_CORRECTION[] PROGMEM ="correction in dB(A)";
const char INTL_BASICAUTH[] PROGMEM = "Authentication";

const char INTL_ENABLE_WIFI[] PROGMEM = "WIFI ENABLED";
const char INTL_FS_WIFI_DESCRIPTION[] PROGMEM = "WiFi Sensor in configuration mode";
const char INTL_FS_WIFI_NAME[] PROGMEM = "Network name";
const char INTL_MORE_SETTINGS[] PROGMEM ="More settings";
const char INTL_AUTO_UPDATE[] PROGMEM = "Auto update firmware";
const char INTL_USE_BETA[] PROGMEM = "Load beta firmware";
const char INTL_DISPLAY[] PROGMEM = "OLED SSD1306";
const char INTL_SH1106[] PROGMEM = "OLED SH1106";
const char INTL_FLIP_DISPLAY[] PROGMEM = "OLED display flip";
const char INTL_LCD1602_27[] PROGMEM = "LCD 1602 (I2C: 0x27)";
const char INTL_LCD1602_3F[] PROGMEM = "LCD 1602 (I2C: 0x3F)";
const char INTL_LCD2004_27[] PROGMEM = "LCD 2004 (I2C: 0x27)";
const char INTL_LCD2004_3F[] PROGMEM = "LCD 2004 (I2C: 0x3F)";
const char INTL_DISPLAY_WIFI_INFO[] PROGMEM = "Display Wifi info";
const char INTL_DISPLAY_DEVICE_INFO[] PROGMEM = "Display device info";
const char INTL_DEBUG_LEVEL[] PROGMEM = "Debug&nbsp;level";
const char INTL_MEASUREMENT_INTERVAL[] PROGMEM = "Measuring interval (sec)";
const char INTL_DURATION_ROUTER_MODE[] PROGMEM = "Duration router mode";
const char INTL_MORE_APIS[] PROGMEM = "More APIs";
const char INTL_SEND_TO_OWN_API[] PROGMEM = "Send data to custom API";
const char INTL_SERVER[] PROGMEM = "Server";
const char INTL_PATH[] PROGMEM = "Path";
const char INTL_PORT[] PROGMEM = "Port";
const char INTL_USER[] PROGMEM = "User";
const char INTL_PASSWORD[] PROGMEM = "Password";
const char INTL_SEND_TO[] PROGMEM = "Send to {v}";
const char INTL_READ_FROM[] PROGMEM = "Read from {v}";
const char INTL_SENSOR_IS_REBOOTING[] PROGMEM = "Sensor is rebooting.";
const char INTL_RESTART_DEVICE[] PROGMEM = "Restart device";
const char INTL_DELETE_CONFIG[] PROGMEM = "delete saved configuration";
const char INTL_RESTART_SENSOR[] PROGMEM = "Restart sensor";
#define INTL_HOME "Home"
#define INTL_BACK_TO_HOME "Back to home page"
const char INTL_CURRENT_DATA[] PROGMEM = "Current data";
const char INTL_DEVICE_STATUS[] PROGMEM = "Device status";
#define INTL_ACTIVE_SENSORS_MAP "Active sensors map (external link)"
#define INTL_CONFIGURATION_DELETE "Delete configuration"
#define INTL_CONFIGURATION_REALLY_DELETE "Are you sure you want to delete the configuration?"
#define INTL_DELETE "Delete"
#define INTL_CANCEL "Cancel"
#define INTL_REALLY_RESTART_SENSOR "Are you sure you want to restart the sensor?"
#define INTL_RESTART "Restart"
const char INTL_SAVE_AND_RESTART[] PROGMEM = "Save configuration and restart";
#define INTL_FIRMWARE "Firmware version"
const char INTL_DEBUG_SETTING_TO[] PROGMEM = "Set debug level to";
#define INTL_NONE "off"
#define INTL_ERROR "only errors"
#define INTL_WARNING "warnings"
#define INTL_MIN_INFO "min. info"
#define INTL_MED_INFO "mid. info"
#define INTL_MAX_INFO "max. info"
#define INTL_CONFIG_DELETED "Configuration was deleted"
#define INTL_CONFIG_CAN_NOT_BE_DELETED "Configuration can not be deleted"
#define INTL_CONFIG_NOT_FOUND "Configuration not found"
const char INTL_TIME_TO_FIRST_MEASUREMENT[] PROGMEM = "Still {v} seconds until first measurement.";
const char INTL_TIME_SINCE_LAST_MEASUREMENT[] PROGMEM = " seconds since last measurement.";
const char INTL_PARTICLES_PER_LITER[] PROGMEM = "particles/liter";
const char INTL_PARTICULATE_MATTER[] PROGMEM = "particulate matter";
const char INTL_TEMPERATURE[] PROGMEM = "temperature";
const char INTL_HUMIDITY[] PROGMEM = "humidity";
const char INTL_PRESSURE[] PROGMEM = "air pressure";
const char INTL_LEQ_A[] PROGMEM = "LAeq";
const char INTL_LA_MIN[] PROGMEM = "LA min";
const char INTL_LA_MAX[] PROGMEM = "LA max";
const char INTL_SIGNAL_STRENGTH[] PROGMEM = "signal strength";
const char INTL_SIGNAL_QUALITY[] PROGMEM = "signal quality";
#define INTL_NUMBER_OF_MEASUREMENTS "Number of measurements"
#define INTL_TIME_SENDING_MS "Time spent uploading"
#define INTL_SENSOR "Sensor"
#define INTL_PARAMETER "Parameter"
#define INTL_VALUE "Value"

#include "./airrohr-logo-common.h"
