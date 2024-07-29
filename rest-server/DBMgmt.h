#pragma once

#include <iostream>
#include <sqlite3.h>
#include <string>
#include <cpprest/json.h>

using namespace web;

class DBMgmt
{
public:
	DBMgmt();
	~DBMgmt();

	bool open(const std::string& dbPath);
	void close();
	bool insertData(const std::string& date, const std::string& time, double cpu, int memory, long disk);
	json::value getDataByDateRange(const std::string& startDate, const std::string& endDate);
private:
	sqlite3* db_;
};