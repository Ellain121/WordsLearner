#include "WordsWelcome.hpp"

#include <QFont>

#include <cassert>

WordsWelcome::WordsWelcome(std::vector<Word>& words, int trainingBit, QWidget* parent)
: Training(words, trainingBit, parent)
{
    assert(!mWords.empty());

    setupCoreWidgets();
    setupCoreWidgetsConnections();
}

void WordsWelcome::setupCoreWidgets()
{
    mMainVBoxLayout = new QVBoxLayout;
    mButtonsHBoxLayout = new QHBoxLayout;
    mWord = new QLabel;
    mTranslation = new QLabel;
    mBackButton = new QPushButton("Back");
    mSkipButton = new QPushButton("Skip");
    mNextButton = new QPushButton("Next");

    mWord->setStyleSheet("font-weight: bold; color: green");
    mTranslation->setStatusTip("color: white");

    mBackButton->setFixedSize(WINDOW_WIDTH * 0.12 , WINDOW_HEIGHT * 0.08);
    mSkipButton->setFixedSize(WINDOW_WIDTH * 0.17 , WINDOW_HEIGHT * 0.11);
    mNextButton->setFixedSize(WINDOW_WIDTH * 0.17 , WINDOW_HEIGHT * 0.11);

    mBackButton->setShortcut(QKeySequence(Qt::Key_Escape));
    mSkipButton->setShortcut(QKeySequence(Qt::Key_Space));
    mNextButton->setShortcut(QKeySequence(Qt::Key_Return));

    mMainVBoxLayout->addWidget(mBackButton, 0, Qt::AlignLeft);
    mMainVBoxLayout->addStretch(1);
    mMainVBoxLayout->addWidget(mWord, 0, Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mTranslation, 0, Qt::AlignCenter);
    mMainVBoxLayout->addStretch(2);
    mButtonsHBoxLayout->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
    mButtonsHBoxLayout->addWidget(mSkipButton);
    mButtonsHBoxLayout->addStretch(1);
    mButtonsHBoxLayout->addWidget(mNextButton);
    mMainVBoxLayout->addLayout(mButtonsHBoxLayout);

    setupWordLabels(mWordIndx);

    setLayout(mMainVBoxLayout);
}

void WordsWelcome::setupWordLabels(int wordIndx)
{
    mWord->setText(QString::fromStdString(mWords[wordIndx].word));
    mTranslation->setText(QString::fromStdString(mWords[wordIndx].translation));
}

void WordsWelcome::setupCoreWidgetsConnections()
{
    connect(mSkipButton, &QPushButton::clicked, this, &WordsWelcome::skipWord);
    connect(mNextButton, &QPushButton::clicked, this, &WordsWelcome::nextWord);
    connect(mBackButton, &QPushButton::clicked, [this](){
        this->backToMenu();
    });
}

void WordsWelcome::nextWord()
{
    Training::nextWord();
    if (isTrainingDone())
    {
        disconnect(mSkipButton, &QPushButton::clicked, this, &WordsWelcome::skipWord);
        disconnect(mNextButton, &QPushButton::clicked, this, &WordsWelcome::nextWord);
        disconnect(mBackButton, &QPushButton::clicked, this, &WordsWelcome::nextWord);
    }
    else
    {
        setupWordLabels(mWordIndx);
    }
}

void WordsWelcome::skipWord()
{
    Training::skipWord();
    setupWordLabels(mWordIndx);
}

TrainingType WordsWelcome::getTrainingType() const
{
    return TrainingType::WordsWelcome_Train;
}