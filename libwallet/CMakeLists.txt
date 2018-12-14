#include_directories(${PROJECT_SOURCE_DIR})
#include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}   ${PROJECT_SOURCE_DIR}/libwallet ${PROJECT_SOURCE_DIR}/include )
link_directories(${PROJECT_BINARY_DIR}/lib)


#ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
message(STATUS "PROJECT_LIB_DIR="  ${PROJECT_BINARY_DIR}/lib )


#FIND_PACKAGE(Boost COMPONENTS log boost_system boost_filesystem boost_program_options boost_thread  REQUIRED)
#if(NOT Boost_FOUND)
#  message(FATAL_ERROR "Boost log  not found!")
#endif(NOT Boost_FOUND)

message(STATUS "secp256k1_LIB_DIR="  ${secp256k1_LIBRARIES})
message(STATUS "boost_LIB_DIR="  ${Boost_LIBRARIES})

#file(GLOB HEADERS "*.h")
#add_library(ethcore ${SRC_LIST} ${HEADERS})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#add_executable(testma ${APP_SRC})

set(IMPORTLIB  ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY}   ${Boost_LIBRARIES}
  boost_system boost_filesystem boost_program_options boost_thread secp256k1  
  boost_log_setup db_cxx-5.3  pthread  m
  
)

file(GLOB WALLET_HEADERS "*.h" "crypto/*.h" "script/*.h" "policy/*.h" "support/*.h" "primitives/*.h" )
file(GLOB WALLET_SRC "*.cpp" "crypto/*.cpp" "script/*.cpp" "policy/*.cpp" "support/*.cpp" "primitives/*.cpp")
#file(GLOB_RECURSE WALLET_SRC ${PROJECT_SOURCE_DIR}/mysign/*.cpp)
#add_executable(mysign ${MAIN_SRC_LIST}  ${MAIN_HEADERS})
#add_executable(libwallet ${WALLET_SRC} ${WALLET_HEADERS})
#target_link_libraries(libwallet ${IMPORTLIB})
add_library(libmywallet ${WALLET_SRC} ${WALLET_HEADERS})
set_target_properties(libmywallet  PROPERTIES OUTPUT_NAME "wallet")


