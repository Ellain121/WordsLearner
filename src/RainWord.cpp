#include "RainWord.hpp"

#include <QTimer>

#include <cassert>

RainWord::RainWord(std::vector<Word>& words, std::vector<SimpleWord>&& simpleWords, int trainingBit, QWidget* parent)
: Training(words, trainingBit, parent)
, mSimpleWords(simpleWords)
, mSimpleWordsIndx(0)
{
    assert(!mWords.empty());    

    setupCoreWidgets();
    setupCoreWidgetsConnections();
}

void RainWord::setupCoreWidgets()
{
    mMainVBoxLayout = new QVBoxLayout;
    mTranslationButtonsVBoxLayout = new QHBoxLayout;
    mWord = new QLabel;
    mTranslationVariantsButtons.resize(5);
    for (int i = 0; i < 5; ++i)
    {
        mTranslationVariantsButtons[i] = new WrapButton;
    }
    
    mBackButton = new QPushButton("Back");
    mAnimation = new QPropertyAnimation(mWord, "pos");

    mWord->setAttribute(Qt::WA_TranslucentBackground);
    mWord->setStatusTip("color: white");
    setupWordsAndTranslation();

    mBackButton->setFixedSize(WINDOW_WIDTH * 0.12 , WINDOW_HEIGHT * 0.08);

    mMainVBoxLayout->addWidget(mBackButton, 0, Qt::AlignLeft);
    mMainVBoxLayout->addStretch(1);
    mMainVBoxLayout->addStretch(1);
    mMainVBoxLayout->addLayout(mTranslationButtonsVBoxLayout);
    mMainVBoxLayout->addWidget(mWord, 0, Qt::AlignCenter);
    mTranslationButtonsVBoxLayout->setAlignment(Qt::AlignCenter);
    mMainVBoxLayout->addStretch(1);

    for (int i = 0; i < 5; ++i)
    {
        mTranslationButtonsVBoxLayout->addWidget(mTranslationVariantsButtons[i]);
    }

    connect(mAnimation, &QPropertyAnimation::finished, [&](){
        mWord->setStyleSheet("color: red");
        QTimer::singleShot(1000, this, &RainWord::skipWord);
    });

    setLayout(mMainVBoxLayout);
}

void RainWord::setupCoreWidgetsConnections()
{
    connect(mBackButton, &QPushButton::clicked, [this](){
        this->backToMenu();
    });
}

void RainWord::setupWordsAndTranslation()
{
    mAnimation->setDuration(10000);
    mAnimation->setStartValue(QPoint(WINDOW_WIDTH / 2, 10));
    mAnimation->setEndValue(QPoint(WINDOW_WIDTH / 2, 0.8 * WINDOW_HEIGHT));
    mAnimation->start();
    mWord->setStyleSheet("color: white");
    mWord->setText(QString::fromStdString(mWords[mWordIndx].word));
    mWord->raise();
    setupWordVariantsButtons();
}

void RainWord::setupWordVariantsButtons()
{
    int rightTranslationIndx = mRandEngine.getRandom(5);
    auto rightTranslation = mWords[mWordIndx].translation;
    for (int i = 0; i < 5; ++i)
    {
        WrapButton* pushButton = mTranslationVariantsButtons[i];
        pushButton->disconnect();
        if (i == rightTranslationIndx)
        {
            pushButton->setText(QString::fromStdString(rightTranslation));
            connect(pushButton, &QPushButton::clicked, [this, pushButton](){
                pushButton->setStyleSheet("color: limegreen");
                mAnimation->stop();
                QTimer::singleShot(700, this, &RainWord::nextWord);

            });
        }
        else
        {
            assert(mSimpleWordsIndx + 1 < mSimpleWords.size());
            auto& translation = mSimpleWords[mSimpleWordsIndx++].translation;
            pushButton->setText(QString::fromStdString(translation));
            connect(pushButton, &QPushButton::clicked, [this, pushButton](){
                pushButton->setStyleSheet("color: red");
                mAnimation->stop();
                QTimer::singleShot(1000, this, &RainWord::skipWord);

            });
        }
        pushButton->setStyleSheet("font-size: 15px;");
        pushButton->setShortcut(QKeySequence(i + '1'));
        pushButton->setFixedSize(WINDOW_WIDTH * 0.19 , WINDOW_HEIGHT * 0.12);
    }
}

void RainWord::nextWord()
{
    Training::nextWord();
    if (isTrainingDone())
    {
        for (auto& variantButton : mTranslationVariantsButtons)
        {
            variantButton->disconnect();            
        }
    }
    else
    {
        setupWordsAndTranslation();
    }
}

void RainWord::skipWord()
{
    Training::skipWord();
    setupWordsAndTranslation();  
}

TrainingType RainWord::getTrainingType() const
{
    return TrainingType::RainWord_Train;
}