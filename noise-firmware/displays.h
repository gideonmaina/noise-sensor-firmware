/*****************************************************************
 * display values                                                *
 *****************************************************************/
static void display_values()
{
    float t_value = -128.0;
    float h_value = -1.0;
    float p_value = -1.0;
    String t_sensor, h_sensor, p_sensor;
    float nc005_value = -1.0;
    float nc010_value = -1.0;
    float nc025_value = -1.0;
    float nc040_value = -1.0;
    float nc100_value = -1.0;
    float la_eq_value = -1.0;
    float la_max_value = -1.0;
    float la_min_value = -1.0;
    String la_sensor;
    double lat_value = -200.0;
    double lon_value = -200.0;
    double alt_value = -1000.0;
    String display_header;
    String display_lines[3] = {"", "", ""};
    uint8_t screen_count = 0;
    uint8_t screens[8];
    int line_count = 0;
    debug_outln_info(F("output values to display..."));
    if (cfg::dnms_read)
    {
        la_sensor = FPSTR(SENSORS_DNMS);
        la_eq_value = last_value_dnms_laeq;
        la_max_value = last_value_dnms_la_max;
        la_min_value = last_value_dnms_la_min;
    }
    if (cfg::dnms_read)
    {
        screens[screen_count++] = 5;
    }
    if (cfg::display_wifi_info)
    {
        screens[screen_count++] = 6; // Wifi info
    }
    if (cfg::display_device_info)
    {
        screens[screen_count++] = 7; // chipID, firmware and count of measurements
    }
    // update size of "screens" when adding more screens!

    if (cfg::has_display || cfg::has_sh1106 || lcd_2004)
    {
        switch (screens[next_display_count % screen_count])
        {
        case 1:
            break;
        case 2:
            display_header = t_sensor;
            if (h_sensor && t_sensor != h_sensor)
            {
                display_header += " / " + h_sensor;
            }
            if ((h_sensor && p_sensor && (h_sensor != p_sensor)) || (h_sensor == "" && p_sensor && (t_sensor != p_sensor)))
            {
                display_header += " / " + p_sensor;
            }
            if (t_sensor != "")
            {
                display_lines[line_count] = "Temp.: ";
                display_lines[line_count] += check_display_value(t_value, -128, 1, 6);
                display_lines[line_count++] += " °C";
            }
            if (h_sensor != "")
            {
                display_lines[line_count] = "Hum.:  ";
                display_lines[line_count] += check_display_value(h_value, -1, 1, 6);
                display_lines[line_count++] += " %";
            }
            if (p_sensor != "")
            {
                display_lines[line_count] = "Pres.: ";
                display_lines[line_count] += check_display_value(p_value / 100, (-1 / 100.0), 1, 6);
                display_lines[line_count++] += " hPa";
            }
            while (line_count < 3)
            {
                display_lines[line_count++] = emptyString;
            }
            break;
        case 3:
            display_header = "NEO6M";
            display_lines[0] = "Lat: ";
            display_lines[0] += check_display_value(lat_value, -200.0, 6, 10);
            display_lines[1] = "Lon: ";
            display_lines[1] += check_display_value(lon_value, -200.0, 6, 10);
            display_lines[2] = "Alt: ";
            display_lines[2] += check_display_value(alt_value, -1000.0, 2, 10);
            break;
        case 4:
            display_header = FPSTR(SENSORS_DNMS);
            display_lines[0] = std::move(tmpl(F("LAeq: {v} db(A)"), check_display_value(la_eq_value, -1, 1, 6)));
            display_lines[1] = std::move(tmpl(F("LA_max: {v} db(A)"), check_display_value(la_max_value, -1, 1, 6)));
            display_lines[2] = std::move(tmpl(F("LA_min: {v} db(A)"), check_display_value(la_min_value, -1, 1, 6)));
            break;
        case 5:
            display_header = F("Wifi info");
            display_lines[0] = "IP: ";
            display_lines[0] += WiFi.localIP().toString();
            display_lines[1] = "SSID: ";
            display_lines[1] += WiFi.SSID();
            display_lines[2] = std::move(tmpl(F("Signal: {v} %"), String(calcWiFiSignalQuality(last_signal_strength))));
            break;
        case 6:
            display_header = F("Device Info");
            display_lines[0] = "ID: ";
            display_lines[0] += esp_chipid;
            display_lines[1] = "FW: ";
            display_lines[1] += SOFTWARE_VERSION;
            display_lines[2] = F("Measurements: ");
            display_lines[2] += String(count_sends);
            break;
        }

        if (cfg::has_display)
        {
            display.clear();
            display.displayOn();
            display.setTextAlignment(TEXT_ALIGN_CENTER);
            display.drawString(64, 1, display_header);
            display.setTextAlignment(TEXT_ALIGN_LEFT);
            display.drawString(0, 16, display_lines[0]);
            display.drawString(0, 28, display_lines[1]);
            display.drawString(0, 40, display_lines[2]);
            display.setTextAlignment(TEXT_ALIGN_CENTER);
            display.drawString(64, 52, displayGenerateFooter(screen_count));
            display.display();
        }
        if (cfg::has_sh1106)
        {
            display_sh1106.clear();
            display_sh1106.displayOn();
            display_sh1106.setTextAlignment(TEXT_ALIGN_CENTER);
            display_sh1106.drawString(64, 1, display_header);
            display_sh1106.setTextAlignment(TEXT_ALIGN_LEFT);
            display_sh1106.drawString(0, 16, display_lines[0]);
            display_sh1106.drawString(0, 28, display_lines[1]);
            display_sh1106.drawString(0, 40, display_lines[2]);
            display_sh1106.setTextAlignment(TEXT_ALIGN_CENTER);
            display_sh1106.drawString(64, 52, displayGenerateFooter(screen_count));
            display_sh1106.display();
        }
        if (lcd_2004)
        {
            display_header = std::move(String((next_display_count % screen_count) + 1) + '/' + String(screen_count) + ' ' + display_header);
            display_lines[0].replace(" µg/m³", emptyString);
            display_lines[0].replace("°", String(char(223)));
            display_lines[1].replace(" µg/m³", emptyString);
            lcd_2004->clear();
            lcd_2004->setCursor(0, 0);
            lcd_2004->print(display_header);
            lcd_2004->setCursor(0, 1);
            lcd_2004->print(display_lines[0]);
            lcd_2004->setCursor(0, 2);
            lcd_2004->print(display_lines[1]);
            lcd_2004->setCursor(0, 3);
            lcd_2004->print(display_lines[2]);
        }
    }

    // ----5----0----5----0
    // T/H: -10.0°C/100.0%
    // T/P: -10.0°C/1000hPa

    if (lcd_1602)
    {
        switch (screens[next_display_count % screen_count])
        {
        case 1:
            break;
        case 2:
            break;
        case 3:
            display_lines[0] = std::move(tmpl(F("T: {v} °C"), check_display_value(t_value, -128, 1, 6)));
            display_lines[1] = std::move(tmpl(F("H: {v} %"), check_display_value(h_value, -1, 1, 6)));
            break;
        case 4:
            display_lines[0] = "Lat: ";
            display_lines[0] += check_display_value(lat_value, -200.0, 6, 11);
            display_lines[1] = "Lon: ";
            display_lines[1] += check_display_value(lon_value, -200.0, 6, 11);
            break;
        case 5:
            display_lines[0] = std::move(tmpl(F("LAeq: {v} db(A)"), check_display_value(la_eq_value, -1, 1, 6)));
            display_lines[1] = std::move(tmpl(F("LA_max: {v} db(A)"), check_display_value(la_max_value, -1, 1, 6)));
            break;
        case 6:
            display_lines[0] = WiFi.localIP().toString();
            display_lines[1] = WiFi.SSID();
            break;
        case 7:
            display_lines[0] = "ID: ";
            display_lines[0] += esp_chipid;
            display_lines[1] = "FW: ";
            display_lines[1] += SOFTWARE_VERSION;
            break;
        }

        display_lines[0].replace("°", String(char(223)));

        lcd_1602->clear();
        lcd_1602->setCursor(0, 0);
        lcd_1602->print(display_lines[0]);
        lcd_1602->setCursor(0, 1);
        lcd_1602->print(display_lines[1]);
    }
    yield();
    next_display_count++;
}

/*****************************************************************
 * Init OLED display                                             *
 *****************************************************************/
static void init_display()
{
    display.init();
    display_sh1106.init();
    if (cfg::has_flipped_display)
    {
        display.flipScreenVertically();
        display_sh1106.flipScreenVertically();
    }
}

/*****************************************************************
 * Init LCD display                                              *
 *****************************************************************/
static void init_lcd()
{
    if (cfg::has_lcd1602)
    {
        lcd_1602 = new LiquidCrystal_I2C(0x3f, 16, 2);
    }
    else if (cfg::has_lcd1602_27)
    {
        lcd_1602 = new LiquidCrystal_I2C(0x27, 16, 2);
    }
    if (lcd_1602)
    {
        lcd_1602->init();
        lcd_1602->backlight();
    }

    if (cfg::has_lcd2004)
    {
        lcd_2004 = new LiquidCrystal_I2C(0x3f, 20, 4);
    }
    else if (cfg::has_lcd2004_27)
    {
        lcd_2004 = new LiquidCrystal_I2C(0x27, 20, 4);
    }
    if (lcd_2004)
    {
        lcd_2004->init();
        lcd_2004->backlight();
    }
}