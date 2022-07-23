#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "Utility.hpp"
#include "DatabaseManager.hpp"
#include "Training.hpp"
#include "StatisticsWidget.hpp"

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>

#include <vector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    private:
        struct MultipleTrainingVars
        {
            std::vector<bool>       trainsDone;
            std::vector<Training*>  trains;
            int                     indx;
        };

    public:
        MainWindow(QWidget* parent = 0);
    
    private:
        void        setupCoreWidgets();
        void        setupCoreWidgetsConnections();
        void        updateButtonsText();
    
    protected:
        virtual void    closeEvent(QCloseEvent *event);

    private slots:
        void        initRepetitionTraining();
        void        initInitialTraining();
        void        initMakeWordTraining();
        void        initChooseWordTraining();
        void        initChooseTranslation();
        void        initRainWord();
        void        initDictionaryState();

        void        activeTrainingDone(TrainingType trainType);
        void        activateMenu();

    private:
        DatabaseManager     mDBManager;
        std::vector<Word>   mCurrentWordsToLearn;

        QWidget*            mMainWidget;
        QStackedWidget*     mMainStackedWidget;
    
        QVBoxLayout*        mMainVBoxLayout;
        StatisticsWidget*   mStatisticsWidget;
        QPushButton*        mRepetitionButton;
        QPushButton*        mInitialTrainingButton;
        QPushButton*        mMakeWordButton;
        QPushButton*        mChooseWordButton;
        QPushButton*        mChooseTranslationButton;
        QPushButton*        mRainWordButton;
        QPushButton*        mDictionaryButton;

        MultipleTrainingVars    mMultiple;
};



#endif