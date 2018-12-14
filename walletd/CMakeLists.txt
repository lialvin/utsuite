#include_directories(${PROJECT_SOURCE_DIR})
#include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/libwallet  ${PROJECT_SOURCE_DIR}/walletd )
link_directories(${PROJECT_BINARY_DIR}/lib)

#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
#ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
message(STATUS "PROJECT_LIB_DIR="  ${PROJECT_BINARY_DIR}/lib )

find_package(OpenSSL)
if(NOT OPENSSL_FOUND)
  message(FATAL_ERROR "OpenSSL not found!")
endif(NOT OPENSSL_FOUND)

#FIND_PACKAGE(Boost COMPONENTS log boost_system boost_filesystem boost_program_options boost_thread  REQUIRED)
#if(NOT Boost_FOUND)
#  message(FATAL_ERROR "Boost log  not found!")
#endif(NOT Boost_FOUND)


message(STATUS "boost_LIB_DIR="  ${Boost_LIBRARIES})

#file(GLOB HEADERS "*.h")
#add_library(ethcore ${SRC_LIST} ${HEADERS})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#add_executable(testma ${APP_SRC})

set(IMPORTLIB   wallet secp256k1  ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY}  ${secp256k1_LIBRARIES} ${Boost_LIBRARIES}
  boost_system boost_filesystem boost_program_options boost_thread boost_chrono
  boost_log_setup db_cxx  pthread  m
  qtutils 
)

file(GLOB WALLETD_HEADERS "*.h" )
file(GLOB WALLETD_SRC "*.cpp" )
#file(GLOB_RECURSE WALLET_SRC ${PROJECT_SOURCE_DIR}/walletd/*.cpp)
#add_executable(walletd ${MAIN_SRC_LIST}  ${MAIN_HEADERS})
add_executable(walletd ${WALLETD_SRC} ${WALLETD_HEADERS})
target_link_libraries(walletd ${IMPORTLIB})

