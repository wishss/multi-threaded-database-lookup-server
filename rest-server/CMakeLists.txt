# CMakeList.txt: rest-server에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
cmake_minimum_required (VERSION 3.8)

# 이 프로젝트의 실행 파일에 소스를 추가합니다.
add_executable (rest-server 
    "main.cpp" 
    "ListenerMgmt.cpp" 
    "ListenerMgmt.h" 
    "SystemStateMgmt.cpp" 
    "SystemStateMgmt.h"
    )

# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.

# 경로를 WSL의 파일 시스템으로 설정
set(INCLUDE_DIR "/usr/include")
set(LIB_DIR "/usr/lib/x86_64-linux-gnu")

# 헤더 파일 경로 추가
include_directories(${INCLUDE_DIR})

# 라이브러리 경로 추가
link_directories(${LIB_DIR})

# 라이브러리 찾기
find_package(cpprestsdk REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(OpenSSL REQUIRED)

# 라이브러리 링크
target_link_libraries(rest-server PRIVATE 
    cpprestsdk::cpprest 
    OpenSSL::SSL 
    OpenSSL::Crypto 
    SQLite::SQLite3
)