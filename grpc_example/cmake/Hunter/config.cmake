hunter_config(grpc
  URL https://github.com/grpc/grpc/archive/v1.26.0.tar.gz
  SHA1 69ac73b58c058e4c48ec1192f1a1c67a46f5194d)

# PIC is needed for compiling grpc
hunter_config(ZLIB VERSION ${HUNTER_ZLIB_VERSION}
              CMAKE_ARGS CMAKE_POSITION_INDEPENDENT_CODE=TRUE)
hunter_config(c-ares VERSION ${HUNTER_c-ares_VERSION}
              CMAKE_ARGS CMAKE_POSITION_INDEPENDENT_CODE=TRUE)
