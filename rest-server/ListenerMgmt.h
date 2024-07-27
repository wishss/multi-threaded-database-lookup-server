// rest-server.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.

#pragma once

#include <cpprest/http_listener.h>
#include <cpprest/uri.h>
#include <cpprest/uri_builder.h>
#include <cpprest/json.h>
#include <iostream>
#include <utility>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class ListenerMgmt {
public:
	ListenerMgmt(const std::string& uri);

	void MgmtStart();

private:
	http_listener listener;
	void Response(http_request request);
};