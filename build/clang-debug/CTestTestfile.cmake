# CMake generated Testfile for 
# Source directory: E:/Code/C_code/libraries/GUI
# Build directory: E:/Code/C_code/libraries/GUI/build/clang-debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[GUITests]=] "E:/Code/C_code/libraries/GUI/build/clang-debug/GUITests.exe")
set_tests_properties([=[GUITests]=] PROPERTIES  _BACKTRACE_TRIPLES "E:/Code/C_code/libraries/GUI/CMakeLists.txt;118;add_test;E:/Code/C_code/libraries/GUI/CMakeLists.txt;0;")
subdirs("freetype")
subdirs("harfbuzz")
subdirs("GraphicsWrapper")
