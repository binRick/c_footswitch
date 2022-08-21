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

TEST t_footswitch_test_main(void){
  const char *argv[] = {
    "footswitch", "-r"
  };
  const int argc = 2;
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

SUITE(s_footswitch_test_main) {
  RUN_TEST(t_footswitch_test_main);
}
SUITE(s_footswitch_test) {
  void *TEST_PARAM = 0;

  RUN_TESTp(t_footswitch_test, (void *)TEST_PARAM);
}

GREATEST_MAIN_DEFS();

int main(const int argc, char **argv) {
  __footswitch_test__setup_executable_path((const char **)argv);
  GREATEST_MAIN_BEGIN();
  if (isatty(STDOUT_FILENO)) {
  }
  RUN_SUITE(s_footswitch_test);
  RUN_SUITE(s_footswitch_test_main);
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
