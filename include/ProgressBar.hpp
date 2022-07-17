#ifndef PROGRESSBAR_HPP
#define PROGRESSBAR_HPP

#include <QWidget>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QLabel>

class ProgressBar : public QWidget
{
    Q_OBJECT

    public:
        ProgressBar(const QString& labelText, int value, int maxValue, QWidget* parent = 0);

        void            setResetDate(const QString& resetDate);
        void            setVal(int val);
        void            addValue(int val);
        void            setMaxValue(int maxVal);
        virtual void    mouseDoubleClickEvent(QMouseEvent *event);

        int             getVal() const;
        int             getMaxVal() const;
        QString         getResetDate() const;
    
    private:
        void            setupWidgets();
        void            updateWidgets();
    
    signals:
        void    maxValueChanged(int val);

    
    private:
        QHBoxLayout*        mMainHBoxLayout;
        QLabel*             mLabel;
        QProgressBar*       mPrgBar;

        QString             mLabelText;
        QString             mResetDate;
        int                 mValue;
        int                 mMaxValue;
};


#endif