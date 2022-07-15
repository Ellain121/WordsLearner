#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "Utility.hpp"
#include "DatabaseManager.hpp"
#include "Training.hpp"

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
        };

    public:
        MainWindow(QWidget* parent = 0);
    
    private:
        void        setupCoreWidgets();
        void        setupCoreWidgetsConnections();

    private slots:
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
    
        QVBoxLayout*    mMainVBoxLayout;
        QPushButton*    mRepetitionButton;
        QPushButton*    mInitialTrainingButton;
        QPushButton*    mMakeWordButton;
        QPushButton*    mChooseWordButton;
        QPushButton*    mChooseTranslationButton;
        QPushButton*    mRainWordButton;
        QPushButton*    mDictionaryButton;

        MultipleTrainingVars    mMultiple;
};



#endif