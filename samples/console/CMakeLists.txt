set(SRC_LIST console_sample_app.cpp)
set(HDR_LIST )
set(INL_LIST )
set(RES_LIST )
set(LIB_LIST toygine)
set(BINARY_NAME console)

set(APP_VERSION_MAJOR       1)
set(APP_VERSION_MINOR       0)
set(APP_VERSION_MAINTENANCE 0)

console_application(${BINARY_NAME} ${SRC_LIST} ${HDR_LIST} ${INL_LIST} ${RES_LIST} ${LIB_LIST})
