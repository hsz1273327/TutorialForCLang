# CMake generated Testfile for 
# Source directory: /Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/create_cmdline_tool/sqrt_opt
# Build directory: /Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/create_cmdline_tool/sqrt_opt
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(TEST_VERSION "/Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/create_cmdline_tool/sqrt_opt/build/bin/sqrt_opt" "-v")
set_tests_properties(TEST_VERSION PROPERTIES  PASS_REGULAR_EXPRESSION "v: 0.1.0" _BACKTRACE_TRIPLES "/Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/create_cmdline_tool/sqrt_opt/CMakeLists.txt;11;add_test;/Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/create_cmdline_tool/sqrt_opt/CMakeLists.txt;0;")
add_test(TEST_36 "/Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/create_cmdline_tool/sqrt_opt/build/bin/sqrt_opt" "-s36")
set_tests_properties(TEST_36 PROPERTIES  PASS_REGULAR_EXPRESSION "6.0" _BACKTRACE_TRIPLES "/Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/create_cmdline_tool/sqrt_opt/CMakeLists.txt;15;add_test;/Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/create_cmdline_tool/sqrt_opt/CMakeLists.txt;0;")
add_test(TEST_2 "/Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/create_cmdline_tool/sqrt_opt/build/bin/sqrt_opt" "-s2")
set_tests_properties(TEST_2 PROPERTIES  PASS_REGULAR_EXPRESSION "1.41" _BACKTRACE_TRIPLES "/Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/create_cmdline_tool/sqrt_opt/CMakeLists.txt;18;add_test;/Users/huangsizhe/Workspace/Documents/TutorialForCLang/c-tool-chain/code/create_cmdline_tool/sqrt_opt/CMakeLists.txt;0;")
