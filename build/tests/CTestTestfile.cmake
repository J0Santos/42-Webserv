# CMake generated Testfile for 
# Source directory: /home/mlancac/.Cursus/lv5/my-webserv/tests
# Build directory: /home/mlancac/.Cursus/lv5/my-webserv/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(webserv_gtest "webserv_utests")
set_tests_properties(webserv_gtest PROPERTIES  _BACKTRACE_TRIPLES "/home/mlancac/.Cursus/lv5/my-webserv/tests/CMakeLists.txt;27;add_test;/home/mlancac/.Cursus/lv5/my-webserv/tests/CMakeLists.txt;0;")
subdirs("../_deps/googletest-build")
