project(webserv_lib)

add_library(${PROJECT_NAME} STATIC
  cgi/Argc.cpp
  cgi/Envp.cpp
  cgi/CgiHandler.cpp
  config/directives.cpp
  config/Line.cpp
  config/Options.cpp
  config/blocks/block.cpp
  config/blocks/route.cpp
  config/Parser.cpp
  http/ErrorCode.cpp
  http/methods.cpp
  http/Request.cpp
  http/Response.cpp
  http/Uri.cpp
  utils/ft_array.cpp
  utils/ft_exceptions.cpp
  utils/ft_filesystem.cpp
  utils/ft_string.cpp
  utils/Host.cpp
  utils/Logger.cpp
  utils/Route.cpp
)

add_library(42::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/inc)