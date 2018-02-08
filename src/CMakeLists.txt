set(UI_SOURCES
    snapcaster.cpp
    controller.cpp
    server-json-rpc.cpp
    ui-handler.cpp
    daemon.cpp)

set(UI_LIBRARIES 
	${CMAKE_THREAD_LIBS_INIT}
	${JSONRPCPP_LIBRARIES})

set(UI_INCLUDE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/externals/aixlog/include
	${JSONRPCPP_INCLUDE_DIRS})

# Avahi
list(APPEND UI_SOURCES browseZeroConf/browseAvahi.cpp)
list(APPEND UI_LIBRARIES ${AVAHI_LIBRARIES})
list(APPEND UI_INCLUDE ${AVAHI_INCLUDE_DIRS})

add_definitions(-DHAS_DAEMON)

include_directories(${UI_INCLUDE})
add_executable(snapcaster-ui ${UI_SOURCES})
target_link_libraries(snapcaster-ui ${UI_LIBRARIES})

install(TARGETS snapcaster-ui COMPONENT snapcaster DESTINATION "${CMAKE_INSTALL_BINDIR}")
