#include <OpenXLSX.hpp>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <filesystem>
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

int main()
{
    system(".\\xls2xlsx_Master.exe");
    auto start = high_resolution_clock::now();
    // 项目缺少 xls 转 xlsx 的功能 , 现在有 昊喆 的 py 帮转
    // 读取规则文件
    json rule;
    std::ifstream i("rule.json");
    i >> rule;

    // 处理 has one 的关系
    json has_one_conditions = rule["has_one_conditions"];
    for (auto& has_one_condition : has_one_conditions)
    {
        std::string main_table_name = has_one_condition["table"].get<std::string>();
        std::string main_table_path = "./xlsx/" + main_table_name + ".xlsx";
        Table main_table(main_table_path, main_table_name);

        std::cout << "====== " << main_table.getName() << " ====== " << std::endl;
        std::vector<json> has_ones = has_one_condition["has"].get<std::vector<json>>();

        //开打从表
        for (auto& has_one : has_ones)
        {
            main_table.insertForeignKeys(has_one["by"].get<std::string>());
            std::string foreign_table_name = "./xlsx/" + has_one["table"].get<std::string>() + ".xlsx";
            Table foreign_table(foreign_table_name, has_one["table"].get<std::string>());
            foreign_table.setPrimaryKey(has_one["to"].get<std::string>());

            std::set<int32_t> foreign_id = foreign_table.getPrimaryKey();
            std::set<int32_t> main_idsss = main_table.getForeignKey(has_one["by"].get<std::string>());
            std::vector<int> bewteen = has_one["between"].get<std::vector<int>>();
            for (auto& i : main_idsss)
            {
                if (i >= bewteen[0] && i <= bewteen[1] && foreign_id.find(i) == foreign_id.end())
                {
                    std::cout << main_table.getName() << " column(" << has_one["by"].get<std::string>() << ") has " << i;
                    std::cout << ", but " << foreign_table.getName() << " column(" << has_one["to"].get<std::string>() << ") miss " << i << std::endl;
                }
            }
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << "run time " << duration.count() / 1000000 << " s" << std::endl;
    system("pause");
    return 0;
}

// std::vector<std::thread> ths;
// // ths.push_back(std::thread(my::fn_object_class(), i, rule));
// for (auto &th : ths)
// {
//     // th.join();
// }

// system("pause");

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
