add_library(threadsync thread_controller.cpp thread_sync.cpp)

set(LIBS thread_sync oclcontroller gtest boost_filesystem boost_system boost_options pthread)

add_executable(thread_sync_test thread_sync.cpp)
target_link_libraries(thread_sync_test gtest_main ${LIBS})

add_executable(thread_controller_test thread_controller.cpp)
target_link_libraries(thread_controller_test gtest_main ${LIBS})

include(TARGETS thread_sync  DESTINATION  lib/threadsyncocl)
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR"/.hpp}")

add_subdirectory(tsyncocl_demo)
