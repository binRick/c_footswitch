////////////////////////////////////////////
#ifdef DEBUG_MEMORY
//#include "debug-memory/debug_memory.h"
#endif
////////////////////////////////////////////
#include "footswitch-test/footswitch-test.h"
#include "footswitch/footswitch.h"
//INCBIN(ResticBinary,"restic");
#include "incbin/incbin.h"
////////////////////////////////////////////
#include "ansi-codes/ansi-codes.h"
#include "c_fsio/include/fsio.h"
#include "c_greatest/greatest/greatest.h"
#include "c_string_buffer/include/stringbuffer.h"
#include "c_stringfn/include/stringfn.h"
#include "c_vector/vector/vector.h"
#include "roscha/include/hmap.h"
#include "roscha/include/object.h"
#include "roscha/include/parser.h"
#include "roscha/include/roscha.h"
#include "roscha/include/sds/sds.h"
#include "roscha/include/token.h"
#include "roscha/include/vector.h"
#include "str-flatten.c/src/str-flatten.h"
char *skhrc_template = "name={{name}}\n"
                       "var1={{var1}}\n"
                       "item.name={{item.name}}, item.age={{item.age}}\n"
                       "items list=\n"
                       "{\%for i in items\%}"
                       " \t#{{loop.index}}> item={{i}}\n"
                       "{\%endfor\%}\n"
                       "items obj list=\n"
                       ""
                       "{\%for i in item_objects\%}"
                       " \t#{{loop.index}}> name={{i.name}}\n"
                       "{\%endfor\%}\n"
                       "";
/*
 * TEST t_roscha(void){
 * roscha(void);
 * PASS();
 * }
 *
 * static void test_eval_variable(void){
 * roscha_init();
 * struct roscha_env *env = roscha_env_new();
 * env->vars = roscha_object_new(hmap_new());
 *
 * roscha_hmap_set_new(env->vars, "name", "world");
 * roscha_hmap_set_new(env->vars, "var1", 1);
 *
 * struct roscha_object *item = roscha_object_new(hmap_new());
 * roscha_hmap_set_new(item, "name", "i");
 * roscha_hmap_set_new(item, "age", 123);
 * roscha_hmap_set(env->vars, "item", item);
 *
 * struct roscha_object *item_objects = roscha_object_new_vector(roscha_vector_new());
 *
 * struct roscha_object *i0 = roscha_object_new(hmap_new());
 * roscha_hmap_set_new(i0, "name", "i0");
 * roscha_vector_push_object(item_objects, i0);
 *
 * struct roscha_object *i1 = roscha_object_new(hmap_new());
 * roscha_hmap_set_new(i1, "name", "i1");
 * roscha_vector_push_object(item_objects, i1);
 *
 * roscha_hmap_set(env->vars, "item_objects", item_objects);
 *
 * struct roscha_object *items = roscha_object_new(roscha_vector_new());
 * roscha_vector_push_new(items, (slice_whole("item0")));
 * roscha_vector_push_new(items, (slice_whole("item1")));
 * roscha_hmap_set(env->vars, "items", items);
 *
 * roscha_env_add_template(env, strdup("test"), input);
 *
 * sds got = roscha_env_render(env, "test");
 * printf("==============================\n");
 * printf("%s", input);
 * printf("==============================\n");
 * printf("%s", got);
 * printf("==============================\n");
 * roscha_deinit();
 * }
 */

enum footswitch_preset_type_t {
  PRESET_ONE,
  PRESET_TWO,
  PRESETS_QTY,
};
enum button_type_t {
  BUTTON_TYPE_LEFT,
  BUTTON_TYPE_CENTER,
  BUTTON_TYPE_RIGHT,
  BUTTON_TYPES_QTY,
};
struct footswitch_skhdrc_t {
  char *keys;
  char *cmd;
  char *text;
  char *name;
  char *sha256sum;
  bool enabled;
};
struct footswitch_preset_keys_t {
  char                       *key;
  char                       *modifiers[8];
  struct footswitch_skhdrc_t skhd;
};
struct footswitch_preset_t {
  struct footswitch_preset_keys_t buttons[BUTTON_TYPES_QTY + 1];
  char                            *skhdrc_file;
};

struct footswitch_preset_t footswitch_presets[] = {
  [PRESET_ONE] =             {
    .skhdrc_file = "/Users/rick/.skhdrc",
    .buttons     =           {
      [BUTTON_TYPE_LEFT] =   {
        .modifiers =         {
          "ctrl",
          "alt",
        },
        .key  = "9",
        .skhd =         {
          .name    = "TEST PRESET ONE",
          .enabled = true,
          .cmd     = "echo PRESET_ONE >> /tmp/footswitch.log",
        },
      },
      [BUTTON_TYPE_CENTER] = {
        .modifiers =         {
          "l_ctrl",
          "l_alt",
        },
        .key       = "8",
      },
      [BUTTON_TYPE_RIGHT] =  {
        .modifiers =         {
          "l_shift",
          "r_shift",
          "r_ctrl",
          "l_ctrl",
          "r_alt",
          "l_alt",
        },
        .key       = "6",
      },
      [BUTTON_TYPES_QTY] =   { 0 },
    },
  },
  [PRESET_TWO] =             {
    .buttons       =               {
      [BUTTON_TYPE_LEFT] =   {
        .modifiers =         {
          "r_shift",
          "l_shift",
          "l_alt",
        },
        .key       = "x",
      },
      [BUTTON_TYPE_CENTER] = {
        .modifiers =         {
          "r_shift",
          "l_shift",
          "l_alt",
        },
        .key       = "y",
      },
      [BUTTON_TYPE_RIGHT] =  {
        .modifiers =         {
          "r_shift",
          "l_shift",
          "l_alt",
        },
        .key       = "z",
      },
      [BUTTON_TYPES_QTY] =   { 0 },
    },
  },
  [PRESETS_QTY] =            { 0 },
};

struct parsed_footswitch_preset_t {
  char                          **argv;
  char                          *argv_s;
  int                           argc;
  enum footswitch_preset_type_t type;
  char                          *skhdrc_cmd_s;
};

struct parsed_footswitch_preset_t *parse_footswitch_preset(enum footswitch_preset_type_t preset_type){
  struct parsed_footswitch_preset_t *parsed_preset = calloc(1, sizeof(struct parsed_footswitch_preset_t));

  parsed_preset->type = preset_type;
  struct footswitch_preset_t *preset;
  struct Vector              *keys_v       = vector_new();
  struct Vector              *skhdrc_cmd_v = vector_new();

  preset = &(footswitch_presets[preset_type]);
  if (preset != NULL) {
    for (size_t i = 0; i < BUTTON_TYPES_QTY; i++) {
      char *b;
      asprintf(&b, "-%lu", i + 1);
      vector_push(keys_v, (void *)strdup(b));
      if (*(preset->buttons[i]).modifiers) {
        char **m = preset->buttons[i].modifiers;
        while (*m != NULL) {
          vector_push(skhdrc_cmd_v, (void *)strdup(*m));
          vector_push(skhdrc_cmd_v, (void *)strdup("+"));
          vector_push(keys_v, (void *)"-m");
          vector_push(keys_v, (void *)strdup(*m));
          m++;
        }
        if (*(preset->buttons[i].key)) {
          vector_push(keys_v, (void *)"-k");
          vector_push(keys_v, (void *)strdup(preset->buttons[i].key));
        }
      }
    }
  }
///////////////////////////////////////////////////////
  char skhdrc_key   = '6';
  char *cmd         = "date >> /tmp/footswitch.log";
  char *skhdrc_file = "/Users/rick/.skhdrc";

///////////////////////////////////////////////////////
  if (vector_size(keys_v) > 0) {
    vector_prepend(keys_v, (void *)"footswitch");
  }
  parsed_preset->argc         = (int)vector_size(keys_v);
  parsed_preset->argv         = (char **)vector_to_array(keys_v);
  parsed_preset->argv_s       = strdup((char *)str_flatten((const char **)parsed_preset->argv, 0, parsed_preset->argc));
  parsed_preset->skhdrc_cmd_s = stringfn_mut_trim(strdup((char *)str_flatten((const char **)vector_to_array(skhdrc_cmd_v), 0, vector_size(skhdrc_cmd_v))));
  if (stringfn_ends_with(parsed_preset->skhdrc_cmd_s, "+")) {
    stringfn_mut_substring(parsed_preset->skhdrc_cmd_s, 0, strlen(parsed_preset->skhdrc_cmd_s) - 1);
  }
  parsed_preset->skhdrc_cmd_s = stringfn_remove(parsed_preset->skhdrc_cmd_s, "_");
  struct StringBuffer *sb = stringbuffer_new();

  stringbuffer_append_string(sb, "\n\n##################################\n");
  stringbuffer_append_string(sb, "###   Footswitch Preset XXXX   ###\n");
  stringbuffer_append_string(sb, "##################################\n");
  stringbuffer_append_string(sb, stringfn_mut_trim(parsed_preset->skhdrc_cmd_s));
  stringbuffer_append_string(sb, " - ");
  stringbuffer_append(sb, skhdrc_key);
  stringbuffer_append_string(sb, ": ");
  stringbuffer_append_string(sb, cmd);
  stringbuffer_append_string(sb, "\n##################################\n\n");
  parsed_preset->skhdrc_cmd_s = stringbuffer_to_string(sb);
  vector_release(keys_v);
  stringbuffer_release(sb);

  printf(AC_YELLOW    "\targc :\t" AC_INVERSE "%d" AC_RESETALL "\n", parsed_preset->argc);
  printf(AC_BLUE      "\targv :\t" AC_INVERSE "%s" AC_RESETALL "\n", parsed_preset->argv_s);

  int res = footswitch_main(parsed_preset->argc, (const char **)parsed_preset->argv);

  if (res == 0) {
    if (fsio_file_exists(skhdrc_file)) {
      printf(AC_RED       "\tshkhd:\n" AC_INVERSE "%s" AC_RESETALL "\n", parsed_preset->skhdrc_cmd_s);
      fsio_append_text_file(skhdrc_file, parsed_preset->skhdrc_cmd_s);
    }
  }
  return(parsed_preset);
} /* parse_footswitch_preset */
////////////////////////////////////////////
static char EXECUTABLE_PATH[PATH_MAX + 1] = { 0 };
static char *EXECUTABLE;
static char *EXECUTABLE_PATH_DIRNAME;
static bool DEBUG_MODE_ENABLED = false;
void __attribute__((constructor)) __constructor__footswitch_test();
void __attribute__((destructor)) __destructor__footswitch_test();
void __footswitch_test__setup_executable_path(const char **argv);

///////////////////////////////////////////////////////////
#define SUITES()                                  \
  SUITE(s_footswitch_test_main_read) {            \
    RUN_TEST(t_footswitch_test_read_pedals);      \
  }                                               \
  SUITE(s_footswitch_test_main_write_abc) {       \
    RUN_TEST(t_footswitch_test_write_pedals_abc); \
  }                                               \
  SUITE(s_footswitch_test_main_write_xyz) {       \
    RUN_TEST(t_footswitch_test_write_pedals_xyz); \
  }                                               \
  SUITE(s_footswitch_test_module) {               \
    RUN_TEST(t_footswitch_test_module);           \
  }                                               \
///////////////////////////////////////////////////////////
#define RUN_SUITES()                           \
  RUN_SUITE(s_footswitch_test);                \
  RUN_SUITE(s_footswitch_test_main_read);      \
  RUN_SUITE(s_footswitch_test_main_write_abc); \
  RUN_SUITE(s_footswitch_test_main_write_xyz); \
  RUN_SUITE(s_footswitch_test_module);         \
///////////////////////////////////////////////////////////

TEST t_footswitch_test_module(void){
  int qty = fs->get_usb_devices_qty();

  ASSERT_GT(qty, 0);
  fs->list_usb_devices();
  free_fs();
  PASS();
}

TEST t_footswitch_get_usb_devices_qty(void){
  int qty = footswitch_usb_devices_qty();

  ASSERT_GT(qty, 0);
  char *msg;

  asprintf(&msg, "%d Connected USB Devices", qty);
  PASSm(msg);
}

TEST t_footswitch_test_enumerate_devices(void){
  footswitch_enumerate_devs();
  PASS();
}

TEST t_footswitch_test_read_pedals(void){
  const char *argv[] = {
    "footswitch", "-r",
    NULL,
  };
  const int  argc = 2;
  int        res  = footswitch_main(argc, (const char **)argv);

  ASSERT_EQ(res, 0);
  PASS();
}

TEST t_footswitch_test_write_pedals_xyz(void){
  const char *argv[] = {
    "footswitch",
    "-1",        "-kx",
    "-2",        "-ky",
    "-3",        "-kz",
    NULL,
  };
  const int  argc = 7;
  int        res  = footswitch_main(argc, (const char **)argv);

  ASSERT_EQ(res, 0);
  PASS();
}

TEST t_parse_footswitch_preset_one(void){
  parse_footswitch_preset(PRESET_ONE);
  PASS();
}

TEST t_parse_footswitch_preset_two(void){
  parse_footswitch_preset(PRESET_TWO);
  PASS();
}

TEST t_footswitch_test_write_pedals_abc(void){
  const char *argv[] = {
    "footswitch",
    "-1",        "-ka",
    "-2",        "-kb",
    "-3",        "-kc",
    NULL,
  };
  const int  argc = 7;
  int        res  = footswitch_main(argc, (const char **)argv);

  ASSERT_EQ(res, 0);
  PASS();
}

SUITES()
/*
 * SUITE(s_roscha) {
 * RUN_TEST(t_roscha);
 * }
 */
SUITE(s_footswitch_preset_one) {
  RUN_TEST(t_parse_footswitch_preset_one);
  RUN_TEST(t_footswitch_test_read_pedals);
}
SUITE(s_footswitch_preset_two) {
  RUN_TEST(t_parse_footswitch_preset_two);
  RUN_TEST(t_footswitch_test_read_pedals);
}
SUITE(s_footswitch_test) {
  RUN_TEST(t_footswitch_get_usb_devices_qty);
  RUN_TEST(t_footswitch_test_enumerate_devices);
}

GREATEST_MAIN_DEFS();

int main(const int argc, char **argv) {
  __footswitch_test__setup_executable_path((const char **)argv);
  GREATEST_MAIN_BEGIN();
  if (isatty(STDOUT_FILENO)) {
  }
  RUN_SUITES()
  RUN_SUITE(s_footswitch_preset_one);
  RUN_SUITE(s_footswitch_preset_two);
//  RUN_SUITE(s_roscha);
  GREATEST_MAIN_END();
}

void __footswitch_test__setup_executable_path(const char **argv){
  realpath(argv[0], EXECUTABLE_PATH);
  EXECUTABLE              = basename(EXECUTABLE_PATH);
  EXECUTABLE_PATH_DIRNAME = dirname(EXECUTABLE_PATH);
  if (DEBUG_MODE_ENABLED) {
    fprintf(stderr, "footswitch-test Initialized @ %s/%s\n", EXECUTABLE_PATH_DIRNAME, EXECUTABLE_PATH);
  }
}

void __attribute__((constructor)) __constructor__footswitch_test(){
  DEBUG_MODE_ENABLED = (getenv("DEBUG")) ? true
      : (getenv("DEBUG_MODE")) ? true
        : (getenv("DEBUGMODE")) ? true
          : false;
}

void __attribute__((destructor)) __destructor__footswitch_test(){
#ifdef DEBUG_MEMORY
  fprintf(stderr, "\nChecking footswitch-test memory leaks\n");
  print_allocated_memory();
#endif
}
