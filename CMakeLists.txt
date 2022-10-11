cmake_minimum_required(VERSION 3.16)
project(jibblesbot VERSION 1.6 DESCRIPTION "Skiing Discord Bot")

add_subdirectory(libs/DPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(jibblesbot src/main.cpp src/VERSION.h src/HelpCommand.cpp src/HelpCommand.h src/updateCommand.cpp src/updateCommand.h src/buyCommand.cpp src/buyCommand.h src/DatabaseUtils.cpp src/DatabaseUtils.h)

target_link_libraries(jibblesbot dpp mariadb)

target_include_directories(${PROJECT_NAME} PRIVATE
        libs/DPP/include
        )
