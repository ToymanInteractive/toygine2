set(PLATFORM_SRC_CORE_LIST  src/platforms/stub/core/assertion_stub.cpp)
set(PLATFORM_HDR_CORE_LIST  )
set(PLATFORM_LIB_CORE_LIST  )

set(PLATFORM_HDR_LIST       src/platforms/macos/platform_config.hpp)

source_group("Platform\\Core" FILES ${PLATFORM_SRC_CORE_LIST} ${PLATFORM_HDR_CORE_LIST})

source_group("Platform" FILES ${PLATFORM_HDR_LIST})

set(SRC_LIST ${SRC_LIST} ${PLATFORM_SRC_CORE_LIST})
set(HDR_LIST ${HDR_LIST} ${PLATFORM_HDR_CORE_LIST} ${PLATFORM_HDR_LIST})
set(LIB_LIST ${LIB_LIST} ${PLATFORM_LIB_CORE_LIST})
