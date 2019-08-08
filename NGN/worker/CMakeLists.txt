message(STATUS "This is CURRENT dir " ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(srv_core)
#add_subdirectory(srvapi)
add_subdirectory(system_worker)
#add_subdirectory(iface_worker)
add_subdirectory(om_worker)

