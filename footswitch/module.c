#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
///////////////////////////////
#include "footswitch/footswitch.h"
#include "footswitch/module.h"
///////////////////////////////

int footswitch_module_init(module(footswitch_module) *exports) {
  printf("footswitch init>\n");
  clib_module_init(footswitch_module, exports);
  exports->pid = getpid();
  return(0);
}

void footswitch_module_deinit(module(footswitch_module) *exports) {
  printf("<%d> footswitch deinit>\n", exports->pid);
}
