#include "MainWinow.hpp"
#include "WordsWelcome.hpp"
#include "MakeWord.hpp"
#include "ChooseWord.hpp"
#include "ChooseTranslation.hpp"
#include "RainWord.hpp"
#include "DictionaryState.hpp"
#include "Repetition.hpp"

#include <QFile>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
, mDBManager()
{
    QFile file("../data/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    setStyleSheet(styleSheet);
    setWindowTitle("WordsLearner v0.1");
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    setupCoreWidgets();
    setupCoreWidgetsConnections();
}

void MainWindow::setupCoreWidgets()
{
    mMainWidget = new QWidget;
    mMainVBoxLayout = new QVBoxLayout;
    mStatisticsWidget = new StatisticsWidget(&mDBManager);
    mMainStackedWidget = new QStackedWidget;

    mMainVBoxLayout->addWidget(mStatisticsWidget, Qt::AlignCenter);
    mMainVBoxLayout->addStretch(1);

    mRepetitionButton = new QPushButton;
    mInitialTrainingButton = new QPushButton;
    mMakeWordButton = new QPushButton;
    mChooseWordButton = new QPushButton;
    mChooseTranslationButton = new QPushButton;
    mRainWordButton = new QPushButton;
    mDictionaryButton = new QPushButton;
    updateButtonsText();

    mRepetitionButton->setFixedSize(0.8 * WINDOW_WIDTH, 0.09 * WINDOW_HEIGHT);
    mInitialTrainingButton->setFixedSize(0.8 * WINDOW_WIDTH, 0.09 * WINDOW_HEIGHT);
    mMakeWordButton->setFixedSize(0.8 * WINDOW_WIDTH, 0.09 * WINDOW_HEIGHT);
    mChooseWordButton->setFixedSize(0.8 * WINDOW_WIDTH, 0.09 * WINDOW_HEIGHT);
    mChooseTranslationButton->setFixedSize(0.8 * WINDOW_WIDTH, 0.09 * WINDOW_HEIGHT);
    mRainWordButton->setFixedSize(0.8 * WINDOW_WIDTH, 0.09 * WINDOW_HEIGHT);
    mDictionaryButton->setFixedSize(0.8 * WINDOW_WIDTH, 0.09 * WINDOW_HEIGHT);

    mMainVBoxLayout->setAlignment(Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mRepetitionButton, 0,   Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mInitialTrainingButton, 0,  Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mMakeWordButton, 0, Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mChooseWordButton, 0, Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mChooseTranslationButton, 0, Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mRainWordButton, 0, Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mDictionaryButton, 0, Qt::AlignCenter);
    mMainWidget->setLayout(mMainVBoxLayout);
    mMainStackedWidget->addWidget(mMainWidget);
    
    setCentralWidget(mMainStackedWidget);
}

void MainWindow::setupCoreWidgetsConnections()
{
    connect(mRepetitionButton, &QPushButton::clicked, this, &MainWindow::initRepetitionTraining);
    connect(mInitialTrainingButton, &QPushButton::clicked, this, &MainWindow::initInitialTraining);
    connect(mMakeWordButton, &QPushButton::clicked, this, &MainWindow::initMakeWordTraining);
    connect(mChooseWordButton, &QPushButton::clicked, this, &MainWindow::initChooseWordTraining);
    connect(mChooseTranslationButton, &QPushButton::clicked, this, &MainWindow::initChooseTranslation);
    connect(mRainWordButton, &QPushButton::clicked, this, &MainWindow::initRainWord);
    connect(mDictionaryButton, &QPushButton::clicked, this, &MainWindow::initDictionaryState);
}

void MainWindow::updateButtonsText()
{
    auto wordsAmountForTrains = mDBManager.getWordsAmountForEachTraining();
    int repetitionN = wordsAmountForTrains[TrainingType::Repetition_Train];
    int initialN = wordsAmountForTrains[TrainingType::Initial_Train];
    int makeWordN = wordsAmountForTrains[TrainingType::MakeWord_Train];
    int chooseWordN = wordsAmountForTrains[TrainingType::ChooseWord_Train];
    int chooseTranslationN = wordsAmountForTrains[TrainingType::ChooseTranslation_Train];
    int rainWordN = wordsAmountForTrains[TrainingType::RainWord_Train];

    mRepetitionButton->setText(QString("Repetition (") + QString::number(repetitionN) + QString(")"));
    mInitialTrainingButton->setText(QString("Initial Training (") + QString::number(initialN) + QString(")"));
    mMakeWordButton->setText(QString("Make Word (") + QString::number(makeWordN) + QString(")"));
    mChooseWordButton->setText(QString("Choose Word (") + QString::number(chooseWordN) + QString(")"));
    mChooseTranslationButton->setText(QString("Choose Translation (") + QString::number(chooseTranslationN) + QString(")"));
    mRainWordButton->setText(QString("Words Rain (") + QString::number(rainWordN) + QString(")"));
    mDictionaryButton->setText("Dictionary");
}

void MainWindow::initRepetitionTraining()
{
    mCurrentWordsToLearn = mDBManager.generateWordsForTraining(TrainingType::Repetition_Train);
    if (mCurrentWordsToLearn.size() < 1) return;

    Repetition* repetitionTrain = new Repetition(mCurrentWordsToLearn, mDBManager.generateRandomWords(TrainingType::Repetition_Train));

    connect(repetitionTrain, &Training::trainingDone, [&](){
        // makeWord->getTrainingType();
        this->activeTrainingDone(TrainingType::Repetition_Train);
    });

    connect(repetitionTrain, &Training::backToMenu, this, &MainWindow::activateMenu);

    mMainStackedWidget->addWidget(repetitionTrain);
    mMainStackedWidget->setCurrentIndex(1);
}

void MainWindow::initInitialTraining()
{
    mCurrentWordsToLearn = mDBManager.generateWordsForTraining(TrainingType::Initial_Train);
    if (mCurrentWordsToLearn.size() < 1) return;

    WordsWelcome* wordsWelcome = new WordsWelcome(mCurrentWordsToLearn);
    ChooseWord* chooseWord = new ChooseWord(mCurrentWordsToLearn, mDBManager.generateRandomWords(TrainingType::ChooseWord_Train));
    ChooseTranslation* chooseTranslation = new ChooseTranslation(mCurrentWordsToLearn, mDBManager.generateRandomWords(TrainingType::ChooseTranslation_Train));
    MakeWord* makeWord = new MakeWord(mCurrentWordsToLearn);

    mMultiple.trains.resize(4);
    mMultiple.trains[0] = wordsWelcome;
    mMultiple.trains[1] = chooseWord;
    mMultiple.trains[2] = chooseTranslation;
    mMultiple.trains[3] = makeWord;

    for (auto& train : mMultiple.trains)
    {
        mMainStackedWidget->addWidget(train);
    }

    mMultiple.trainsDone.clear();
    mMultiple.trainsDone = std::vector<bool>(4, false);
    mMultiple.indx = 0;

    for (int i = 0; i < mMultiple.trains.size(); ++i)
    {
        auto train = mMultiple.trains[i];
        connect(train, &Training::fullCircle, [&](){
            int& indx = mMultiple.indx;
            int size = mMultiple.trains.size();
            do {
                indx = (indx + 1) % size;
            } while (mMultiple.trainsDone[indx] != false);

            mMainStackedWidget->setCurrentIndex(indx + 1);
        });

        connect(train, &Training::trainingDone, [&, i](){
            mMultiple.trainsDone[i] = true;
            bool isAllDone = true;
            for (bool done : mMultiple.trainsDone)
            {
                if (!done)
                {
                    isAllDone = false;
                    break;
                }
            }
            if (isAllDone)
            {
                this->activeTrainingDone(TrainingType::Initial_Train);
            }
            else
            {
                int& indx = mMultiple.indx;
                int size = mMultiple.trains.size();
                do {
                    indx = (indx + 1) % size;
                } while (mMultiple.trainsDone[indx] != false);
                mMainStackedWidget->setCurrentIndex(indx + 1);
            }
        });

        connect(train, &Training::backToMenu, this, &MainWindow::activateMenu);
    }

    mMainStackedWidget->setCurrentIndex(1);
}

void MainWindow::initMakeWordTraining()
{
    mCurrentWordsToLearn = mDBManager.generateWordsForTraining(TrainingType::MakeWord_Train);
    if (mCurrentWordsToLearn.size() < 1) return;

    MakeWord* makeWord = new MakeWord(mCurrentWordsToLearn);

    connect(makeWord, &Training::trainingDone, [&](){
        // makeWord->getTrainingType();
        this->activeTrainingDone(TrainingType::MakeWord_Train);
    });

    connect(makeWord, &Training::backToMenu, this, &MainWindow::activateMenu);

    mMainStackedWidget->addWidget(makeWord);
    mMainStackedWidget->setCurrentIndex(1);
}

void MainWindow::initChooseWordTraining()
{
    mCurrentWordsToLearn = mDBManager.generateWordsForTraining(TrainingType::ChooseWord_Train);
    if (mCurrentWordsToLearn.size() < 1) return;

    ChooseWord* chooseWord = new ChooseWord(mCurrentWordsToLearn, mDBManager.generateRandomWords(TrainingType::ChooseWord_Train));

    connect(chooseWord, &Training::trainingDone, [&](){
        this->activeTrainingDone(TrainingType::ChooseWord_Train);
    });
    connect(chooseWord, &Training::backToMenu, this, &MainWindow::activateMenu);

    mMainStackedWidget->addWidget(chooseWord);
    mMainStackedWidget->setCurrentIndex(1);
}

void MainWindow::initChooseTranslation()
{
    mCurrentWordsToLearn = mDBManager.generateWordsForTraining(TrainingType::ChooseTranslation_Train);
    if (mCurrentWordsToLearn.size() < 1) return;

    ChooseTranslation* chooseTranslation = new ChooseTranslation(mCurrentWordsToLearn, mDBManager.generateRandomWords(TrainingType::ChooseTranslation_Train));

    connect(chooseTranslation, &Training::trainingDone, [&](){
        this->activeTrainingDone(TrainingType::ChooseTranslation_Train);
    });
    connect(chooseTranslation, &Training::backToMenu, this, &MainWindow::activateMenu);

    mMainStackedWidget->addWidget(chooseTranslation);
    mMainStackedWidget->setCurrentIndex(1);
}

void MainWindow::initRainWord()
{
    mCurrentWordsToLearn = mDBManager.generateWordsForTraining(TrainingType::RainWord_Train);
    if (mCurrentWordsToLearn.size() < 1) return;

    RainWord* rainWord = new RainWord(mCurrentWordsToLearn, mDBManager.generateRandomWords(TrainingType::RainWord_Train));

    connect(rainWord, &Training::trainingDone, [&](){
        this->activeTrainingDone(TrainingType::RainWord_Train);
    });
    connect(rainWord, &Training::backToMenu, this, &MainWindow::activateMenu);

    mMainStackedWidget->addWidget(rainWord);
    mMainStackedWidget->setCurrentIndex(1);
}

void MainWindow::initDictionaryState()
{
    DictionaryState* dictState = new DictionaryState(&mDBManager);

    connect(dictState, &DictionaryState::backToMenu, this, &MainWindow::activateMenu);
    connect(dictState, &DictionaryState::wordDeletedOrReset, mStatisticsWidget, &StatisticsWidget::wordDeletedOrReset);
    connect(dictState, &DictionaryState::newWord, mStatisticsWidget, &StatisticsWidget::newWordAdded);

    mMainStackedWidget->addWidget(dictState);
    mMainStackedWidget->setCurrentIndex(1);
}

void MainWindow::activeTrainingDone(TrainingType trainType)
{
    // CRUTCH LMAO
    int cnt = 0;
    for (auto& mCWTL : mCurrentWordsToLearn)
    {
        if (isFullyLearned(mCWTL.status))
        {
            mCWTL.status |= (0b1 << (int)getTrainingStatusBit(TrainingType::Repetition_Train));
            cnt++;
        }
        
    }

    mDBManager.writeChanges(mCurrentWordsToLearn);
    activateMenu();

    mStatisticsWidget->updateStatistics(cnt);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    delete mMainStackedWidget;
    QWidget::closeEvent(event);
}

void MainWindow::activateMenu()
{
    mCurrentWordsToLearn.clear();
    // mMultiple.trains.clear();

    while (mMainStackedWidget->count() > 1)
    {
        QWidget* widget = mMainStackedWidget->widget(1);
        mMainStackedWidget->removeWidget(widget);
        widget->deleteLater();
    }

    updateButtonsText();
    mMainStackedWidget->setCurrentIndex(0);
}