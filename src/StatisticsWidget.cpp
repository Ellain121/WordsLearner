#include "StatisticsWidget.hpp"
#include "DatabaseManager.hpp"

StatisticsWidget::StatisticsWidget(DatabaseManager* dbManager, QWidget* parent)
: QWidget(parent)
, mDBManager(dbManager)
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
    int wordsCnt = mDBManager->countWords();
    int fullyLearnedCnt = mDBManager->countFullyLearnedWords();

    mMainLayout = new QGridLayout;
    mTotalWordsLabel = new QLabel(QString("Total words: ") + QString::number(wordsCnt));
    mFullyLearnedWordsLabel = new QLabel(QString("Learned words: ") + QString::number(fullyLearnedCnt));
    mUnlearnedWordsLabel = new QLabel(QString("Unlearned words: ") + QString::number(wordsCnt - fullyLearnedCnt));

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

void StatisticsWidget::setupWidgetsConnections()
{

}

void StatisticsWidget::updateStatistics(int newlyLearnedWordsCnt)
{
    mDailyPrgBar->addValue(newlyLearnedWordsCnt);
    mWeeklyPrgBar->addValue(newlyLearnedWordsCnt);
    mMonthlyPrgBar->addValue(newlyLearnedWordsCnt);
    mYearlyPrgBar->addValue(newlyLearnedWordsCnt);
    mPrgBarCont->writeChanges();
}

