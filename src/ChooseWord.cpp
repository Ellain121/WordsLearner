#include "ChooseWord.hpp"

#include <QTimer>

#include <cassert>

ChooseWord::ChooseWord(std::vector<Word>& words, std::vector<SimpleWord>&& simpleWords, int trainingBit, QWidget* parent)
: Training(words, trainingBit, parent)
, mSimpleWords(simpleWords)
, mSimpleWordsIndx(0)
{
    assert(!mWords.empty());

    setupCoreWidgets();
    setupCoreWidgetsConnections();
}

void ChooseWord::setupCoreWidgets()
{
    mMainVBoxLayout = new QVBoxLayout;
    mWordButtonsVBoxLayout = new QVBoxLayout;
    mButtonsHBoxLayout = new QHBoxLayout;
    mTranslation = new QLabel;
    mBackButton = new QPushButton("Back");
    mSkipButton = new QPushButton("I don't know");
    mNextButton = new QPushButton("Next");

    mWordVariantsButtons.resize(5);
    for (int i = 0; i < 5; ++i)
    {
        mWordVariantsButtons[i] = new QPushButton;
    }

    mBackButton->setShortcut(QKeySequence(Qt::Key_Escape));
    mSkipButton->setShortcut(QKeySequence(Qt::Key_Space));

    // mWord->setStyleSheet("font-weight: bold; color: green");
    mTranslation->setStatusTip("color: white");
    setupWordsAndTranslation();

    mBackButton->setFixedSize(WINDOW_WIDTH * 0.12 , WINDOW_HEIGHT * 0.08);
    mSkipButton->setFixedSize(WINDOW_WIDTH * 0.17 , WINDOW_HEIGHT * 0.11);
    mNextButton->setFixedSize(WINDOW_WIDTH * 0.17 , WINDOW_HEIGHT * 0.11);

    mMainVBoxLayout->addWidget(mBackButton, 0, Qt::AlignLeft);
    mMainVBoxLayout->addStretch(1);
    // mMainVBoxLayout->addWidget(mWord, 0, Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mTranslation, 0, Qt::AlignCenter);
    mMainVBoxLayout->addStretch(1);
    for (int i = 0; i < 5; ++i)
    {
        mWordButtonsVBoxLayout->addWidget(mWordVariantsButtons[i]);
    }
    mMainVBoxLayout->addLayout(mWordButtonsVBoxLayout);
    mWordButtonsVBoxLayout->setAlignment(Qt::AlignCenter);
    mMainVBoxLayout->addStretch(1);
    mButtonsHBoxLayout->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
    mButtonsHBoxLayout->addWidget(mSkipButton);
    mButtonsHBoxLayout->addStretch(1);
    mButtonsHBoxLayout->addWidget(mNextButton);
    mMainVBoxLayout->addLayout(mButtonsHBoxLayout);

    setLayout(mMainVBoxLayout);
}

void ChooseWord::setupCoreWidgetsConnections()
{
    connect(mBackButton, &QPushButton::clicked, [this](){
        this->backToMenu();
    });
}

void ChooseWord::setupWordsAndTranslation()
{
    mTranslation->setText(QString::fromStdString(mWords[mWordIndx].translation));
    setupWordVariantsButtons();
}

void ChooseWord::setupWordVariantsButtons()
{
    int rightWordIndx = mRandEngine.getRandom(5);
    auto rightWord = mWords[mWordIndx].word;
    for (int i = 0; i < 5; ++i)
    {
        QPushButton* pushButton = mWordVariantsButtons[i];
        pushButton->disconnect();
        if (i == rightWordIndx)
        {
            pushButton->setText(QString::fromStdString(rightWord));
            connect(pushButton, &QPushButton::clicked, [this, pushButton](){
                pushButton->setStyleSheet("color: limegreen; font-size: 18px;");
                QTimer::singleShot(700, this, &ChooseWord::nextWord);

            });
        }
        else
        {
            assert(mSimpleWordsIndx + 1 < mSimpleWords.size());
            auto& word = mSimpleWords[mSimpleWordsIndx++].word;
            pushButton->setText(QString::fromStdString(word));
            connect(pushButton, &QPushButton::clicked, [this, pushButton](){
                pushButton->setStyleSheet("color: red; font-size: 22px;");
                QTimer::singleShot(1000, this, &ChooseWord::skipWord);

            });
        }
        pushButton->setStyleSheet("font-size: 20px;");
        pushButton->setShortcut(QKeySequence(i + '1'));
        pushButton->setFixedSize(WINDOW_WIDTH * 0.6 , WINDOW_HEIGHT * 0.12);
    }
}

void ChooseWord::nextWord()
{
    Training::nextWord();
    if (isTrainingDone())
    {
        for (auto& variantButton : mWordVariantsButtons)
        {
            variantButton->disconnect();            
        }
    }
    else
    {
        setupWordsAndTranslation();
    }
}

void ChooseWord::skipWord()
{
    Training::skipWord();
    setupWordsAndTranslation();  
}

TrainingType ChooseWord::getTrainingType() const
{
    return TrainingType::ChooseWord_Train;
}