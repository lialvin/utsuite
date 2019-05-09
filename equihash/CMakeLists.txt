#include_directories(${PROJECT_SOURCE_DIR})
#include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/equihash  ${PROJECT_SOURCE_DIR}/libsodium-1.0.15/src/libsodium/include  ${PROJECT_SOURCE_DIR}/include )
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
  pthread  m qtutils  sodium
)
#secp256k1

#file(GLOB EQUIHASH_HEADERS "*.h" )

set(EQUIHASH_HEADERS chacha20.h common.h  equihash.h  hmac_sha256.h hmac_sha512.h scrypt.h ripemd160.h sha1.h sha256.h sha512.h )  

set(EQUIHASH_SRC chacha20.cpp equihash.cpp equihash.tcc hmac_sha256.cpp hmac_sha512.cpp hmac_sha512.cpp scrypt.cpp ripemd160.cpp  sha1.cpp  sha256.cpp sha512.cpp main.cpp)  

#file(GLOB_RECURSE WALLET_SRC ${PROJECT_SOURCE_DIR}/str2hex/*.cpp)

add_executable(equihash ${EQUIHASH_SRC} ${EQUIHASH_HEADERS})
target_link_libraries(equihash ${IMPORTLIB})

