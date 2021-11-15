cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)

# Explicitly list the test source code and headers. The Catch header-only unit
# test framework is stored in with the test source.

# TODO(jhpark): real test !!!
#set(LibSaS_TEST_SRC
#    test_cmake_demo.cpp
#)
#set(LibSaS_TEST_HEADER
#    catch.hpp
#)

#PREPEND(LibSaS_TEST_SRC)

# Make an executable target that depends on the test source code we specified
# above.
#add_executable(TestLibSaS ${LibSaS_TEST_SRC} ${LibSaS_TEST_HEADER})

# Enable testing via CTest
#enable_testing()
# Add our test as runnable via CTest
#add_test(NAME TestLibSaS  COMMAND TestLibSaS)

# Link our unit tests against the library we compiled
#target_link_libraries(TestLibSaS LibSaS)

# (jhpark): simple executable targets
add_executable(TestLibSaS_DAS lib_das_test.cc)
add_executable(TestLibSaS_NETWORK lib_network_test.cc)
add_executable(TestLibSaS_Multithread lib_multiclient_test.cc)

target_link_libraries(TestLibSaS_DAS LibSaS)
target_link_libraries(TestLibSaS_NETWORK LibSaS)
target_link_libraries(TestLibSaS_Multithread LibSaS pthread)
