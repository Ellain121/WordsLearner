#include "MakeWord.hpp"

#include <QTimer>
#include <QThread>

#include <cassert>

MakeWord::MakeWord(std::vector<Word>& words, QWidget* parent)
: Training(words, getTrainingStatusBit(TrainingType::MakeWord_Train), parent)
, mLetterIndx(0)
, mWordMistakesCnt(0)
{
    setupCoreWidgets();
    setupCoreWidgetsConnections();
}

void MakeWord::setupCoreWidgets()
{
    mMainVBoxLayout = new QVBoxLayout;
    mLettersHBoxLayout = new QHBoxLayout;
    mTranslationLabel = new QLabel;
    mWordLabel = new QLabel;
    mBackButton = new QPushButton("Back");

    mBackButton->setFixedSize(WINDOW_WIDTH * 0.12 , WINDOW_HEIGHT * 0.08);
    
    setupWordAndTranslation();

    mMainVBoxLayout->addWidget(mBackButton, 0, Qt::AlignLeft);
    mMainVBoxLayout->addStretch(1);
    mMainVBoxLayout->addWidget(mTranslationLabel, 0, Qt::AlignCenter);
    mMainVBoxLayout->addStretch(1);
    mMainVBoxLayout->addWidget(mWordLabel, 0, Qt::AlignCenter);
    mMainVBoxLayout->addStretch(1);
    mMainVBoxLayout->addLayout(mLettersHBoxLayout);
    mMainVBoxLayout->addStretch(1);

    setLayout(mMainVBoxLayout);
}

void MakeWord::setupCoreWidgetsConnections()
{
    connect(mBackButton, &QPushButton::clicked, [&](){
        emit backToMenu();
    });
}

void MakeWord::setupWordAndTranslation()
{
    mWordLabel->setStyleSheet("color: white");
    mTranslationLabel->setText(QString::fromStdString(mWords[mWordIndx].translation));
    mWordLabel->setText(" ");
    setupWordButtons();
    setupWordButtonsConnections();
}

void MakeWord::nextWord()
{
    Training::nextWord();
    if (isTrainingDone())
    {
        for (auto& lButton : mWordButtons)
        {
            lButton->disconnect();
        }
    }
    else
    {
        setupWordAndTranslation();
    }
}

void MakeWord::skipWord()
{
    Training::skipWord();
    setupWordAndTranslation();
}

bool MakeWord::letterButtonPressed(char ch)
{
    auto& word = mWords[mWordIndx].word;
    assert(mLetterIndx < word.size());

    if (word[mLetterIndx] == ch)
    {
        mWordLabel->setText(mWordLabel->text() + ch);
        mLetterIndx++;
        return true;
    }
    return false;
}

void MakeWord::setupWordButtons()
{
    // delete all previou letter buttons, clear vector, clear layout, clear indices
    mWordButtons.clear();
    QLayoutItem* item;
    while ( ( item = mLettersHBoxLayout->takeAt( 0 ) ) != NULL )
    {
        // std::cout << "DELETED\n";
        delete item->widget();
        delete item;
    }
    mLetterIndx = 0;
    mWordMistakesCnt = 0;
    //-----------------------------------------------------

    auto& word = mWords[mWordIndx].word;
    int alpha[256] = {0};
    for (auto& ch : word)
    {
        // recheck probably space also true in isprint
        if (isprint(ch) || ch == ' ')
        {
            alpha[ch]++;
        }
    }
    mLettersHBoxLayout->addStretch(3);
    for (int i = 0; i < 256; ++i)
    {
        if (alpha[i] > 0)
        {
            LetterButton* charButton = new LetterButton(QChar(i), alpha[i]);
            mWordButtons.push_back(charButton);
            charButton->setFixedSize(WINDOW_WIDTH * 0.1, WINDOW_WIDTH * 0.1);
            charButton->setShortcut(QKeySequence(toupper(i)));
            mLettersHBoxLayout->addWidget(charButton, 1, Qt::AlignCenter);
        }
    }
    mLettersHBoxLayout->addStretch(3);
}

void MakeWord::setupWordButtonsConnections()
{
    for (auto& lButton : mWordButtons)
    {   
        connect(lButton, &QPushButton::clicked, [&](){
            if (mWordMistakesCnt >= 2) return;

            // if right letterButton
            if (letterButtonPressed(lButton->text()[0].toLatin1()))
            {
                lButton->handleCorrectClick();
                if (mLetterIndx == mWords[mWordIndx].word.size())
                {
                    mWordLabel->setStyleSheet("color: limegreen");
                    QTimer::singleShot(700, this, &MakeWord::nextWord);
                }
            }
            else 
            {
                mWordMistakesCnt++;
                // word failed, go to the next one if they're exist
                if (mWordMistakesCnt >= 2)
                {
                    // skipWord();
                    mWordLabel->setText(QString::fromStdString(mWords[mWordIndx].word));
                    mWordLabel->setStyleSheet("color: red");
                    QTimer::singleShot(1500, this, &MakeWord::skipWord);
                }
                // if mistake, then mark letterButton red for 500 ms and switch 
                // back to white afterwards
                lButton->setColor(getQColor(Color::Red));
                lButton->update();
                lButton->repaint();
                QTimer::singleShot(500, [&](){
                    lButton->setColor(getQColor(Color::White));
                    lButton->update();
                    lButton->repaint(); 
                });
            }
        });
    }
}

TrainingType MakeWord::getTrainingType() const
{
    return TrainingType::MakeWord_Train;
}