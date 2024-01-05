#include "./oledfont.h" // avoids including the default Arial font, needs to be included before SSD1306.h
#include <SSD1306.h>
#include <SH1106.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <StreamString.h>
#include <DallasTemperature.h>
#include "./dnms_i2c.h"
#include "./SPH0645.h"
#include <Adafruit_FONA.h>