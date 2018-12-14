#include_directories(${PROJECT_SOURCE_DIR})
#include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}   ${PROJECT_SOURCE_DIR}/libdbadapter )
link_directories(${PROJECT_BINARY_DIR}/lib  /usr/local/lib)


#ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
message(STATUS "PROJECT_LIB_DIR="  ${PROJECT_BINARY_DIR}/lib )

set(IMPORTLIB  ${Boost_LIBRARIES} 
  boost_system boost_filesystem boost_program_options boost_thread 
  pthread  
)

file(GLOB DBADAPTER_HEADERS "*.h" )
file(GLOB DBADAPTER_SRC "*.cpp" "*.cc" )

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_library(libdbadapter ${DBADAPTER_SRC} ${DBADAPTER_HEADERS})
set_target_properties(libdbadapter PROPERTIES OUTPUT_NAME "dbadapter")

