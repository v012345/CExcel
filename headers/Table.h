#pragma once
#include <OpenXLSX.hpp>
#include <string>
#include <map>
#include <vector>
#include <set>
class Table
{
private:
    std::string name;
    // xlsx 文件

    OpenXLSX::XLDocument table;
    // xlsx 文件左下角的分页
    OpenXLSX::XLWorksheet sheet;
    int real_row_count;
    // 表头
    std::map<std::string, std::uint32_t> head;
    //数据
    std::map<std::string, std::vector<OpenXLSX::XLCellValue>> data;
    std::map<std::string, std::set<std::int32_t>> foreign_keys;
    std::set<std::int32_t> primary_key;

public:
    Table(std::string path);
    std::map<std::string, std::uint32_t> getHead();
    std::vector<OpenXLSX::XLCellValue> getColumn(std::string column_name);
    std::map<std::string, std::set<std::int32_t>> getForeignKeys();
    void insertForeignKeys(std::string foreign_key);
    void setPrimaryKey(std::string primary_key);
    std::set<std::int32_t> getForeignKey(std::string foreign_key);
    std::set<std::int32_t> getPrimaryKey();
    int getRealRowCount();
    std::string getName();
    ~Table();
};
