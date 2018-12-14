include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)
link_directories(${PROJECT_BINARY_DIR}/lib)

#ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
set(MAIN_SRC_LIST
 mystr.cpp
 btstat.cpp
 readcfg.cpp
 testmain.cpp
  )

set(MAIN_HEADERS
  mystr.h 
  btstat.h
  readcfg.h
  testmain.h
)

#file(GLOB HEADERS "*.h")
#add_library(ethcore ${SRC_LIST} ${HEADERS})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#add_executable(testma ${APP_SRC})

set(IMPORTLIB 
hell mysqlclient pthread  boost_system boost_filesystem boost_program_options boost_thread boost_thread  boost_log  m
boost_log_setup 
)

add_executable(tblmgrt ${MAIN_SRC_LIST}  ${MAIN_HEADERS})
target_link_libraries(tblmgrt ${IMPORTLIB})


