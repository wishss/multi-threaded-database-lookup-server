#pragma once

#include <cpprest/http_listener.h>
#include <cpprest/uri.h>
#include <cpprest/uri_builder.h>
#include <cpprest/json.h>
#include <iostream>
#include <utility>

#include "DBMgmt.h"

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

class ListenerMgmt {
public:
	ListenerMgmt(const std::string& uri);
	~ListenerMgmt();

	void startMgmt();
	void stopMgmt();

private:
	void response(http_request request);

private:
	http_listener listener_;
	std::atomic<bool> stop_requested_;
	DBMgmt db_mgmt_;
};