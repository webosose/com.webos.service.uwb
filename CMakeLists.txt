cmake_minimum_required(VERSION 2.8.7)
project(webos-uwb-service)

include(webOS/webOS)
webos_modules_init(1 0 0 QUALIFIER RC4)
webos_component(1 0 0)

include(FindPkgConfig)

pkg_check_modules(GLIB2 REQUIRED glib-2.0)
add_definitions(${GLIB2_CFLAGS})

pkg_check_modules(LUNASERVICE2 REQUIRED luna-service2)
include_directories(${LUNASERVICE2_INCLUDE_DIRS})
add_definitions(${LUNASERVICE2_CFLAGS})

pkg_check_modules(PBNJSON_CXX REQUIRED pbnjson_cpp)
add_definitions(${PBNJSON_CXX_CFLAGS})

pkg_check_modules(PMLOGLIB REQUIRED PmLogLib)
add_definitions(${PMLOGLIB_CFLAGS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(UWB_SERVICE_NAME webos-uwb-service)

file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    ${GLIB2_LDFLAGS} ${LUNASERVICE2_LDFLAGS} ${PBNJSON_CXX_LDFLAGS}
    ${PMLOGLIB_LDFLAGS}
    luna-service2++ pthread)

webos_build_daemon(NAME ${PROJECT_NAME})
webos_build_system_bus_files()
webos_build_configured_file(files/systemd/com.webos.service.uwb.service SYSCONFDIR systemd/system)
