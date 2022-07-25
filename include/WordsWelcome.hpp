#ifndef WORDSWELCOME_HPP
#define WORDSWELCOME_HPP

#include "Training.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include <vector>
#include <string>

class WordsWelcome : public Training
{
    Q_OBJECT

    public:
        WordsWelcome(std::vector<Word>& words, int trainingBit, QWidget* parent = 0);

        TrainingType    getTrainingType() const;
    
    private:
        void    setupCoreWidgets();
        void    setupCoreWidgetsConnections();

        void    setupWordLabels(int wordIndx);

    private slots:
        void    nextWord();
        void    skipWord();

    private:
        QVBoxLayout*                mMainVBoxLayout;
        QHBoxLayout*                mButtonsHBoxLayout;
        QLabel*                     mWord;
        QLabel*                     mTranslation;
        QPushButton*                mBackButton;
        QPushButton*                mSkipButton;
        QPushButton*                mNextButton;

};

#endif