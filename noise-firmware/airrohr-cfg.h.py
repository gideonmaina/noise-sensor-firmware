#!/usr/bin/env python3

configshape_in = """
String		current_lang
String		wlanssid
Password		wlanpwd
String		www_username
Password		www_password
String		fs_ssid
Password		fs_pwd
Bool		www_basicauth_enabled
Bool        wifi_enabled
Bool        sph0645_read
Bool		dnms_read
String		dnms_correction
Bool        gsm_capable
Bool		send2cfa
Bool		ssl_cfa
Bool		send2dusti
Bool		ssl_dusti
Bool		send2madavi
Bool		ssl_madavi
Bool		send2sensemap
Bool		send2fsapp
Bool		send2aircms
Bool		send2csv
Bool		auto_update
Bool		use_beta
Bool		has_display
Bool		has_sh1106
Bool		has_flipped_display
Bool		has_lcd1602
Bool		has_lcd1602_27
Bool		has_lcd2004
Bool		has_lcd2004_27
Bool		display_wifi_info
Bool		display_device_info
UInt		debug
Time		sending_intervall_ms
Time		time_for_wifi_config
String		senseboxid
Bool		send2custom
String		host_custom
String		url_custom
UInt		port_custom
String		user_custom
Password		pwd_custom
Bool		ssl_custom
Bool		send2influx
String		host_influx
String		url_influx
UInt		port_influx
String		user_influx
Password		pwd_influx
String		measurement_name_influx
Bool		ssl_influx
UInt        gsm_pin
String      gprs_apn
String      gprs_username
Password        gprs_password
"""

with open("airrohr-cfg.h", "w") as h:
    print("""

// This file is generated, please do not edit.
// Change airrohr-cfg.h.py instead.

enum ConfigEntryType : unsigned short {
	Config_Type_Bool,
	Config_Type_UInt,
	Config_Type_Time,
	Config_Type_String,
	Config_Type_Password
};

struct ConfigShapeEntry {
	enum ConfigEntryType cfg_type;
	unsigned short cfg_len;
	const __FlashStringHelper* cfg_key;
	union {
		void* as_void;
		bool* as_bool;
		unsigned int* as_uint;
		char* as_str;
	} cfg_val;
};

enum ConfigShapeId {""", file=h)

    for cfgentry in configshape_in.strip().split('\n'):
        print("\tConfig_", cfgentry.split()[1], ",", sep='', file=h)
    print("};", file=h)

    for cfgentry in configshape_in.strip().split('\n'):
        _, cfgkey = cfgentry.split()
        print("const char CFG_KEY_", cfgkey.upper(),
              "[] PROGMEM = \"", cfgkey, "\";", sep='', file=h)

    print("static constexpr ConfigShapeEntry configShape[] PROGMEM = {",
          file=h)
    for cfgentry in configshape_in.strip().split('\n'):
        cfgtype, cfgkey = cfgentry.split()
        print("\t{ Config_Type_", cfgtype,
              ", sizeof(cfg::" + cfgkey + ")-1" if cfgtype in ('String', 'Password') else ", 0",
              ", FPSTR(CFG_KEY_", cfgkey.upper(),
              "), ", "" if cfgtype in ('String', 'Password') else "&",
              "cfg::", cfgkey, " },", sep='', file=h)
    print("};", file=h)
