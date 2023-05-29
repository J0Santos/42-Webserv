project(webserv_lib)

add_library(${PROJECT_NAME} STATIC
  utils/ft_exceptions.cpp
  utils/ft_filesystem.cpp
  utils/ft_string.cpp
  utils/Host.cpp
  utils/Logger.cpp
)

add_library(42::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/inc)