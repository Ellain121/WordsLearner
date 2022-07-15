#include "TableButton.hpp"

TableButton::TableButton(size_t row, const QString &text, QWidget* parent)
: QPushButton(text, parent)
, row(row)
{

}

size_t TableButton::getRow() const
{
    return row;
}