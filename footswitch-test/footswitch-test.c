////////////////////////////////////////////
#ifdef DEBUG_MEMORY
//#include "debug-memory/debug_memory.h"
#endif
////////////////////////////////////////////
#include "footswitch-test/footswitch-test.h"
#include "footswitch/footswitch.h"
////////////////////////////////////////////
#include "ansi-codes/ansi-codes.h"
#include "c_fsio/include/fsio.h"
#include "c_greatest/greatest/greatest.h"
#include "c_stringfn/include/stringfn.h"
#include "c_string_buffer/include/stringbuffer.h"
#include "c_vector/include/vector.h"
#include "str-flatten.c/src/str-flatten.h"

enum footswitch_preset_type_t {
  PRESET_ONE,
  PRESETS_QTY,
};
enum button_type_t {
  BUTTON_TYPE_LEFT,
  BUTTON_TYPE_CENTER,
  BUTTON_TYPE_RIGHT,
  BUTTON_TYPES_QTY,
};

struct footswitch_preset_keys_t {
  char *key;
  char *modifiers[8];
};
struct footswitch_preset_t {
  struct footswitch_preset_keys_t buttons[BUTTON_TYPES_QTY+1];
};

struct footswitch_preset_t footswitch_presets[] = {
  [PRESET_ONE] = {
    .buttons = {
      [BUTTON_TYPE_LEFT] = {
        .modifiers = {
          "l_shift",
          "r_shift",
          "r_alt",
          NULL,
        },
        .key = "a",
      },
      [BUTTON_TYPE_CENTER] = {
        .modifiers = {
          "l_shift",
          "r_shift",
          "r_alt",
          NULL,
        },
        .key = "b",
      },
      [BUTTON_TYPE_RIGHT] = {
        .modifiers = {
          "l_shift",
          "r_shift",
          "r_alt",
          NULL,
        },
        .key = "c",
      },
      [BUTTON_TYPES_QTY] = { 0 },
    },
  },
  [PRESETS_QTY] = { 0 },
};

struct parsed_footswitch_preset_t {
  char **argv;
  char *argv_s;
  int argc;
  enum footswitch_preset_type_t type;
};

struct parsed_footswitch_preset_t *parse_footswitch_preset(enum footswitch_preset_type_t preset_type){
  struct parsed_footswitch_preset_t *parsed_preset = calloc(1,sizeof(struct parsed_footswitch_preset_t));
  parsed_preset->type = preset_type;
  struct footswitch_preset_t *preset;
  struct Vector *keys_v = vector_new();
  char *b;
  preset = &(footswitch_presets[preset_type]);
  if(preset != NULL){
    for(size_t i=0; i< BUTTON_TYPES_QTY;i++){
      asprintf(&b,"-%lu",i+1);
      vector_push(keys_v,(void*)strdup(b));
      if(*(preset->buttons[i]).modifiers){
        vector_push(keys_v,"-m");
        vector_push(keys_v,*(preset->buttons[i].modifiers));
        if(*(preset->buttons[i].key)){
          vector_push(keys_v,"-k");
          vector_push(keys_v,preset->buttons[i].key);
        }
      }
    }
  }

  if(vector_size(keys_v)>0)
    vector_prepend(keys_v,(void*)"footswitch");
  parsed_preset->argc = (int)vector_size(keys_v);
  parsed_preset->argv = (char**)vector_to_array(keys_v);
  parsed_preset->argv_s = strdup((char *)str_flatten((const char **)parsed_preset->argv,0,parsed_preset->argc));
  vector_release(keys_v);

  printf(AC_YELLOW    "\targc:\t" AC_INVERSE "%d" AC_RESETALL "\n",parsed_preset->argc);
  printf(AC_BLUE      "\targv:\t" AC_INVERSE "%s" AC_RESETALL "\n",parsed_preset->argv_s);

  return(parsed_preset);
}
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

TEST t_parse_footswitch_preset(void){
  parse_footswitch_preset(PRESET_ONE);
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

SUITE(s_footswitch_test) {
  RUN_TEST(t_parse_footswitch_preset);
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
