#include <syslog.h>
#include <unistd.h>

/**
 * Check the log messages with `journalctl -f` or `tail -f /var/log/syslog`
 */

int main(int argc, char* argv[]) {
  // Only sends the logs with equal or higher levels to syslog
  setlogmask(LOG_UPTO(LOG_INFO));

  openlog("cpp_playground/utils/syslog/hello_world", LOG_CONS | LOG_PID | LOG_PERROR | LOG_NDELAY,
          LOG_LOCAL1);

  syslog(LOG_DEBUG, "A tree shakes in a forest.");              // ignored according to the mask
  syslog(LOG_INFO, "A tree falls in a forest.");                // sent according to the mask
  syslog(LOG_NOTICE, "Program started by user %d.", getuid());  // sent according to the mask
  syslog(LOG_ERR, "A tree explodes in a forest.");              // sent according to the mask

  closelog();
  return 0;
}
