#ifndef UTILS
#define UTILS

typedef unsigned int uint;

/**
 * Error logging code attributed to Andrei Ciobanu (https://github.com/nomemory)
 * from his linear algebra library repo (https://github.com/nomemory/neat-matrix-library)
*/
#define DEBUG_TRUE 1

#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define YELLOW "\033[0;33m"
#define RESET "\033[0m"

void basis_log(FILE *stream, const char *file_name,
  unsigned int line, const char *format, ...);

void basis_vlog(FILE* stream, const char *file_name,
  unsigned int line, const char *format, va_list argp);

#define BASIS_FLOG(stream, fmt, ...) \
      if (DEBUG_TRUE) { \
        basis_log(stream, __FILE__, __LINE__, fmt, __VA_ARGS__); \
      } \

#define BASIS_FINFO(fmt, ...) \
      if (DEBUG_TRUE) { \
          basis_log(stdout, __FILE__, __LINE__, fmt, __VA_ARGS__); \
      } \

#define BASIS_FERROR(fmt, ...) \
      if (DEBUG_TRUE) { \
          basis_log(stderr, __FILE__, __LINE__, RED fmt RESET, __VA_ARGS__); \
      } \

#define BASIS_ERROR(fmt) \
      if (DEBUG_TRUE) { \
        basis_log(stderr, __FILE__, __LINE__, RED fmt RESET); \
      } \


double rand_interval(double min, double max);
void map(void (*f) (void *, void *, void **), int n, void **a0, void **a1, void **d);

#endif