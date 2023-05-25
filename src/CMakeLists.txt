project(webserv_lib)

add_library(${PROJECT_NAME} STATIC
  utils/Logger.cpp
  utils/string.cpp
  utils/FileSystem.cpp
)

add_library(42::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/inc)