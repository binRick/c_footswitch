#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
///////////////////////////////
#include "footswitch/footswitch.h"
#include "footswitch/module.h"
///////////////////////////////
static inline void footswitch_module_test(char *message);

int footswitch_module_init(module(footswitch_module) *exports) {
  printf("footswitch init>\n");
  clib_module_init(footswitch_module, exports);
  exports->get_usb_devices_qty = footswitch_usb_devices_qty;
  exports->list_usb_devices    = footswitch_enumerate_devs;
  return(0);
}

void footswitch_module_deinit(module(footswitch_module) *exports) {
  printf("footswitch deinit>\n");
}
