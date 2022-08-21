////////////////////////////////////////////
#ifdef DEBUG_MEMORY
#include "debug-memory/debug_memory.h"
#endif
////////////////////////////////////////////
#include "footswitch-test/footswitch-test.h"
#include "footswitch/footswitch.h"
////////////////////////////////////////////
#include "ansi-codes/ansi-codes.h"
#include "c_fsio/include/fsio.h"
#include "c_greatest/greatest/greatest.h"
#include "c_stringfn/include/stringfn.h"
#include "c_vector/include/vector.h"

////////////////////////////////////////////
static char EXECUTABLE_PATH[PATH_MAX + 1] = { 0 };
static char *EXECUTABLE;
static char *EXECUTABLE_PATH_DIRNAME;
static bool DEBUG_MODE_ENABLED = false;
void __attribute__((constructor)) __constructor__footswitch_test();
void __attribute__((destructor)) __destructor__footswitch_test();
void __footswitch_test__setup_executable_path(const char **argv);

///////////////////////////////////////////////////////////
#define SUITES()\
SUITE(s_footswitch_test_main_read) {\
  RUN_TEST(t_footswitch_test_read_pedals);\
}\
SUITE(s_footswitch_test_main_write_abc) {\
  RUN_TEST(t_footswitch_test_write_pedals_abc);\
}\
SUITE(s_footswitch_test_main_write_xyz) {\
  RUN_TEST(t_footswitch_test_write_pedals_xyz);\
}\
SUITE(s_footswitch_test_module) {\
  RUN_TEST(t_footswitch_test_module);\
}\
///////////////////////////////////////////////////////////
#define RUN_SUITES()\
  RUN_SUITE(s_footswitch_test);\
  RUN_SUITE(s_footswitch_test_main_read);\
  RUN_SUITE(s_footswitch_test_main_write_abc);\
  RUN_SUITE(s_footswitch_test_main_write_xyz);\
  RUN_SUITE(s_footswitch_test_module);\
///////////////////////////////////////////////////////////

TEST t_footswitch_test_module(void){
  int qty = fs->get_usb_devices_qty();
  ASSERT_GT(qty,0);
  fs->list_usb_devices();
  free_fs();
  PASS();
}

TEST t_footswitch_get_usb_devices_qty(void){
  int qty = footswitch_usb_devices_qty();
  ASSERT_GT(qty,0);
  char *msg;
  asprintf(&msg,"%d Connected USB Devices",qty);
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
  const int argc = 2;
  int res = footswitch_main(argc,(const char **)argv);
  ASSERT_EQ(res,0);
  PASS();
}
TEST t_footswitch_test_write_pedals_xyz(void){
  const char *argv[] = {
    "footswitch", 
    "-1","-kx",
    "-2","-ky",
    "-3","-kz",
    NULL,
  };
  const int argc = 7;
  int res = footswitch_main(argc,(const char **)argv);
  ASSERT_EQ(res,0);
  PASS();
}
TEST t_footswitch_test_write_pedals_abc(void){
  const char *argv[] = {
    "footswitch", 
    "-1","-ka",
    "-2","-kb",
    "-3","-kc",
    NULL,
  };
  const int argc = 7;
  int res = footswitch_main(argc,(const char **)argv);
  ASSERT_EQ(res,0);
  PASS();
}
TEST t_footswitch_test(void *PARAMETER){
  char *msg         = NULL;
  int  tested_value = -1;

  tested_value = 1;
  ASSERT_EQm("Test Value should equal One", tested_value, 1);
  asprintf(&msg, "Test footswitch-test OK\n\t(%s) @ (%s)\n\tValidated tested_value: %d",
           EXECUTABLE,
           EXECUTABLE_PATH_DIRNAME,
           tested_value
           );
  PASSm(msg);
}

SUITES()

SUITE(s_footswitch_test) {
  void *TEST_PARAM = 0;

  RUN_TESTp(t_footswitch_test, (void *)TEST_PARAM);
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
