#include "DBMgmt.h"

DBMgmt::DBMgmt(const std::string& dbPath)
    :dbPath_(dbPath), db_(nullptr)
{
}

DBMgmt::~DBMgmt()
{
}

bool DBMgmt::open() {
    int rc = sqlite3_open(dbPath_.c_str(), &db_);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }

    // WAL 방식으로 설정
    rc = sqlite3_exec(db_, "PRAGMA journal_mode = WAL;", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to set WAL mode: " << sqlite3_errmsg(db_) << std::endl;
        sqlite3_close(db_);
        db_ = nullptr;
        return false;
    }

    // 데이터 테이블 생성
    rc = sqlite3_exec(db_,
        "CREATE TABLE IF NOT EXISTS data (date TEXT, time TEXT, cpu REAL, memory INTEGER, disk INTEGER, PRIMARY KEY(date, time));",
        nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to create table: " << sqlite3_errmsg(db_) << std::endl;
        sqlite3_close(db_);
        db_ = nullptr;
        return false;
    }

    return true;
}

void DBMgmt::close() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

bool DBMgmt::insertData(const std::string& date, const std::string& time, double cpu, int memory, long disk) {
    std::stringstream ss;
    ss << "INSERT INTO data (date, time, cpu, memory, disk) "
        << "VALUES ('" << date << "', '" << time << "', "
        << cpu << ", " << memory << ", " << disk << ");";
    std::string query = ss.str();

    int rc = sqlite3_exec(db_, query.c_str(), nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to insert data: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }

    std::cout << "Inserted data for date " << date << std::endl;
    return true;
}

json::value DBMgmt::getDataByDateRange(const std::string& startDate, const std::string& endDate) {
    json::value select_data;
    
    std::stringstream ss;
    ss << "SELECT * FROM data WHERE date BETWEEN '" << startDate << "' AND '" << endDate << "';";

    std::string query = ss.str();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return select_data;
    }

    // 결과 읽기
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        
        select_data[U("date")] = json::value::string(U(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
        select_data[U("time")] = json::value::string(U(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
        select_data[U("cpu")] = json::value::number(sqlite3_column_double(stmt, 2));
        select_data[U("memory")] = json::value::number(sqlite3_column_int(stmt, 3));
        select_data[U("disk")] = json::value::number(sqlite3_column_int(stmt, 4));

        //test
        std::cout << "date : " << json::value::string(U(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))))
            << " time : " << json::value::string(U(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))))
            << " cpu : " << json::value::number(sqlite3_column_double(stmt, 2))
            << " memory : " << json::value::number(sqlite3_column_int(stmt, 3))
            << " disk : " << json::value::number(sqlite3_column_int(stmt, 4)) 
            << std::endl;
    }

    sqlite3_finalize(stmt);
    return select_data;
}