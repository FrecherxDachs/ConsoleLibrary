cmake_minimum_required(VERSION 2.8)

project(breakout)

SET(CMAKE_CXX_FLAGS "-std=c++0x")

set(SOURCE
  main.cpp
  Breakout.cpp
  ../ConsoleLibrary/Menu.cpp
  ../ConsoleLibrary/LinuxConsole.cpp
)

set(HEADERS
  Breakout.h
  ../ConsoleLibrary/Menu.h
  ../ConsoleLibrary/Console.h
)

add_executable(breakout ${SOURCE} ${HEADERS})

target_link_libraries(breakout "ncurses")

install(TARGETS breakout RUNTIME DESTINATION bin)
