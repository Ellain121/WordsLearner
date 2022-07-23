#include "StatisticsWidget.hpp"
#include "DatabaseManager.hpp"

StatisticsWidget::StatisticsWidget(DatabaseManager* dbManager, QWidget* parent)
: QWidget(parent)
, mDBManager(dbManager)
, mTotalWordsCnt(0)
, mFullyLearnedWordsCnt(0)
{
    setupWidgets();
    setupWidgetsConnections();
}

StatisticsWidget::~StatisticsWidget()
{
    delete mPrgBarCont;
}

void StatisticsWidget::setupWidgets()
{
    mMainLayout = new QGridLayout;
    mTotalWordsLabel = new QLabel;
    mFullyLearnedWordsLabel = new QLabel;
    mUnlearnedWordsLabel = new QLabel;
    setupLabels();

    mDailyPrgBar = new ProgressBar("Daily goal: ", 25, 10);
    mWeeklyPrgBar = new ProgressBar("Weekly goal: ", 35, 70);
    mMonthlyPrgBar = new ProgressBar("Monthly goal: ", 123, 300);
    mYearlyPrgBar = new ProgressBar("Yearly goal: ", 500, 3650);

    mPrgBarCont = new ProgressBarContainer ({
        mDailyPrgBar, mWeeklyPrgBar, mMonthlyPrgBar, mYearlyPrgBar});
    

    mMainLayout->addWidget(mTotalWordsLabel,        0, 0, Qt::AlignCenter);
    mMainLayout->addWidget(mFullyLearnedWordsLabel, 1, 0, Qt::AlignCenter);
    mMainLayout->addWidget(mUnlearnedWordsLabel,    2, 0, Qt::AlignCenter);
    mMainLayout->addWidget(mDailyPrgBar,            0, 1, Qt::AlignCenter);
    mMainLayout->addWidget(mWeeklyPrgBar,           1, 1, Qt::AlignCenter);
    mMainLayout->addWidget(mMonthlyPrgBar,          2, 1, Qt::AlignCenter);
    mMainLayout->addWidget(mYearlyPrgBar,           3, 1, Qt::AlignCenter);

    setLayout(mMainLayout);
}

void StatisticsWidget::setupLabels()
{
    mTotalWordsCnt = mDBManager->countWords();
    mFullyLearnedWordsCnt = mDBManager->countFullyLearnedWords();
    
    mTotalWordsLabel = new QLabel(QString("Total words: ") + QString::number(mTotalWordsCnt));
    mFullyLearnedWordsLabel = new QLabel(QString("Learned words: ") + QString::number(mFullyLearnedWordsCnt));
    mUnlearnedWordsLabel = new QLabel(QString("Unlearned words: ") + QString::number(mTotalWordsCnt - mFullyLearnedWordsCnt));
}

void StatisticsWidget::updateLabels()
{
    mTotalWordsLabel->setText(QString("Total words: ") + QString::number(mTotalWordsCnt));
    mFullyLearnedWordsLabel->setText(QString("Learned words: ") + QString::number(mFullyLearnedWordsCnt));
    mUnlearnedWordsLabel->setText(QString("Unlearned words: ") + QString::number(mTotalWordsCnt - mFullyLearnedWordsCnt));
}

void StatisticsWidget::setupWidgetsConnections()
{
}

void StatisticsWidget::wordDeletedOrReset(QString learnedDate, bool isFullyLearned, bool isDeleted)
{
    mTotalWordsCnt -= isDeleted;
    if (!isFullyLearned) return;

    mFullyLearnedWordsCnt--;
    updateLabels();

    QString previousDayDate = Time::getPreviousStartDateStr(TimeInterval::Day);
    QString previousWeekDate = Time::getPreviousStartDateStr(TimeInterval::Week);
    QString previousMonthDate = Time::getPreviousStartDateStr(TimeInterval::Month);
    QString previousYearDate = Time::getPreviousStartDateStr(TimeInterval::Year);
    
    if (Time::compare(learnedDate, previousDayDate)) mDailyPrgBar->addValue(-1);
    if (Time::compare(learnedDate, previousWeekDate)) mWeeklyPrgBar->addValue(-1);
    if (Time::compare(learnedDate, previousMonthDate)) mMonthlyPrgBar->addValue(-1);
    if (Time::compare(learnedDate, previousYearDate)) mYearlyPrgBar->addValue(-1);
    mPrgBarCont->writeChanges();
    
}

void StatisticsWidget::updateStatistics(int newlyLearnedWordsCnt)
{
    mFullyLearnedWordsCnt += newlyLearnedWordsCnt;
    updateLabels();

    mDailyPrgBar->addValue(newlyLearnedWordsCnt);
    mWeeklyPrgBar->addValue(newlyLearnedWordsCnt);
    mMonthlyPrgBar->addValue(newlyLearnedWordsCnt);
    mYearlyPrgBar->addValue(newlyLearnedWordsCnt);
    mPrgBarCont->writeChanges();
}

void StatisticsWidget::newWordAdded()
{
    mTotalWordsCnt++;
    updateLabels();
}
