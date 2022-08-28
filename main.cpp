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
#include <thread>
#include <chrono>
using namespace std::chrono;
#include "headers/Table.h"
// namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace OpenXLSX;
namespace my
{

    class fn_object_class
    {
        // Overload () operator
    public:
        void operator()(int i, json rule)
        {
            // 切出主表
            json has_one_conditions = rule["has_one_condition"];
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
    };
}
int main()
{
    auto start = high_resolution_clock::now();
  
    // t2.join();
    // t2.join();
    // 项目缺少 xls 转 xlsx 的功能 , 现在有 昊喆 的 py 帮转

    // 读取规则文件

    json rule;
    std::ifstream i("rule.json");
    i >> rule;

    // 处理 has one 的关系
    json has_one_conditions = rule["has_one_condition"];
    std::vector<std::thread> ths;
    for (size_t i = 0; i < has_one_conditions.size(); i++)

    {
        ths.push_back(std::thread(my::fn_object_class(), i, rule));
    }
    for (auto &th : ths)
    {
        th.join();
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    // To get the value of duration use the count()
    // member function on the duration object
    std::cout << duration.count() << std::endl;
    return 0;
}



// CPP program to demonstrate multithreading
// // using three different callables.
// #include <iostream>
// #include <thread>
// using namespace std;

// // A dummy function
// void foo(int Z)
// {
//     for (int i = 0; i < Z; i++)
//     {
//         cout << "Thread using function"
//                 " pointer as callable\n";
//     }
// }

// // A callable object
// class thread_obj
// {
// public:
//     void operator()(int x)
//     {
//         for (int i = 0; i < x; i++)
//             cout << "Thread using function"
//                     " object as callable\n";
//     }
// };

// int main()
// {
//     cout << "Threads 1 and 2 and 3 "
//             "operating independently"
//          << endl;

//     // This thread is launched by using
//     // function pointer as callable
//     thread th1(foo, 3);

//     // This thread is launched by using
//     // function object as callable
//     thread th2(thread_obj(), 3);

//     // Define a Lambda Expression
//     auto f = [](int x)
//     {
//         for (int i = 0; i < x; i++)
//             cout << "Thread using lambda"
//                     " expression as callable\n";
//     };

//     // This thread is launched by using
//     // lamda expression as callable
//     thread th3(f, 3);

//     // Wait for the threads to finish
//     // Wait for thread t1 to finish
//     th1.join();

//     // Wait for thread t2 to finish
//     th2.join();

//     // Wait for thread t3 to finish
//     th3.join();

//     return 0;
// }
