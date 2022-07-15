#ifndef MAKEWORD_HPP
#define MAKEWORD_HPP

#include "Training.hpp"
#include "LetterButton.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVector>

#include <vector>

class MakeWord : public Training
{
    Q_OBJECT

    public:
        MakeWord(std::vector<Word>& words, QWidget* parent = 0);

        virtual TrainingType    getTrainingType() const;

    private:
        void        setupCoreWidgets();
        void        setupCoreWidgetsConnections();

        void        setupWordAndTranslation();
        void        setupWordButtons();
        void        setupWordButtonsConnections();

    private slots:
        bool        letterButtonPressed(char ch);
        void        nextWord();
        void        skipWord();

    private:
        QVBoxLayout*            mMainVBoxLayout;
        QHBoxLayout*            mLettersHBoxLayout;
        QLabel*                 mTranslationLabel;
        QLabel*                 mWordLabel;
        QPushButton*            mBackButton;
          
        std::vector<LetterButton*>  mWordButtons;

        int                     mLetterIndx;
        int                     mWordMistakesCnt;
};


#endif