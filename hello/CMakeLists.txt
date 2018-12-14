#include_directories(${PROJECT_SOURCE_DIR})
#include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/hello ${PROJECT_SOURCE_DIR}/include )
link_directories(${PROJECT_BINARY_DIR}/lib)


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

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

set(IMPORTLIB  ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY} 
  pthread  m qtutils  
)
#secp256k1

file(GLOB HELLO_HEADERS "*.h" )

set(HELLO_SRC  PoW.c c_hmac_md5.c   c_aes128.c c_camellia128.c  c_crc32.c c_gost.c c_ripemd160.c c_sha1.c c_sha3_256.c c_skein512_256.c c_blake2s256.c c_des.c c_haval5_256.c c_rc4.c c_sha256.c 
c_sha512.c c_whirlpool.c jtr_gost.c jtr_skein.c jtr_crc32.c jtr_haval.c blake2s.c keccak1600.c my_time.c common.c oneWayFunction.c main.c)
#file(GLOB_RECURSE WALLET_SRC ${PROJECT_SOURCE_DIR}/str2hex/*.cpp)

add_executable(hello ${HELLO_SRC} ${HELLO_HEADERS})
target_link_libraries(hello ${IMPORTLIB})

