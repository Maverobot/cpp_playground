#pragma once

#include <lely/util/diag.h>
#include <syslog.h>
#include <cstdlib>

/**
 * @brief The diag() handler used for the sys logging.
 *
 * @param handle   the extra argument specified to diag_at_set_handler().
 * @param severity the severity of the message.
 * @param errc     the native error code.
 * @param at       a pointer the location in a text file (can be NULL).
 * @param format   a printf-style format string.
 * @param ap       the list with arguments to be printed according to
 *                 <b>format</b>.
 *
 * Use it with `diag_at_set_handler(&syslog_diag_at_handler, nullptr);`.
 *
 */
inline void syslog_diag_at_handler(void* handle,
                                   enum diag_severity severity,
                                   int errc,
                                   const struct floc* at,
                                   const char* format,
                                   va_list ap) {
  (void)handle;

  int errsv = errno;
  char* s = nullptr;
  if (vasprintf_diag_at(&s, DIAG_INFO, errc, at, format, ap) >= 0) {
    int level = LOG_INFO;
    switch (severity) {
      case DIAG_DEBUG:
        level = LOG_DEBUG;
        break;
      case DIAG_INFO:
        level = LOG_INFO;
        break;
      case DIAG_WARNING:
        level = LOG_WARNING;
        break;
      case DIAG_ERROR:
        level = LOG_ERR;
        break;
      case DIAG_FATAL:
        level = LOG_CRIT;
        break;
    }
    syslog(level, "lely: %s", s);
  }
  free(s);
  errno = errsv;
}

/**
 * @brief The diag() handler used for the sys logging.
 *
 * @param handle   the extra argument specified to diag_set_handler().
 * @param severity the severity of the message.
 * @param errc     the native error code.
 * @param format   a printf-style format string.
 * @param ap       the list with arguments to be printed according to
 *                 <b>format</b>.
 *
 * Use it with `diag_set_handler(&syslog_diag_handler, nullptr);`.
 */
inline void syslog_diag_handler(void* handle,
                                enum diag_severity severity,
                                int errc,
                                const char* format,
                                va_list ap) {
  syslog_diag_at_handler(handle, severity, errc, nullptr, format, ap);
}

/**
 * @brief Sets the diag() handlers to the sys logging.
 */
inline void set_lely_diag_handler_with_syslog() {
  diag_set_handler(&syslog_diag_handler, nullptr);
  diag_at_set_handler(&syslog_diag_at_handler, nullptr);
}
