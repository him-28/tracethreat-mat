
# Define project

project ("syncocl")
cmake_minimum_required(VERSION 2.8) # Requires 2.8 

add_definitions("-g -Wall -Werror -O2")

set(CMAKE_INSTALL_PREFIX "/home/chatsiri/workspacecpp/techniquecpp/pthread_sync_ocl/build")

set(THIRD_PARTY_DIR "/home/chatsiri/workspacecpp/techniquecpp/pthread_sync_ocl/3rd")

set(BOOST_DIR "/home/chatsiri/workspacecpp/boost-truck")
set(GTEST_DIR "/home/chatsiri/workspacecpp/gtest-1.6.0")

include_directories(
	"${PROJECT_SOURCE_DIR}";
	"${PROJECT_BINARY_DIR}";
	"${BOOST_DIR}/";
	"${GTEST_DIR}/include";
)

#Set library path
link_directories(
	"${PROJECT_BINARY_DIR}/src/thread";
	"${PROJECT_BINARY_DIR}/src/ocl";
	"${BOOST_DIR}/stage/lib";
	"${GTEST_DIR}/lib";
)

#add package
add_subdirectory(src/thread)
add_subdirectory(src/ocl)
