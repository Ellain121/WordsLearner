#ifndef TABLEBUTTON_HPP
#define TABLEBUTTON_HPP

#include <QPushButton>

class TableButton : public QPushButton
{
    Q_OBJECT
    
    public:
                TableButton(size_t row, const QString &text, QWidget* parent = 0);
            
        size_t      getRow() const;
    
    private:
        size_t          row;
};

#endif