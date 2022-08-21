#pragma once
#ifndef __FS_COMMON_H__
#define __FS_COMMON_H__

enum modifier {
  CTRL    = 1,
  SHIFT   = 2,
  ALT     = 4,
  WIN     = 8,
  R_CTRL  = 16,
  R_SHIFT = 32,
  R_ALT   = 64,
  R_WIN   = 128,
};

enum mouse_button {
  MOUSE_LEFT   = 1,
  MOUSE_RIGHT  = 2,
  MOUSE_MIDDLE = 4,
  MOUSE_DOUBLE = 8,
};

typedef unsigned char Bool;

Bool parse_modifier(const char *arg, enum modifier *mod);
Bool parse_mouse_button(const char *arg, enum mouse_button *btn);
Bool encode_string(const char *str, unsigned char *arr);
Bool encode_key(const char *key, unsigned char *b);
const char * decode_byte(unsigned char b);

#endif
