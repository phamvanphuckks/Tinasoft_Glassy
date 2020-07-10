#ifndef _VL53L0X_PLATFORM_LOG_H_
#define _VL53L0X_PLATFORM_LOG_H_

#include <stdio.h>
#include <string.h>
/* LOG Functions */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file vl53l0_platform_log.h
 *
 * @brief platform log function definition
 */

//#define VL53L0X_LOG_ENABLE 0

enum {
  TRACE_LEVEL_NONE,
  TRACE_LEVEL_ERRORS,
  TRACE_LEVEL_WARNING,
  TRACE_LEVEL_INFO,
  TRACE_LEVEL_DEBUG,
  TRACE_LEVEL_ALL,
  TRACE_LEVEL_IGNORE
};

enum {
  TRACE_FUNCTION_NONE = 0,
  TRACE_FUNCTION_I2C = 1,
  TRACE_FUNCTION_ALL = 0x7fffffff // all bits except sign
};

enum {
  TRACE_MODULE_NONE = 0x0,
  TRACE_MODULE_API = 0x1,
  TRACE_MODULE_PLATFORM = 0x2,
  TRACE_MODULE_ALL = 0x7fffffff // all bits except sign
};

#ifdef VL53L0X_LOG_ENABLE

#include <sys/time.h>

extern uint32_t _trace_level;

int32_t VL53L0X_trace_config(char *filename, uint32_t modules, uint32_t level,
                             uint32_t functions);

void trace_print_module_function(uint32_t module, uint32_t level,
                                 uint32_t function, const char *format, ...);

// extern FILE * log_file;

#define LOG_GET_TIME() (int)clock()

#define _LOG_FUNCTION_START(module, fmt, ...)                                  \
  trace_print_module_function(module, _trace_level, TRACE_FUNCTION_ALL,        \
                              "%ld <START> %s " fmt "\n", LOG_GET_TIME(),      \
                              __FUNCTION__, ##__VA_ARGS__);

#define _LOG_FUNCTION_END(module, status, ...)                                 \
  trace_print_module_function(module, _trace_level, TRACE_FUNCTION_ALL,        \
                              "%ld <END> %s %d\n", LOG_GET_TIME(),             \
                              __FUNCTION__, (int)status, ##__VA_ARGS__)

#define _LOG_FUNCTION_END_FMT(module, status, fmt, ...)                        \
  trace_print_module_function(module, _trace_level, TRACE_FUNCTION_ALL,        \
                              "%ld <END> %s %d " fmt "\n", LOG_GET_TIME(),     \
                              __FUNCTION__, (int)status, ##__VA_ARGS__)

// __func__ is gcc only
//#define VL53L0X_ErrLog( fmt, ...)  fprintf(stderr, "VL53L0X_ErrLog %s" fmt
//"\n", __func__, ##__VA_ARGS__)

#else /* VL53L0X_LOG_ENABLE no logging */
#define VL53L0X_ErrLog(...) (void)0
#define _LOG_FUNCTION_START(module, fmt, ...) (void)0
#define _LOG_FUNCTION_END(module, status, ...) (void)0
#define _LOG_FUNCTION_END_FMT(module, status, fmt, ...) (void)0
#endif /* else */

#define VL53L0X_COPYSTRING(str, ...) strcpy(str, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* _VL53L0X_PLATFORM_LOG_H_ */
