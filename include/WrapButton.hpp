#ifndef WRAPBUTTON_HPP
#define WRAPBUTTON_HPP

#include <QPushButton>

class WrapButton : public QPushButton
{
    Q_OBJECT

    public:
        WrapButton(QWidget* parent = 0);
        WrapButton(const QString& text, QWidget* parent = 0);

        virtual void        setText(const QString &text);

        virtual void        setFixedSize(int w, int h);
        virtual void        setFixedSize(float w, float h);
        virtual void        setFixedSize(double w, double h);
    
    private:
        void    wrapText(const QString& text);
    
    private:
        const int     mIndentation;
};

#endif