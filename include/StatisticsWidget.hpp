#ifndef STATISTICSWIDGET_HPP
#define STATISTICSWIDGET_HPP

#include "ProgressBarContainer.hpp"
#include "ProgressBar.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

class DatabaseManager;

class StatisticsWidget : public QWidget
{
    Q_OBJECT

    public:
        StatisticsWidget(DatabaseManager* dbManager, QWidget* parent = 0);
        ~StatisticsWidget();

        void    updateStatistics(int newlyLearnedWordsCnt);

    private:
        void    setupWidgets();
        void    setupWidgetsConnections();
        void    setupLabels();
        void    updateLabels();
    
    public slots:
        void    wordDeletedOrReset(QString learnedDate, bool isFullyLearned, bool isDeleted);
        void    newWordAdded();
    
    
    private:
        DatabaseManager*            mDBManager;
        QGridLayout*                mMainLayout;
        QLabel*                     mTotalWordsLabel;
        QLabel*                     mFullyLearnedWordsLabel;
        QLabel*                     mUnlearnedWordsLabel;
        ProgressBar*                mDailyPrgBar;
        ProgressBar*                mWeeklyPrgBar;
        ProgressBar*                mMonthlyPrgBar;
        ProgressBar*                mYearlyPrgBar;
        ProgressBarContainer*       mPrgBarCont;
        int                         mTotalWordsCnt;
        int                         mFullyLearnedWordsCnt;

};

#endif