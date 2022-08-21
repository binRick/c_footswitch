#include "hidapi/hidapi.h"
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

struct footswitch_t {
  struct hid_device_info device;
};

struct footswitch_key_t {
};

int footswitch_usb_devices_qty(void);
void footswitch_enumerate_devs(void);

int footswitch_main(const int argc, const char **argv);
