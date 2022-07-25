#ifndef RAINWORD_HPP
#define RAINWORD_HPP

#include "Training.hpp"
#include "WrapButton.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPropertyAnimation>

class RainWord : public Training
{
    Q_OBJECT

    public:
        RainWord(std::vector<Word>& words, std::vector<SimpleWord>&& simpleWords, int trainingBit, QWidget* parent = 0);

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

        QPropertyAnimation*         mAnimation;
        QLabel*                     mWord;

        QVBoxLayout*                mMainVBoxLayout;
        QHBoxLayout*                mTranslationButtonsVBoxLayout;
        QPushButton*                mBackButton;
        QVector<WrapButton*>        mTranslationVariantsButtons;

        RandomEngine                mRandEngine;
};

#endif