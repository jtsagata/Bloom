set(BSHELL_SOURCES
        utils.h utils.cpp
        Mixer.h
        repl.h repl.cpp
        repl_hooks.h repl_hooks.cpp
        BloomFilter.cpp BloomFilter.h
        pasaTempo.h
        bshell.cpp)

add_executable(bshell ${BSHELL_SOURCES})
target_link_libraries(bshell  replxx ${Boost_LIBRARIES})

#Copy Data files
add_custom_command( TARGET bshell
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copying unit test data.."
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data
)



