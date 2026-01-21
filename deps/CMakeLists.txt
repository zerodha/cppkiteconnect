cmake_minimum_required(VERSION 3.10)
project(CPPKiteConnect)

function(downloadFile URL destination hashType hash)
    if(NOT EXISTS ${destination})
        message("Downloading ${destination} ..")
        file(DOWNLOAD ${URL}
            ${destination}
            TIMEOUT 60 # seconds
            EXPECTED_HASH ${hashType}=${hash}
        )
    else()
        message("${destination} already exists")
    endif()
endfunction(downloadFile)

#Download GTest
downloadFile(   "https://github.com/google/googletest/archive/release-1.10.0.tar.gz"
                "${CMAKE_SOURCE_DIR}/deps/googletest-1.10.0.tar.gz"
                SHA256
                9dc9157a9a1551ec7a7e43daea9a694a0bb5fb8bec81235d8a1e6ef64c716dcb
            )

#Download openssl
downloadFile(   "https://github.com/openssl/openssl/archive/OpenSSL_1_1_1i.tar.gz"
                "${CMAKE_SOURCE_DIR}/deps/OpenSSL_1_1_1i.tar.gz"
                SHA256
                728d537d466a062e94705d44ee8c13c7b82d1b66f59f4e948e0cbf1cd7c461d8
            )

# Download zlib
downloadFile(   "https://github.com/madler/zlib/archive/v1.2.11.tar.gz"
                "${CMAKE_SOURCE_DIR}/deps/zlib-v1.2.11.tar.gz"
                SHA256
                629380c90a77b964d896ed37163f5c3a34f6e6d897311f1df2a7016355c45eff
            )
