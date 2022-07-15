#ifndef LETTERBUTTON_HPP
#define LETTERBUTTON_HPP

#include <QPushButton>

class LetterButton : public QPushButton
{
    Q_OBJECT

    public:
        LetterButton(QWidget* parent = 0);
        LetterButton(QChar letter, int cnt, QWidget* parent = 0);

        char    getChar() const;
        QChar   getQChar() const;

        void    handleCorrectClick();
        void    setColor(const QColor& color);
    
    private:
        void    paintEvent(QPaintEvent* e);
    
    private:
        QChar       mChar;
        int         mCnt;
        QColor      mColor;
    
};

#endif