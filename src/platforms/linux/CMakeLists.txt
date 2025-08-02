set(SRC_CORE_LIST              src/platforms/stub/core/assertion_stub.cpp)
set(HDR_CORE_LIST              )
set(LIB_CORE_LIST              )

source_group("Platform\\Core" FILES ${SRC_CORE_LIST} ${HDR_CORE_LIST})
set(SRC_LIST ${SRC_LIST} ${SRC_CORE_LIST})
set(HDR_LIST ${HDR_LIST} ${HDR_CORE_LIST})
set(LIB_LIST ${LIB_LIST} ${LIB_CORE_LIST})
