# PIC is needed for compiling grpc
hunter_config(ZLIB VERSION ${HUNTER_ZLIB_VERSION}
              CMAKE_ARGS CMAKE_POSITION_INDEPENDENT_CODE=TRUE)
hunter_config(c-ares VERSION ${HUNTER_c-ares_VERSION}
              CMAKE_ARGS CMAKE_POSITION_INDEPENDENT_CODE=TRUE)
