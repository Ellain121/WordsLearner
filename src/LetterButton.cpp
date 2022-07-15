#include "LetterButton.hpp"

#include <QPainter>

LetterButton::LetterButton(QWidget* parent)
: QPushButton(parent)
, mChar('?')
, mCnt(1)
, mColor(255, 255, 255, 255)
{
}

LetterButton::LetterButton(QChar letter, int cnt, QWidget* parent)
: QPushButton(QString(letter), parent)
, mChar(letter)
, mCnt(cnt)
, mColor(255, 255, 255, 255)
{

}

void LetterButton::paintEvent(QPaintEvent* e)
{
    const auto& mSize = size();
    const QPointF pt = QPointF(mSize.width()/2, mSize.height()/2);
    QPushButton::paintEvent(e);
    QPainter p(this);
    // p.setPen(mColor);
    //    drawText(p, pt, Qt::AlignBottom, "_LB");
    //    drawText(p, pt, Qt::AlignVCenter, "_LC");
    //    drawText(p, pt, Qt::AlignTop, "_LT");
    p.setPen(mColor);
    //    drawText(p, pt, Qt::AlignBottom | Qt::AlignHCenter, "MB");
    QRect rect{0, 0, mSize.width(), mSize.height()};
    p.drawText(rect, Qt::AlignCenter, text());
    if (mCnt > 1)
    {
        p.setFont({"Helvetica", 7});
        p.drawText(rect, Qt::AlignRight | Qt::AlignBottom, QString::number(mCnt) + QString("  "));
    }
}

void LetterButton::setColor(const QColor& color)
{
    mColor = color;
}

void LetterButton::handleCorrectClick()
{
    mCnt--;
    if (mCnt < 1)
    {
        setVisible(false);
    }
    repaint();
}

char LetterButton::getChar() const
{
    return mChar.toLatin1();
}

QChar LetterButton::getQChar() const
{
    return mChar;
}