#include <iostream>
#include <filesystem>
#include <thread>
#include <vector>
#include <chrono>
int main(int argc, char const* argv[])
{
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "xls to xlsx..." << std::endl;
    std::cout << "md5 need to be implemented" << std::endl;
    std::vector<std::string> xlsFiles;
    std::vector<std::thread> cmds;

    for (auto&& file : std::filesystem::directory_iterator(std::filesystem::current_path().parent_path()))
    {

        if (file.is_regular_file() && file.path().extension().string() == ".xls")
        {
            xlsFiles.push_back(file.path().stem().string());
        }
    }


    int deal_count_per_thread = 30;

    for (size_t i = 0; i < xlsFiles.size() / deal_count_per_thread; i++)
    {
        std::string cmd = ".\\xls2xlsx_Worker.exe ";
        for (size_t j = i * deal_count_per_thread; j < i * deal_count_per_thread + deal_count_per_thread; j++)
        {
            if (j >= xlsFiles.size())
                break;
            cmd.append("\"").append(xlsFiles[j]).append("\" ");
        }

        cmds.push_back(std::thread([cmd, i]() { system(cmd.c_str()); }));
    }
    for (auto&& cmd : cmds)
    {
        cmd.join();
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "run time " << duration.count() / 1000000 << " s" << std::endl;
    return 0;
}
