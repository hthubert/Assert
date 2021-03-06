#include <iostream>
#include <vector>

#include <pempek_assert.h>

// custom prefix
#define ASSERT          PEMPEK_ASSERT
#define ASSERT_WARNING  PEMPEK_ASSERT_WARNING
#define ASSERT_DEBUG    PEMPEK_ASSERT_DEBUG
#define ASSERT_ERROR    PEMPEK_ASSERT_ERROR
#define ASSERT_FATAL    PEMPEK_ASSERT_FATAL
#define ASSERT_CUSTOM   PEMPEK_ASSERT_CUSTOM
#define ASSERT_USED     PEMPEK_ASSERT_USED

void trigger_assert_debug_even(int i)
{
  ASSERT_DEBUG((i % 2) == 0, "not an even number: %d", i);
}

void trigger_assert_debug_odd(int i)
{
  ASSERT_DEBUG((i % 2) != 0, "not an odd number: %d", i);
}

void trigger_assert_error()
{
  void* ptr = 0;
  ASSERT_ERROR(ptr != 0, "invalid ptr: must not be null");
}

void trigger_assert_custom1()
{
  void* ptr = 0;
  ASSERT_CUSTOM(100, ptr != 0, "invalid ptr: must not be null");
}

ASSERT_USED(std::vector<int>) trigger_assert_unused_return_value1()
{
  return std::vector<int>(10);
}

#undef PEMPEK_ASSERT_ENABLED
#define PEMPEK_ASSERT_ENABLED 0
#include <pempek_assert.h>

void trigger_assert_custom2()
{
  void* ptr = 0;
  ASSERT_CUSTOM(100, ptr != 0, "invalid ptr: must not be null");
}

ASSERT_USED(int) trigger_assert_unused_return_value2()
{
  return 0;
}

#undef PEMPEK_ASSERT_ENABLED
#define PEMPEK_ASSERT_ENABLED 1
#include <pempek_assert.h>

int main()
{
  ASSERT(true);
  ASSERT_WARNING(true);
  ASSERT_DEBUG(true);
  ASSERT_ERROR(true);
  ASSERT_FATAL(true);
  ASSERT_CUSTOM(0, true);

  for (int i = 0; i < 5; ++i)
    trigger_assert_debug_even(i);

  for (int i = 0; i < 5; ++i)
    trigger_assert_debug_odd(i);

  try
  {
    trigger_assert_error();
  }
  catch(const pempek::assert::AssertionException& e)
  {
    std::cout << "AssertionException caught:" << std::endl;
    std::cout << "  [file]:       " << e.file() << std::endl;
    std::cout << "  [line]:       " << e.line() << std::endl;
    std::cout << "  [function]:   " << e.function() << std::endl;
    std::cout << "  [expression]: " << e.expression() << std::endl;
    std::cout << "  [what]:       " << e.what() << std::endl;
    std::cout << std::endl;
  }

  trigger_assert_custom1();

  trigger_assert_custom2();

  {
    std::vector<int> v = trigger_assert_unused_return_value1();
    v.clear();

    // trigger assert on scope exit
    trigger_assert_unused_return_value1();
  }

  {
    trigger_assert_unused_return_value2();
  }

  ASSERT_FATAL(false, "He's dead. He's dead, Jim");

  std::cout << std::endl;
  std::cout << "if you see this message, this means you decided to ignore all assertions" << std::endl;

  return 0;
}
