include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)
link_directories( /usr/local/boost_1_67_0/stage/lib ${PROJECT_BINARY_DIR}/lib)

ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
set(MAIN_SRC_LIST
  stat_rec.cpp 
  )

set(MAIN_HEADERS
)

#file(GLOB HEADERS "*.h")
#add_library(ethcore ${SRC_LIST} ${HEADERS})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#add_executable(testma ${APP_SRC})

set(IMPORTLIB 
   boost_system 
)

add_executable(stat_rec  stat_rec.cpp)
target_link_libraries(stat_rec  ${IMPORTLIB})

#add_executable(tcp_client  blocking_tcp_echo_client.cpp)
#target_link_libraries(tcp_client  ${IMPORTLIB})

