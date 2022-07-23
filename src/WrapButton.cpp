#include "WrapButton.hpp"

namespace
{

}

WrapButton::WrapButton(QWidget* parent)
: QPushButton(parent)
, mIndentation(5)
{
}

WrapButton::WrapButton(const QString& text, QWidget* parent)
: WrapButton(parent)
{
    wrapText(text);
}

void WrapButton::setFixedSize(int w, int h)
{
    QPushButton::setFixedSize(w, h);
    wrapText(text());
}

void WrapButton::setFixedSize(float w, float h)
{
    setFixedSize((int)w, (int)h);
}

void WrapButton::setFixedSize(double w, double h)
{
    setFixedSize((int)w, (int)h);
}

void WrapButton::setText(const QString &text)
{
    wrapText(text);
}

void WrapButton::wrapText(const QString& text)
{
    QFont myFont = font();
    // QString str("I wonder how wide this is?");

    QFontMetrics fm(myFont);
    int textWidth = fm.width(text);
    int allowedWidth = size().width() - mIndentation * 2;
    
    QString line;
    QString result;
    int indx = 0;
    while (indx < text.size())
    {
        if (text[indx] == '\n')
        {
            indx++;
            continue;
        }
        
        line += text[indx++];
        int width = fm.width(line);
        if (width >= allowedWidth)
        {
            // std::cout << "newLine!\n";
            indx--;
            line.chop(1);
            result += line + '\n';
            line.clear();
        }
    }

    if (!line.isEmpty())
    {
        result += line;
    }

    QPushButton::setText(result);
}