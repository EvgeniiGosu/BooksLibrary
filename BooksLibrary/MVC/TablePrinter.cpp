#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <wchar.h>

#include "../../CommonLib/Str.h"

#include "TablePrinter.h"

namespace {
    size_t count_utf8_chars(const std::string& str)
    {
        size_t count = 0;
        for (size_t i = 0; i < str.size(); ++i)
            if ((str[i] & 0xC0) != 0x80)
                ++count;

        return count;
    }
}

void CTablePrinter::SetHeaders(const std::vector<std::string>& hdrs)
{
    m_headers = hdrs;
    m_columnWidths.resize(m_headers.size());

    for (size_t i = 0; i < m_headers.size(); ++i)
        m_columnWidths[i] = m_headers[i].length();
}

void CTablePrinter::AddRow(const std::vector<Cell>& row)
{
    if (row.size() != m_headers.size())
        throw std::invalid_argument("The number of elements in a row must match the number of headings.");

    m_rows.push_back(row);

    for (size_t i = 0; i < row.size(); ++i)
    {
        size_t cellWidth = GetMaxLineWidth(row[i]);
        m_columnWidths[i] = std::max(m_columnWidths[i], cellWidth);
    }
}

void CTablePrinter::Print() const
{
    PrintSeparator();

    PrintRow(ToCells(m_headers));

    PrintSeparator();

    for (const auto& row : m_rows)
    {
        PrintRow(row);
        PrintSeparator();
    }
}

std::vector<CTablePrinter::Cell> CTablePrinter::ToCells(const std::vector<std::string>& strRow) const
{
    std::vector<Cell> cells;

    for (const auto& s : strRow)
        cells.emplace_back(s);

    return cells;
}

size_t CTablePrinter::GetMaxLineWidth(const Cell& cell) const
{
    if (std::holds_alternative<std::string>(cell))
    {
        return std::get<std::string>(cell).length();
    }
    else if (std::holds_alternative<std::wstring>(cell))
    {
        return std::get<std::wstring>(cell).length();
    }
    else if (std::holds_alternative<std::vector<std::string>>(cell))
    {
        const auto& lines = std::get<std::vector<std::string>>(cell);
        size_t maxLen = 0;
        for (const auto& line : lines)
            maxLen = std::max(maxLen, line.length());
        return maxLen;
    }
    else
    {
        const auto& lines = std::get<std::vector<std::wstring>>(cell);
        size_t maxLen = 0;
        for (const auto& line : lines)
            maxLen = std::max(maxLen, str::WstringToUtf8String(line).length());
        return maxLen;
    }
}

size_t CTablePrinter::GetCellHeight(const Cell& cell) const
{
    if (std::holds_alternative<std::string>(cell))
        return 1;
    else if (std::holds_alternative<std::wstring>(cell))
        return 1;
    else if (std::holds_alternative<std::vector<std::string>>(cell))
        return std::get<std::vector<std::string>>(cell).size();
    else
        return std::get<std::vector<std::wstring>>(cell).size();
}

void CTablePrinter::PrintRow(const std::vector<Cell>& row) const
{
    size_t maxLines = 1;
    for (const auto& cell : row)
        maxLines = std::max(maxLines, GetCellHeight(cell));

    for (size_t line = 0; line < maxLines; ++line)
    {
        std::cout << "|";
        for (size_t col = 0; col < row.size(); ++col)
        {
            std::string content;

            if (std::holds_alternative<std::string>(row[col]))
            {
                content = (line == 0) ? std::get<std::string>(row[col]) : "";
            }
            else if (std::holds_alternative<std::wstring>(row[col]))
            {
                content = (line == 0) ? str::WstringToUtf8String(std::get<std::wstring>(row[col])) : "";
            }
            else if (std::holds_alternative<std::vector<std::string>>(row[col]))
            {
                const auto& lines = std::get<std::vector<std::string>>(row[col]);
                content = (line < lines.size()) ? lines[line] : "";
            }
            else
            {
                const auto& lines = std::get<std::vector<std::wstring>>(row[col]);
                content = (line < lines.size()) ? str::WstringToUtf8String(lines[line]) : "";
            }

            std::cout << " " << std::setw(m_columnWidths[col] + (content.size() - count_utf8_chars(content))) << std::left << content << " |";
        }
        
        std::cout << "\n";
    }
}

void CTablePrinter::PrintSeparator() const
{
    std::cout << "+";
    for (size_t width : m_columnWidths)
        std::cout << std::string(width + 2, '-') << "+";

    std::cout << "\n";
}
