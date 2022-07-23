#ifndef REPETITION_HPP
#define REPETITION_HPP

#include "Training.hpp"
#include "WrapButton.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QVector>

#include <vector>

class Repetition : public Training
{
    Q_OBJECT

    public:
        Repetition(std::vector<Word>& words, std::vector<SimpleWord>&& simpleWords, QWidget* parent = 0);

        TrainingType    getTrainingType() const;

    private:
        void    setupCoreWidgets();
        void    setupCoreWidgetsConnections();   

        void    setupWordsAndTranslation();
        void    setupTranslationVariantsButtons();
    
    private slots:
        void    nextWord();
        void    skipWord();

    private:
        std::vector<SimpleWord>     mSimpleWords;
        int                         mSimpleWordsIndx;

        QVBoxLayout*                mMainVBoxLayout;
        QHBoxLayout*                mTranslationButtonsVBoxLayout;;
        QLabel*                     mWord;
        QPushButton*                mBackButton;
        QPushButton*                mSkipButton;
        QVector<WrapButton*>        mTranslationVariantsButtons;

        RandomEngine                mRandEngine;     
};

#endif