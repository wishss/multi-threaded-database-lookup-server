#include "ListenerMgmt.h"

ListenerMgmt::ListenerMgmt(const std::string& uri)
    : listener_(uri)
{
    listener_.support(methods::GET, std::bind(&ListenerMgmt::response, this, std::placeholders::_1));
}

ListenerMgmt::~ListenerMgmt()
{

}

void ListenerMgmt::startMgmt() {
    if (!db_mgmt_.open("state.db")) { throw std::runtime_error("Failed to open database."); }

    try {
        listener_
            .open()
            .then([this]() { std::cout << "Starting to listen at: " << listener_.uri().to_string() << std::endl; })
            .wait();

        // mgmt가 종료될 때까지 대기
        std::string line;
        std::cout << "Press Enter to stop the server..." << std::endl;
        std::getline(std::cin, line);

        // mgmt 종료
        stopMgmt();
        std::cout << "Server stopped." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
}

void ListenerMgmt::stopMgmt() {
    if (!stop_requested_.load()) {
        stop_requested_.store(true);
        listener_.close().wait();
        db_mgmt_.close();
    }
}

void ListenerMgmt::response(http_request request) {
    request
        .extract_json()
        .then([=](json::value request_data) {
        // URL 에서 데이터 추출
        auto path = uri::split_path(uri::decode(request.request_uri().path()));

        if (path.size() != 4 || path[0] != "api" || path[1] != "systemstate") {
            return request.reply(status_codes::BadRequest, U("Invalid URL format\n"));
        }

        std::string start_date = path[2];
        std::string end_date = path[3];

        // 클라이언트에 응답 반환
        return request.reply(status_codes::OK, db_mgmt_.getDataByDateRange(start_date, end_date));
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
        .wait();
}