#include_directories(${PROJECT_SOURCE_DIR})
#include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/libdbadapter ${PROJECT_SOURCE_DIR}/libwallet  ${PROJECT_SOURCE_DIR}/ulordcenter )
#include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/libdbadapter  ${PROJECT_SOURCE_DIR}/ulordcenter  /usr/local/include)
link_directories(${PROJECT_BINARY_DIR}/lib  /usr/local/lib)


#ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
message(STATUS "PROJECT_LIB_DIR="  ${PROJECT_BINARY_DIR}/lib )


find_package(OpenSSL)
if(NOT OPENSSL_FOUND)
  message(FATAL_ERROR "OpenSSL not found!")
endif(NOT OPENSSL_FOUND)

#muduo_protobuf_codec
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(IMPORTLIB  wallet  secp256k1 ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY}   ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY} ${Boost_LIBRARIES} 
  boost_system boost_filesystem boost_program_options boost_thread boost_log_setup boost_serialization
  pthread  db_cxx-5.3   muduo_base_cpp11 muduo_net_cpp11  dbadapter  mysqlcppconn mysqlcppconn-static  mysqlclient
  m  
)
#mysqlclient

file(GLOB ULORD_HEADERS "*.h" )
file(GLOB ULORD_SRC "*.cpp"  "*.cc")
add_executable(ulordcenter ${ULORD_SRC} ${ULORD_HEADERS})
target_link_libraries(ulordcenter ${IMPORTLIB})

