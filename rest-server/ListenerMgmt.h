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

	void startMgmt();

private:
	http_listener listener_;
	void response(http_request request);
};