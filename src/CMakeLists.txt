project(webserv_lib)

add_library(${PROJECT_NAME} STATIC
  cgi/Envp.cpp
  http/methods.cpp
  http/Request.cpp
  http/Uri.cpp
  utils/ft_exceptions.cpp
  utils/ft_filesystem.cpp
  utils/ft_string.cpp
  utils/Host.cpp
  utils/Logger.cpp
  utils/Route.cpp
)

add_library(42::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/inc)