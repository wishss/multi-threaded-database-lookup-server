# multi-threaded-database-lookup-server
### 멀티스레드 데이터베이스 조회 서버
+ 여러 클라이언트가 동시에 데이터베이스 조회를 요청할 수 있는 서버
+ 각 클라이언트는 조회 조건을 서버에 전송하고, 서버는 해당 조건에 맞는 데이터를 반환

## 개발 환경 구축
+ 환경 : Windows 11, WSL
+ IDE : Visual Stdio 2019
+ DB : SQLite
+ C++

### WSL 설정
Linux용 Windows 하위 시스템 체크 및 재부팅 후, 다음 명령어를 관리자 권한으로 실행하십시오:

    dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
    dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
    https://wslstorestorage.blob.core.windows.net/wslblob/wsl_update_x64.msi
    wsl --set-default-version 2

### Ubuntu ssh 서버 실행
    sudo apt install openssh-server
    sudo service ssh start

### Ubuntu 개발 도구 설치
    sudo apt update
    sudo apt install build-essential gdb cmake
    sudo apt install sqlite3

### Visual Stdio 2019 에서 WSL 연결 설정
1. 프로젝트 생성
2. 도구 -> 옵션 -> 플랫폼 간 -> 연결 관리자 -> 원격시스템 추가
3. 디버그 대상 .exe로 변경

## 프로젝트 빌드/테스트/실행



## 전체 아키텍처 구성도
![image](https://github.com/user-attachments/assets/463169cf-eca8-48fa-b45e-5a6016208be2)




## REST API 요청/응답 명세




## 사용한 라이브러리




## 요구사항 불만족 항목


