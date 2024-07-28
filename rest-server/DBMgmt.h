#pragma once

#include <iostream>
#include <sqlite3.h>
#include <string>
#include <cpprest/json.h>

using namespace web;

class DBMgmt
{
public:
	DBMgmt(const std::string& dbPath);
	~DBMgmt();

	bool open();
	void close();
	bool insertData(const std::string& date, const std::string& time, double cpu, int memory, long disk);
	json::value getDataByDateRange(const std::string& startDate, const std::string& endDate);
private:
	std::string dbPath_;
	sqlite3* db_;
};