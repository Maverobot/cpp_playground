#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER hello_world

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "hello-tp.h"

#if !defined(_HELLO_TP_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _HELLO_TP_H

#include <lttng/tracepoint.h>

TRACEPOINT_EVENT(hello_world,
                 my_first_tracepoint,
                 TP_ARGS(char*, message),
                 TP_FIELDS(ctf_string(message, message)))

#endif /* _HELLO_TP_H */

#include <lttng/tracepoint-event.h>
