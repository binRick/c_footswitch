#include "common.h"
#include "debug.h"
#include <errno.h>
#include <hidapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct pedal_data {
  unsigned char header[8];
  unsigned char data[48];
  int           data_len;
} pedal_data;

typedef struct pedal_protocol {
  unsigned char start[8];
  pedal_data    pedals[3];
} pedal_protocol;

void write_pedals();
void write_pedal(pedal_data *pedal);
void compile_mouse_xyw(const char *mx, const char *my, const char *mw);
void compile_mouse_button(const char *btn_str);
void compile_modifier(const char *mod_str);
void compile_key(const char *key);
void compile_raw_string(const char *str);
void compile_string_key(const char *key);
void compile_string(const char *str);
void compile_string_data(unsigned char *data, size_t len);
Bool set_pedal_type(unsigned char new_type);
void read_pedals();
void print_string(unsigned char data[]);
void print_key(unsigned char data[]);
void print_mouse(unsigned char data[]);
void usb_write(unsigned char data[8]);
void deinit();
void init_pedals();
void init_pedal(pedal_data *p, int num);
void init();
void init_pid(unsigned short vid, unsigned short pid);
void usage();
extern pedal_data     *curr_pedal;
extern pedal_protocol pd;
extern hid_device     *dev;
