#pragma once

#include <vector>
#include <string>
#include <variant>

class CTablePrinter 
{
private:
    using Cell = std::variant<std::string, std::wstring, std::vector<std::string>, std::vector<std::wstring>>;

public:
    void SetHeaders(const std::vector<std::string>& hdrs);
    void AddRow(const std::vector<Cell>& row);
    void Print() const;

private:
    std::vector<Cell> ToCells(const std::vector<std::string>& strRow) const;
    size_t GetMaxLineWidth(const Cell& cell) const;
    size_t GetCellHeight(const Cell& cell) const;
    void PrintRow(const std::vector<Cell>& row) const;
    void PrintSeparator() const;

private:
    std::vector<std::string> m_headers;
    std::vector<std::vector<Cell>> m_rows;
    std::vector<size_t> m_columnWidths;
};