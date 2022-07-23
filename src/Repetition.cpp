#include "Repetition.hpp"

#include <QTimer>

#include <cassert>

Repetition::Repetition(std::vector<Word>& words, std::vector<SimpleWord>&& simpleWords, QWidget* parent)
: Training(words, getTrainingStatusBit(TrainingType::Repetition_Train), parent)
, mSimpleWords(simpleWords)
, mSimpleWordsIndx(0)
{
    assert(!mWords.empty());

    setupCoreWidgets();
    setupCoreWidgetsConnections();
}

void Repetition::setupCoreWidgets()
{
    mMainVBoxLayout = new QVBoxLayout;
    mTranslationButtonsVBoxLayout = new QHBoxLayout;
    mWord = new QLabel;
    mBackButton = new QPushButton("Back");
    mSkipButton = new QPushButton("I don't know");

    mTranslationVariantsButtons.resize(2);
    for (int i = 0; i < 2; ++i)
    {
        mTranslationVariantsButtons[i] = new WrapButton;
    }

    mBackButton->setShortcut(QKeySequence(Qt::Key_Escape));
    mSkipButton->setShortcut(QKeySequence(Qt::Key_Space));

    // mWord->setStyleSheet("font-weight: bold; color: green");
    mWord->setStyleSheet("color: white;font-size: 40px;");
    setupWordsAndTranslation();

    mBackButton->setFixedSize(WINDOW_WIDTH * 0.12 , WINDOW_HEIGHT * 0.08);
    mSkipButton->setFixedSize(WINDOW_WIDTH * 0.17 , WINDOW_HEIGHT * 0.11);

    mMainVBoxLayout->addWidget(mBackButton, 0, Qt::AlignLeft);
    mMainVBoxLayout->addStretch(1);
    // mMainVBoxLayout->addWidget(mWord, 0, Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mWord, 0, Qt::AlignCenter);
    mMainVBoxLayout->addStretch(1);
    mTranslationButtonsVBoxLayout->addStretch(1);
    for (int i = 0; i < 2; ++i)
    {
        if (i == 1) mTranslationButtonsVBoxLayout->addStretch(1);
        mTranslationButtonsVBoxLayout->addWidget(mTranslationVariantsButtons[i]);
    }
    mTranslationButtonsVBoxLayout->addStretch(1);
    mMainVBoxLayout->addLayout(mTranslationButtonsVBoxLayout);
    mTranslationButtonsVBoxLayout->setAlignment(Qt::AlignCenter);
    mMainVBoxLayout->addStretch(1);
    mMainVBoxLayout->addWidget(mSkipButton, Qt::AlignCenter);

    setLayout(mMainVBoxLayout);
}

void Repetition::setupCoreWidgetsConnections()
{
    connect(mBackButton, &QPushButton::clicked, [this](){
        this->backToMenu();
    });
}

void Repetition::setupWordsAndTranslation()
{
    mWord->setText(QString::fromStdString(mWords[mWordIndx].word));
    setupTranslationVariantsButtons();
}

void Repetition::setupTranslationVariantsButtons()
{
    int rightTranslationIndx = mRandEngine.getRandom(2);
    auto rightTranslation = mWords[mWordIndx].translation;
    for (int i = 0; i < 2; ++i)
    {
        WrapButton* pushButton = mTranslationVariantsButtons[i];
        pushButton->disconnect();
        if (i == rightTranslationIndx)
        {
            // pushButton = new QPushButton(QString::fromStdString(rightTranslation));
            pushButton->setText(QString::fromStdString(rightTranslation));
            connect(pushButton, &QPushButton::clicked, [this, pushButton](){
                pushButton->setStyleSheet("color: limegreen; font-size: 18px;");
                QTimer::singleShot(700, this, &Repetition::nextWord);

            });
        }
        else
        {
            assert(mSimpleWordsIndx + 1 < mSimpleWords.size());
            auto& translation = mSimpleWords[mSimpleWordsIndx++].translation;
            // pushButton = new QPushButton(QString::fromStdString(translation));
            pushButton->setText(QString::fromStdString(translation));
            connect(pushButton, &QPushButton::clicked, [this, pushButton](){
                pushButton->setStyleSheet("color: red; font-size: 22px;");
                QTimer::singleShot(1000, this, &Repetition::skipWord);

            });
        }
        pushButton->setStyleSheet("font-size: 15px;");
        pushButton->setShortcut(QKeySequence(i == 0 ? Qt::Key_Left : Qt::Key_Right));
        pushButton->setFixedSize(WINDOW_WIDTH * 0.3 , WINDOW_HEIGHT * 0.12);
    }
}

void Repetition::nextWord()
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

void Repetition::skipWord()
{
    Training::skipWord();
    setupWordsAndTranslation();  
}

TrainingType Repetition::getTrainingType() const
{
    return TrainingType::ChooseWord_Train;
}