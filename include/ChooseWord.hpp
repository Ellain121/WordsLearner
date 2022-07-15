#ifndef CHOOSEWORD_HPP
#define CHOOSEWORD_HPP

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVector>

#include "Training.hpp"

#include <vector>

class ChooseWord : public Training
{
    Q_OBJECT

    public:
        ChooseWord(std::vector<Word>& words, std::vector<SimpleWord>&& simpleWords, QWidget* parent = 0);

        TrainingType    getTrainingType() const;
    
    private:
        void    setupCoreWidgets();
        void    setupCoreWidgetsConnections();

        void    setupWordsAndTranslation();
        void    setupWordVariantsButtons();
    
    private slots:
        void    nextWord();
        void    skipWord();
    
    private:
        std::vector<SimpleWord>     mSimpleWords;
        int                         mSimpleWordsIndx;

        QVBoxLayout*                mMainVBoxLayout;
        QVBoxLayout*                mWordButtonsVBoxLayout;
        QHBoxLayout*                mButtonsHBoxLayout;
        QLabel*                     mTranslation;
        QPushButton*                mBackButton;
        QPushButton*                mSkipButton;
        QPushButton*                mNextButton;
        QVector<QPushButton*>       mWordVariantsButtons;

        RandomEngine                mRandEngine;
};

#endif