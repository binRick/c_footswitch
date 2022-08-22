#pragma once
#include "footswitch/def.h"
#include "module/def.h"
#include "module/module.h"
#include "module/require.h"

module(footswitch_module) {
  define(footswitch_module, CLIB_MODULE);
  void  (*list_usb_devices)(void);
  int   (*get_usb_devices_qty)(void);
  pid_t pid;
};

int  footswitch_module_init(module(footswitch_module) * exports);
void footswitch_module_deinit(module(footswitch_module) * exports);

#include "footswitch/exports.h"

exports(footswitch_module) {
  .init                = footswitch_module_init,
  .deinit              = footswitch_module_deinit,
  .list_usb_devices    = footswitch_enumerate_devs,
  .get_usb_devices_qty = footswitch_usb_devices_qty,
  .pid                 = 0,
};
