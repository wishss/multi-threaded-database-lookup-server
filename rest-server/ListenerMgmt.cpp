#include "ListenerMgmt.h"

ListenerMgmt::ListenerMgmt(const std::string& uri)
    : listener_(uri)
{
    listener_.support(methods::GET, std::bind(&ListenerMgmt::response, this, std::placeholders::_1));
}

void ListenerMgmt::startMgmt() {
    Sleep(5000);
    try {
        listener_
            .open()
            .then([this]() { std::cout << "Starting to listen at: " << listener_.uri().to_string() << std::endl; })
            .wait();

        // 서버가 종료될 때까지 대기
        std::string line;
        std::getline(std::cin, line);
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
}

void ListenerMgmt::response(http_request request) {
    request
        .extract_json()
        .then([=](json::value request_data) {
        /* db조회 기능 필요 */

        // 요청을 처리하고 응답 생성
        json::value response_data;
        response_data[U("date")] = json::value::string(U("2024-07-25"));
        response_data[U("cpu")] = json::value::number(85.6);
        response_data[U("memory")] = json::value::number(4096.0);
        response_data[U("disk")] = json::value::number(2048.0);

        // 클라이언트에 응답 반환
        return request.reply(status_codes::OK, response_data);
        })
        .then([](pplx::task<void> previous_task) {
                try {
                    // 응답이 정상적으로 전송되었는지 확인
                    previous_task.get();
                }
                catch (const std::exception& e) {
                    std::cerr << "Failed to send response: " << e.what() << std::endl;
                }
            })
        .wait(); // 비동기 작업을 기다림 (필요시 사용)
}