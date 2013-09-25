add_library(thread_sync thread_sync.cpp)

set(LIBS thread_sync ocl_controller gtest boost_filesystem boost_system pthread)

add_executable(thread_sync_test thread_sync_test.cpp)
target_link_libraries(thread_sync_test gtest_main ${LIBS})

#add_executable(thread_controller_test thread_controller.cpp)
#target_link_libraries(thread_controller_test gtest_main ${LIBS})

install(TARGETS thread_sync DESTINATION lib/threadsyncocl)
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR/.hpp}")

add_subdirectory(tsyncocl_demo)
