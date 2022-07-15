#include "ChooseWord.hpp"

#include <QTimer>

#include <cassert>

ChooseWord::ChooseWord(std::vector<Word>& words, std::vector<SimpleWord>&& simpleWords, QWidget* parent)
: Training(words, getTrainingStatusBit(TrainingType::ChooseWord_Train), parent)
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
    mSkipButton = new QPushButton("Skip");
    mNextButton = new QPushButton("Next");

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
     // delete all previou letter buttons, clear vector, clear layout, clear indices
    mWordVariantsButtons.clear();
    QLayoutItem* item;
    while ( ( item = mWordButtonsVBoxLayout->takeAt( 0 ) ) != NULL )
    {
        // std::cout << "DELETED\n";
        delete item->widget();
        delete item;
    }
    //-----------------------------------------------------
    int rightWordIndx = mRandEngine.getRandom(5);
    auto rightWord = mWords[mWordIndx].word;
    for (int i = 0; i < 5; ++i)
    {
        QPushButton* pushButton;
        if (i == rightWordIndx)
        {
            pushButton = new QPushButton(QString::fromStdString(rightWord));
            connect(pushButton, &QPushButton::clicked, [this, pushButton](){
                pushButton->setStyleSheet("color: limegreen");
                QTimer::singleShot(700, this, &ChooseWord::nextWord);

            });
        }
        else
        {
            assert(mSimpleWordsIndx + 1 < mSimpleWords.size());
            auto& word = mSimpleWords[mSimpleWordsIndx++].word;
            pushButton = new QPushButton(QString::fromStdString(word));
            connect(pushButton, &QPushButton::clicked, [this, pushButton](){
                pushButton->setStyleSheet("color: red");
                QTimer::singleShot(1000, this, &ChooseWord::skipWord);

            });
        }
        pushButton->setShortcut(QKeySequence(i + '1'));
        pushButton->setFixedSize(WINDOW_WIDTH * 0.6 , WINDOW_HEIGHT * 0.12);
        mWordVariantsButtons.push_back(pushButton);
        mWordButtonsVBoxLayout->addWidget(pushButton);
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