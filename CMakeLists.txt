add_executable(ipc_test main.cpp)
target_link_libraries(ipc_test pthread)
add_executable(utest_ptr SmartPtr.cpp)
target_link_libraries(utest_ptr gtest gmock)
