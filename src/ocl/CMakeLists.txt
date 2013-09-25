add_library(ocl_controller ocl_controller.cpp)

set(LIBS ocl_controller gtest boost_filesystem boost_system pthread)

add_executable(ocl_controller_test ocl_controller_test.cpp)
target_link_libraries(ocl_controller_test gtest_main ${LIBS})

install(TARGETS ocl_controller  DESTINATION lib/threadsyncocl)
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR/.hpp}")
install(FILES  ${HEADER_FILES} DESTINATION include/ocl)
