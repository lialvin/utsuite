#include_directories(${PROJECT_SOURCE_DIR})
#include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include   ${PROJECT_SOURCE_DIR}/unode )
link_directories(${PROJECT_BINARY_DIR}/lib  /usr/local/lib)


#ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
message(STATUS "PROJECT_LIB_DIR="  ${PROJECT_BINARY_DIR}/lib )


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(IMPORTLIB   ${Boost_LIBRARIES} 
  boost_system boost_filesystem boost_program_options boost_thread boost_log_setup
  pthread  muduo_base muduo_net muduo_protobuf_codec  boost_serialization
  m 
)

file(GLOB ULORD_HEADERS "*.h" )
file(GLOB ULORD_SRC "*.cpp" )
add_executable(unode ${ULORD_SRC} ${ULORD_HEADERS})
target_link_libraries(unode ${IMPORTLIB})

