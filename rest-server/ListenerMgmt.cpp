// rest-server.cpp : 애플리케이션의 진입점을 정의합니다.
//

#include "ListenerMgmt.h"

ListenerMgmt::ListenerMgmt(const std::string& uri)
    : listener(uri)
{
    listener.support(methods::GET, std::bind(&ListenerMgmt::Response, this, std::placeholders::_1));
}

void ListenerMgmt::MgmtStart() {
    try {
        listener
            .open()
            .then([this]() { std::cout << "Starting to listen at: " << listener.uri().to_string() << std::endl; })
            .wait();

        // 서버가 종료될 때까지 대기
        std::string line;
        std::getline(std::cin, line);
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
}

void ListenerMgmt::Response(http_request request) {
    // JSON 응답 생성
    json::value response_data;
    response_data[U("date")] = json::value::string(U("2024-07-25"));
    response_data[U("cpu")] = json::value::number(85.6);
    response_data[U("memory")] = json::value::number(4096.0);
    response_data[U("disk")] = json::value::number(2048.0);

    // 응답을 클라이언트에 반환
    request.reply(status_codes::OK, response_data);
}