#ifndef CHOOSETRANSLATION_HPP
#define CHOOSETRANSLATION_HPP

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVector>

#include "Training.hpp"

#include <vector>

class ChooseTranslation : public Training
{
    Q_OBJECT

    public:
        ChooseTranslation(std::vector<Word>& words, std::vector<SimpleWord>&& simpleWords,
                int trainingBit, QWidget* parent = 0);

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
        QVBoxLayout*                mTranslationButtonsVBoxLayout;
        QHBoxLayout*                mButtonsHBoxLayout;
        QLabel*                     mWord;
        QPushButton*                mBackButton;
        QPushButton*                mSkipButton;
        QPushButton*                mNextButton;
        QVector<QPushButton*>       mTranslationVariantsButtons;

        RandomEngine                mRandEngine;
};

#endif