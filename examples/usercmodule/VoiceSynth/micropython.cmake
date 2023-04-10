# Create an INTERFACE library for our C module.
add_library(usermod_cexample INTERFACE)


# Add our source files to the lib
target_sources(usermod_cexample INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/aquestalk.c
)
#target_compile_options(micropython INTERFACE "-Wl,--whole-archive  /home/hal/esp/micropython/examples/usercmodule/aquestalk/src/esp32/libaquestalk.a -Wl,--no-whole-archive")
#target_compile_options(usermod_cexample INTERFACE "-Wall -l/home/hal/esp/micropython/examples/usercmodule/aquestalk/src/esp32/libaquestalk.a")
# Add the current directory as an include directory.
target_include_directories(usermod_cexample INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_cexample)
#target_link_libraries(/home/hal/esp/micropython/examples/usercmodule/aquestalk/src/esp32/libaquestalk.a)
link_libraries(/home/hal/esp/micropython/examples/usercmodule/aquestalk/src/esp32/libaquestalk.a)
#link_directories(usermod PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/esp32)
#target_link_libraries(usermod INTERFACE
#    ${CMAKE_CURRENT_LIST_DIR}/src/esp32/libaquestalk.a
#)
#link_directories(usermod_cexample INTERFACE
#    ${CMAKE_CURRENT_LIST_DIR}/src/esp32
#)
#target_link_libraries(usermod_cexample aquestalk)


