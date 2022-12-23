/**************************************************************************$
 * Copyright (C) 2022 Andre Riesberg, andre@riesberg-net.de
 *
 * Redistribution and use in source and binary forms of this file,
 * with or without modification, are permitted. See the Creative
 * Commons Zero (CC0 1.0) License for more details.
 ***************************************************************************/
 /*
   Minimal standalone software for a tinkerforge ESP32-Ethernet brick. 
   Use this as a skeleton version for your own standalone projects.

   For the integration into the thinkerforg universe please use the 
   original firmware!
 */
//---------------------------------------------------------------------------
#include "main.h"
#include "hal_arduino_esp32_ethernet_brick/hal_arduino_esp32_ethernet_brick.h"
#include "bindings/errors.h"
#include "bindings/bricklet_rgb_led_button.h"
// Note: For other bricklets copy the required files to the "bindings" folder 
#include <ETH.h>
#ifdef HAS_OTA
  #include <ArduinoOTA.h>
#endif
//---------------------------------------------------------------------------
static const char *Tag = __FILE__;
//---------------------------------------------------------------------------
#define LED_BLUE_PIN   15 // On board LED
#define LED_GREEN_PIN   2 // Link LED
#define ETH_POWER_PIN   5
#define ETH_TYPE        ETH_PHY_KSZ8081
//---------------------------------------------------------------------------
static TF_HAL hal;
static TF_RGBLEDButton rgbLEDButton;
//---------------------------------------------------------------------------
struct {
  IPAddress localIP;
} ethLink;
//---------------------------------------------------------------------------
void setStatusLED(bool on) {
  digitalWrite(LED_BLUE_PIN, !on);
}
//---------------------------------------------------------------------------
void setLinkLED(bool on) {
  digitalWrite(LED_GREEN_PIN, on);
}
//---------------------------------------------------------------------------
static void __Error(const char *file, uint32_t line, const char *message) {
  ESP_LOGE(Tag, "%s in %s:%u\n", message, file, line);
  for (;;) {
    delay(100);
    setStatusLED(true); 
    delay(100);
    setStatusLED(false);
  }
}
#define ERROR(message) __Error(__FILE__, __LINE__, message)
//---------------------------------------------------------------------------
static void __Check(const char *file, uint32_t line, esp_err_t result) {
  if (result != ESP_OK) {
    char s[80];
    if (result > 0) {
      esp_err_to_name_r(result, s, sizeof(s));
      __Error(file, line, esp_err_to_name(result));
    } else {
      sniprintf(s, sizeof(s), "Check fail %d", result);
      __Error(file, line, s);
    }
  }
}
#define CHECK(result) __Check(__FILE__, __LINE__, result)
//---------------------------------------------------------------------------
void setup() {
  Tag = Tag;
  ESP_LOGI(Tag, __DATE__ " " __TIME__ ", free memory %u kB", xPortGetFreeHeapSize() / 1024);

  pinMode(LED_BLUE_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  setStatusLED(false);
  setLinkLED(false);
  
  // If you got an error on unknown "ETH_PHY_KSZ8081" update espressif32 to version >= 5.2
  ETH.begin(0, ETH_POWER_PIN, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_KSZ8081);
  ETH.setHostname(APP_NAME);  

  CHECK(tf_hal_create(&hal));

  for (uint8_t i = 0; i < TF_INVENTORY_SIZE; i++) {
    char ret_uid_str[7]; 
    char ret_port_name[2];
    uint16_t ret_device_id;
    if (tf_hal_get_device_info(&hal, i, ret_uid_str, ret_port_name, &ret_device_id) == TF_E_OK) 
      ESP_LOGI(Tag, "Bricklet %u at port %c, uid is %s", ret_device_id, ret_port_name[0], ret_uid_str);
  } 

  CHECK(tf_rgb_led_button_create(&rgbLEDButton, "A", &hal));
}
//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
void loop() {
  static uint8_t count = 0;
  setStatusLED(!(count++ & 0x20));

  {
    IPAddress localIP = ETH.localIP();
    if (localIP != ethLink.localIP) {
      ethLink.localIP = localIP;
      if (ethLink.localIP == IPAddress()) {
        // Link down
        setLinkLED(false);
        #ifdef HAS_OTA
          ArduinoOTA.end();
        #endif
      } else {
        // Link up
        setLinkLED(true);
        ESP_LOGI(Tag, "Ethernet: %s", ethLink.localIP.toString().c_str());
        #ifdef HAS_OTA
          ArduinoOTA.begin();
        #endif
      }
    }  
  }

  if (!(count & 0x3F)) {
    // Only send if a color change is desired
    static bool blink = false;
    if (blink) 
      tf_rgb_led_button_set_color(&rgbLEDButton, 0, 0, 0);
    else
      tf_rgb_led_button_set_color(&rgbLEDButton, 0, 255, 0);
    blink = !blink;
  }

  tf_hal_callback_tick(&hal, 0);

  #ifdef HAS_OTA
    ArduinoOTA.handle();
  #endif

  delay(10);
}
//-------------------------------------------------------------------------------
