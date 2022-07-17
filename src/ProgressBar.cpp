#include "ProgressBar.hpp"
#include "Utility.hpp"

#include <QInputDialog>

ProgressBar::ProgressBar(const QString& labelText, int value, int maxValue, QWidget* parent)
: QWidget(parent)
, mLabelText(labelText)
, mValue(value)
, mMaxValue(maxValue > 0 ? maxValue : 1)
{
    setupWidgets();
}

void ProgressBar::setupWidgets()
{
    mMainHBoxLayout = new QHBoxLayout;
    mLabel = new QLabel(mLabelText);
    mPrgBar = new QProgressBar;

    mLabel->setStyleSheet("font-size: 20px;");

    updateWidgets();

    mMainHBoxLayout->addWidget(mLabel);
    mMainHBoxLayout->addWidget(mPrgBar);

    setLayout(mMainHBoxLayout);
}

void ProgressBar::updateWidgets()
{
    mPrgBar->setFormat(QString::number(mValue) + "/" + QString::number(mMaxValue));
    mPrgBar->setMaximum(mMaxValue);
    mPrgBar->setValue(mValue > mMaxValue? mMaxValue : mValue);
    mPrgBar->setFixedWidth(WINDOW_WIDTH * 0.4);
}

void ProgressBar::setResetDate(const QString& resetDate)
{
    mResetDate = resetDate;
}

void ProgressBar::setVal(int val)
{
    mValue = val;
    updateWidgets();
}

void ProgressBar::addValue(int val)
{
    mValue += val;
    updateWidgets();
}

void ProgressBar::setMaxValue(int maxVal)
{
    mMaxValue = (maxVal > 0 ? maxVal : 1);
    updateWidgets();
}

void ProgressBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    bool ok;
    int goalNumber = QInputDialog::getInt(this, QString("Set your ") + mLabelText,
            tr("Set goal: "), mMaxValue, 0, 2147483647, 1, &ok);

    if (ok)
    {
        mMaxValue = goalNumber;
        mPrgBar->setMaximum(goalNumber);
        emit maxValueChanged(goalNumber);
        updateWidgets();
    }
}


int ProgressBar::getVal() const
{
    return mValue;
}

int ProgressBar::getMaxVal() const
{
    return mMaxValue;
}

QString ProgressBar::getResetDate() const
{
    return mResetDate;
}
