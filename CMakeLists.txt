cmake_minimum_required(VERSION 3.9)

project(multilingual VERSION 1.0.0 DESCRIPTION "libmultilingual")

include(GNUInstallDirs)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(multilingual SHARED src/Translator.cpp src/TranslatorTypes.cpp)

set_target_properties(multilingual PROPERTIES
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER include/multilingual.h)

configure_file(multilingual.pc.in multilingual.pc @ONLY)

target_include_directories(multilingual PRIVATE include)

install(TARGETS multilingual
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${CMAKE_BINARY_DIR}/multilingual.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
