#include "ChooseTranslation.hpp"

#include <QTimer>

#include <cassert>

ChooseTranslation::ChooseTranslation(std::vector<Word>& words, std::vector<SimpleWord>&& simpleWords, QWidget* parent)
: Training(words, getTrainingStatusBit(TrainingType::ChooseTranslation_Train), parent)
, mSimpleWords(simpleWords)
, mSimpleWordsIndx(0)
{
    assert(!mWords.empty());

    setupCoreWidgets();
    setupCoreWidgetsConnections();
}

void ChooseTranslation::setupCoreWidgets()
{
    mMainVBoxLayout = new QVBoxLayout;
    mTranslationButtonsVBoxLayout = new QVBoxLayout;
    mButtonsHBoxLayout = new QHBoxLayout;
    mWord = new QLabel;
    mBackButton = new QPushButton("Back");
    mSkipButton = new QPushButton("I don't know");
    mNextButton = new QPushButton("Next");

    mBackButton->setShortcut(QKeySequence(Qt::Key_Escape));
    mSkipButton->setShortcut(QKeySequence(Qt::Key_Space));

    // mWord->setStyleSheet("font-weight: bold; color: green");
    mWord->setStatusTip("color: white");
    setupWordsAndTranslation();

    mBackButton->setFixedSize(WINDOW_WIDTH * 0.12 , WINDOW_HEIGHT * 0.08);
    mSkipButton->setFixedSize(WINDOW_WIDTH * 0.17 , WINDOW_HEIGHT * 0.11);
    mNextButton->setFixedSize(WINDOW_WIDTH * 0.17 , WINDOW_HEIGHT * 0.11);

    mMainVBoxLayout->addWidget(mBackButton, 0, Qt::AlignLeft);
    mMainVBoxLayout->addStretch(1);
    // mMainVBoxLayout->addWidget(mWord, 0, Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mWord, 0, Qt::AlignCenter);
    mMainVBoxLayout->addStretch(1);
    mMainVBoxLayout->addLayout(mTranslationButtonsVBoxLayout);
    mTranslationButtonsVBoxLayout->setAlignment(Qt::AlignCenter);
    mMainVBoxLayout->addStretch(1);
    mButtonsHBoxLayout->setAlignment(Qt::AlignCenter | Qt::AlignBottom);
    mButtonsHBoxLayout->addWidget(mSkipButton);
    mButtonsHBoxLayout->addStretch(1);
    mButtonsHBoxLayout->addWidget(mNextButton);
    mMainVBoxLayout->addLayout(mButtonsHBoxLayout);

    setLayout(mMainVBoxLayout);
}

void ChooseTranslation::setupCoreWidgetsConnections()
{
    connect(mBackButton, &QPushButton::clicked, [this](){
        this->backToMenu();
    });
}

void ChooseTranslation::setupWordsAndTranslation()
{
    mWord->setText(QString::fromStdString(mWords[mWordIndx].word));
    setupTranslationVariantsButtons();
}

void ChooseTranslation::setupTranslationVariantsButtons()
{
     // delete all previou letter buttons, clear vector, clear layout, clear indices
    mTranslationVariantsButtons.clear();
    QLayoutItem* item;
    while ( ( item = mTranslationButtonsVBoxLayout->takeAt( 0 ) ) != NULL )
    {
        // std::cout << "DELETED\n";
        delete item->widget();
        delete item;
    }
    //-----------------------------------------------------
    int rightTranslationIndx = mRandEngine.getRandom(5);
    auto rightTranslation = mWords[mWordIndx].translation;
    for (int i = 0; i < 5; ++i)
    {
        QPushButton* pushButton;
        if (i == rightTranslationIndx)
        {
            pushButton = new QPushButton(QString::fromStdString(rightTranslation));
            connect(pushButton, &QPushButton::clicked, [this, pushButton](){
                pushButton->setStyleSheet("color: limegreen");
                QTimer::singleShot(700, this, &ChooseTranslation::nextWord);

            });
        }
        else
        {
            assert(mSimpleWordsIndx + 1 < mSimpleWords.size());
            auto& translation = mSimpleWords[mSimpleWordsIndx++].translation;
            pushButton = new QPushButton(QString::fromStdString(translation));
            connect(pushButton, &QPushButton::clicked, [this, pushButton](){
                pushButton->setStyleSheet("color: red");
                QTimer::singleShot(1000, this, &ChooseTranslation::skipWord);

            });
        }
        pushButton->setShortcut(QKeySequence(i + '1'));
        pushButton->setFixedSize(WINDOW_WIDTH * 0.6 , WINDOW_HEIGHT * 0.12);
        mTranslationVariantsButtons.push_back(pushButton);
        mTranslationButtonsVBoxLayout->addWidget(pushButton);
    }
}

void ChooseTranslation::nextWord()
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

void ChooseTranslation::skipWord()
{
    Training::skipWord();
    setupWordsAndTranslation();  
}

TrainingType ChooseTranslation::getTrainingType() const
{
    return TrainingType::ChooseWord_Train;
}