#ifndef SYNAPSE_ASSERT
#define SYNAPSE_ASSERT

#include <stdio.h>
#include <stdlib.h>
typedef int (*assert_ptr_t)(const char *, const char *, int, const char *);

#ifdef __cplusplus
extern "C" {
#endif
assert_ptr_t set_assert_handler(assert_ptr_t);
extern assert_ptr_t assert_handler_ptr;
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#define ASSERT_HALT() (std::abort())
#else
#define ASSERT_HALT() (abort())
#endif

#define ASSERT_HANDLER(x, y, z, t) ((*assert_handler_ptr)(x, y, z, t))
#define XASSERT(x, m) (!(x) && ASSERT_HANDLER(#x, __FILE__, __LINE__, m) && (ASSERT_HALT(), 1))
#define ASSERT(x) XASSERT(x, NULL)

#ifndef NDEBUG
#define DASSERT(x) ASSERT(x)
#else
#define DASSERT(x) (0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

assert_ptr_t
set_assert_handler(assert_ptr_t handler)
{
   assert_ptr_t old = assert_handler_ptr;
   assert_handler_ptr = handler;
   return old;
}

static int
default_handler(const char *expr, const char *file, int line, const char *msg)
{
   fprintf(stderr, "Assertion failed on %s line %d: %s\n", file, line, expr);
   if (msg != NULL)
      fprintf(stderr, "Diagnostic: %s\n", msg);
   return 1;
}

assert_ptr_t assert_handler_ptr = default_handler;

#ifdef __cplusplus
}
#endif

#endif // SYNAPSE_ASSERT