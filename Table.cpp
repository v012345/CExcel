#include "headers/Table.h"
Table::Table(std::string path)
{
    this->name = path;
    this->table.open(path);
    this->sheet = table.workbook().worksheet(table.workbook().worksheetNames()[0]);

    std::map<std::string, std::uint32_t> head;
    OpenXLSX::XLRowIterator row = sheet.rows().begin();
    int index = 0;
    for (auto &value : std::vector<OpenXLSX::XLCellValue>(row->values()))
    {
        head.insert(std::make_pair(value.get<std::string>(), index));
        index++;
    }
   
    this->head = head;
}
std::string Table::getName()
{
    return this->name;
};
void Table::setPrimaryKey(std::string primary_key)
{
    std::set<std::int32_t> primary_keys;
    for (OpenXLSX::XLCellValue &v : this->getColumn(primary_key))
    {
        if (v.type() == OpenXLSX::XLValueType::Integer)
        {
            primary_keys.insert(v.get<std::int32_t>());
        }
        else if (v.type() == OpenXLSX::XLValueType::String)
        {
            primary_keys.insert(stoi(v.get<std::string>()));
        }
        else
        {
            break;
        }
    }
    this->primary_key = primary_keys;
}
void Table::insertForeignKeys(std::string foreign_key)
{
    if (this->foreign_keys.find(foreign_key) != this->foreign_keys.end())
    {
        return;
    }
    std::set<std::int32_t> foreign_keys;
    for (OpenXLSX::XLCellValue &v : this->getColumn(foreign_key))
    {
        if (v.type() == OpenXLSX::XLValueType::Integer)
        {
            // std::cout << m.get<int>() << std::endl;
            foreign_keys.insert(v.get<std::int32_t>());
        }
        else if (v.type() == OpenXLSX::XLValueType::String)
        {
            std::stringstream test(v.get<std::string>().c_str());
            std::string segment;
            std::vector<std::string> seglist;

            while (std::getline(test, segment, '|'))
            {
                try
                {
                    foreign_keys.insert(stoi(segment));
                }
                catch (const std::exception &e)
                {
                    std::cout << "table " << this->name << " column " << foreign_key << " has a value " << segment << " which can convert to int" << std::endl;
                    // std::cerr << e.what() << '\n';
                }
            }
        }
    }

    this->foreign_keys.insert(std::make_pair(foreign_key, foreign_keys));
}
std::map<std::string, std::set<std::int32_t>> Table::getForeignKeys()
{
    return this->foreign_keys;
}
std::map<std::string, std::uint32_t> Table::getHead()
{
    return this->head;
}
std::set<std::int32_t> Table::getForeignKey(std::string foreign_key)
{
    return this->foreign_keys.at(foreign_key);
}

std::set<std::int32_t> Table::getPrimaryKey()
{
    return this->primary_key;
}

std::vector<OpenXLSX::XLCellValue> Table::getColumn(std::string column_name)
{
    if (this->data.find(column_name) == this->data.end())
    {
        OpenXLSX::XLRowIterator row = sheet.rows().begin();
        row++;
        std::vector<OpenXLSX::XLCellValue> column_data;
        int i = 0;
        while (row != this->sheet.rows().end())
        {
            OpenXLSX::XLCellValue m = std::vector<OpenXLSX::XLCellValue>(row->values()).at(this->head.at(column_name));
            column_data.push_back(m);
            if (i == 5465)
            {
                std::cout << i++ << std::endl;
            }
            i++;

            row++;
        }
        this->data.insert(std::make_pair(column_name, column_data));
    }
    return this->data.at(column_name);
}
Table::~Table()
{
    this->table.close();
}