set(UI_SOURCES
    snapCasterUi.cpp
    queue.cpp
    daemon.cpp)

set(UI_LIBRARIES 
	${CMAKE_THREAD_LIBS_INIT}
	${JSONRPCPP_LIBRARIES})

set(UI_INCLUDE
    ${CMAKE_SOURCE_DIR}/src
	${JSONRPCPP_INCLUDE_DIRS})

# Avahi
list(APPEND UI_SOURCES browseZeroConf/browseAvahi.cpp)
list(APPEND UI_LIBRARIES ${AVAHI_LIBRARIES})
list(APPEND UI_INCLUDE ${AVAHI_INCLUDE_DIRS})

add_definitions(-DHAS_DAEMON)

include_directories(${UI_INCLUDE})
add_executable(snapclient ${UI_SOURCES})
target_link_libraries(snapclient ${UI_LIBRARIES})

install(TARGETS snapcaster-ui COMPONENT snapcaster-ui DESTINATION "${CMAKE_INSTALL_BINDIR}")
