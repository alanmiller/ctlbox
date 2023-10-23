#include <stdio.h>
#include <pico/stdlib.h>
#include "pico/cyw43_arch.h"
#include <inttypes.h>
#include "tusb.h"
#include "tusb_config.h"
#include "usb_descriptors.h"

//    button number       1      2     3    4      5     6     7     8      9    10
//    keycode             i      o    F1    k     F3    F9     9     0    left  right
//    gpio pins           6      7     8    9     10    11    12    13     20     21 
uint8_t key_usage[10] = {0x0c, 0x12, 0x3a, 0x0e, 0x3c, 0x42, 0x26, 0x27, 0x50, 0x4f};
uint button_pins[10] =  {6, 7, 8, 9, 10, 11, 12, 13, 20, 21};
bool button_state[10] = {false};


// This callback will be invoked when we need to send a report.
void tud_hid_report_cb(uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t bufsize) {
    // Clear the report
    memset(buffer, 0, bufsize);

    // Set the usage codes for the keys that are currently pressed
    int key_count = 0;
    for (int i = 0; i < 10; i++) {
        if (button_state[i] && key_count < 10) {
            buffer[key_count++] = key_usage[i];
        }
    }
}

bool read_button_state(int button) {
    // Assuming button_pins[i] is the GPIO pin number for button i
    // and the button is wired with a pull-up resistor
    // gpio_get() should return false when the button is pressed and true when it's not pressed
    return !gpio_get(button_pins[button]);
}

int main(void) {
  // Initialize the Pico.
  stdio_init_all();
  tusb_init();
  
  puts("Initializing controlbox firmware....");
  if (cyw43_arch_init()) {
    puts("  Wi-Fi init failed");
  } else {
    puts("  Wi-Fi initialized.");
    // cyw43_arch_enable_sta_mode();
  }

  // Initialize the GPIO pins
  for (int i = 0; i < 10; i++) {
    gpio_init(button_pins[i]);
    gpio_pull_up(button_pins[i]);
    gpio_set_dir(button_pins[i], GPIO_IN);
  }
  puts("  Buttons initialized.");
  puts("  Polling buttons:");

  while (1) {
        // Let TinyUSB do its work
        tud_task();
        // Update the button states and send a report if necessary
        uint8_t keycode[6] = { 0, 0, 0, 0, 0, 0 };
        for (int i = 0; i < 10; i++) {
            bool new_state = read_button_state(i);
            bool old_state = button_state[i];
            //printf("Debug: Button %d, old_state: %s, new_state: %s\n", i+1, old_state ? "true" : "false", new_state ? "true" : "false");
            if (old_state != new_state) {
                // Button state has changed; send a keyboard(1)  or a gamepad report(2)
                if (new_state) {
                  // Key press set the first keycode to the key usag
                  printf("  Button %d, gpio %d, old/new state: %s/%s , sending: %x\n",
                        i+1, 
                        (int)button_pins[i], 
                        old_state ? "true" : "false", 
                        new_state ? "true" : "false", 
                        key_usage[i]);
                  keycode[0] = key_usage[i];
                } else {
                  // Key release
                  printf("  Button %d released\n", i+1 );
                  memset(keycode, 0, sizeof(keycode));
                }
                tud_hid_keyboard_report(1, 0, keycode);
            }
            // update states
            button_state[i] = new_state; 
        }
  }
  return 0;
}