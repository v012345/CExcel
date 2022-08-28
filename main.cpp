#include <OpenXLSX.hpp>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <sstream>
// #include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <typeinfo>
#include "headers/Table.h"
// namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace OpenXLSX;

int main()
{
    // 项目缺少 xls 转 xlsx 的功能 , 现在有 昊喆 的 py 帮转

    // 读取规则文件
    json rule;
    std::ifstream i("rule.json");
    i >> rule;

    // 处理 has one 的关系
    json has_one_conditions = rule["has_one_condition"];
    for (size_t i = 0; i < has_one_conditions.size(); i++)

    {
        // 切出主表
        json has_one_relationship = has_one_conditions[i];
        std::cout << "======= dealing with " << has_one_relationship["table"].get<std::string>() << std::endl;
        std::string main_table_name = "./xlsx/" + has_one_relationship["table"].get<std::string>() + ".xlsx";

        Table main_table(main_table_name);
        std::map<std::string, std::uint32_t> table_head = main_table.getHead();
        // std::vector<OpenXLSX::XLCellValue> cc = main_table.getColumn("id");
        std::vector<json> has_ones = has_one_relationship["has"].get<std::vector<json>>();
        for (size_t i = 0; i < has_ones.size(); i++)
        {
            json foreign_table = has_ones[i];
            std::string foreign_key_name = foreign_table["by"].get<std::string>();
            main_table.insertForeignKeys(foreign_key_name);
        }

        //开打从表
        for (size_t i = 0; i < has_ones.size(); i++)
        {

            json foreign = has_ones[i];
            std::string foreign_table_name = "./xlsx/" + foreign["table"].get<std::string>() + ".xlsx";
            Table foreign_table(foreign_table_name);
            foreign_table.setPrimaryKey(foreign["to"].get<std::string>());

            std::set<int32_t> foreign_id = foreign_table.getPrimaryKey();
            std::set<int32_t> main_idsss = main_table.getForeignKey(foreign["by"].get<std::string>());
            std::vector<int> bewteen = foreign["between"].get<std::vector<int>>();
            for (auto &i : main_idsss)
            {
                if (i >= bewteen[0] && i <= bewteen[1])
                {
                    if (foreign_id.find(i) == foreign_id.end())
                    {

                        std::cout << main_table.getName() << " " << foreign["by"].get<std::string>() << " has " << i << std::endl;
                        std::cout << "but " << foreign_table.getName() << " " << foreign["to"].get<std::string>() << " missing " << i << std::endl;
                    }
                }
            }
        }
        std::cout << " ====== done" << std::endl;
    }

    return 0;
}