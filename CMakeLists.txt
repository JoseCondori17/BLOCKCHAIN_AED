cmake_minimum_required(VERSION 3.25)
project(BLOCKCHAIN_AED)
# Establecer la ruta de instalación de OpenSSL
set(OPENSSL_ROOT_DIR C:/Program Files/OpenSSL-Win64/include)
# Buscar y configurar OpenSSL
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 14)

# Crear el objetivo y vincular con OpenSSL
add_executable(BLOCKCHAIN_AED
        main.cpp Block.h
        Blockchain.h
        Transaction.h
        ForwardList.h
        mergesort.h
        CircularArray.h
        HeapMinMax.h
        )
target_link_libraries(BLOCKCHAIN_AED PUBLIC OpenSSL::Crypto)
