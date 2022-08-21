#pragma once
#include "footswitch/footswitch.h"
#include "module/def.h"
#include "module/module.h"
#include "module/require.h"

module(footswitch_module) {
  define(footswitch_module, CLIB_MODULE);
  void (*list_usb_devices)(void);
  int  (*get_usb_devices_qty)(void);
};

int  footswitch_module_init(module(footswitch_module) * exports);
void footswitch_module_deinit(module(footswitch_module) * exports);

exports(footswitch_module) {
  .init                = footswitch_module_init,
  .deinit              = footswitch_module_deinit,
  .list_usb_devices    = NULL,
  .get_usb_devices_qty = NULL,
};

#define fs    require(footswitch_module)
#define free_fs()    clib_module_free(fs)
