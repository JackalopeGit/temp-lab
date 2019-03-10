cmake_minimum_required(VERSION 3.5)

project("Lab1")

add_compile_options(
#important
    -std=c++17 -fno-exceptions -fno-rtti
)

configure_file(grammar               ${CMAKE_CURRENT_BINARY_DIR}/grammar               COPYONLY)
configure_file(useless_language_file ${CMAKE_CURRENT_BINARY_DIR}/useless_language_file COPYONLY)
configure_file(basic_reals           ${CMAKE_CURRENT_BINARY_DIR}/basic_reals           COPYONLY)

set(mtl_SRC

    templates/array/resizable.h
    templates/array/smart.h
)

set(tools_SRC
    tools/exception.h
    tools/exception.cpp
    tools/file.h
    tools/string.h
    tools/io.h
)

add_executable(LAB1.1

    ${mtl_SRC}
    ${tools_SRC}

    grammar

    misc.h
    main1.cpp
)

add_executable(LAB1.2

    ${mtl_SRC}
    ${tools_SRC}

    useless_language_file

    misc.h
    main2.cpp
)
add_executable(LAB1.3

    ${mtl_SRC}
    ${tools_SRC}

    basic_reals

    misc.h
    main3.cpp
)
