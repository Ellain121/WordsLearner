#include "MainWinow.hpp"
#include "WordsWelcome.hpp"
#include "MakeWord.hpp"
#include "ChooseWord.hpp"
#include "ChooseTranslation.hpp"
#include "RainWord.hpp"
#include "DictionaryState.hpp"

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

    mMainVBoxLayout->addStretch(1);
    auto wordsAmountForTrains = mDBManager.getWordsAmountForEachTraining();
    int repetitionN = wordsAmountForTrains[TrainingType::Repetition_Train];
    int initialN = wordsAmountForTrains[TrainingType::Initial_Train];
    int makeWordN = wordsAmountForTrains[TrainingType::MakeWord_Train];
    int chooseWordN = wordsAmountForTrains[TrainingType::ChooseWord_Train];
    int chooseTranslationN = wordsAmountForTrains[TrainingType::ChooseTranslation_Train];
    int rainWordN = wordsAmountForTrains[TrainingType::RainWord_Train];

    mRepetitionButton = new QPushButton(QString("Repetition (") + QString::number(repetitionN) + QString(")"));
    mInitialTrainingButton = new QPushButton(QString("Initial Training (") + QString::number(initialN) + QString(")"));
    mMakeWordButton = new QPushButton(QString("Make Word (") + QString::number(makeWordN) + QString(")"));
    mChooseWordButton = new QPushButton(QString("Choose Word (") + QString::number(chooseWordN) + QString(")"));
    mChooseTranslationButton = new QPushButton(QString("Choose Translation (") + QString::number(chooseTranslationN) + QString(")"));
    mRainWordButton = new QPushButton(QString("Words Rain (") + QString::number(rainWordN) + QString(")"));
    mDictionaryButton = new QPushButton("Dictionary");

    mRepetitionButton->setFixedSize(0.8 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);
    mInitialTrainingButton->setFixedSize(0.8 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);
    mMakeWordButton->setFixedSize(0.8 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);
    mChooseWordButton->setFixedSize(0.8 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);
    mChooseTranslationButton->setFixedSize(0.8 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);
    mRainWordButton->setFixedSize(0.8 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);
    mDictionaryButton->setFixedSize(0.8 * WINDOW_WIDTH, 0.1 * WINDOW_HEIGHT);

    mMainVBoxLayout->setAlignment(Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mRepetitionButton, 0,   Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mInitialTrainingButton, 0,  Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mMakeWordButton, 0, Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mChooseWordButton, 0, Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mChooseTranslationButton, 0, Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mRainWordButton, 0, Qt::AlignCenter);
    mMainVBoxLayout->addWidget(mDictionaryButton, 0, Qt::AlignCenter);
    mMainWidget->setLayout(mMainVBoxLayout);
    setCentralWidget(mMainWidget);
}

void MainWindow::setupCoreWidgetsConnections()
{
    connect(mInitialTrainingButton, &QPushButton::clicked, this, &MainWindow::initInitialTraining);
    connect(mMakeWordButton, &QPushButton::clicked, this, &MainWindow::initMakeWordTraining);
    connect(mChooseWordButton, &QPushButton::clicked, this, &MainWindow::initChooseWordTraining);
    connect(mChooseTranslationButton, &QPushButton::clicked, this, &MainWindow::initChooseTranslation);
    connect(mRainWordButton, &QPushButton::clicked, this, &MainWindow::initRainWord);
    connect(mDictionaryButton, &QPushButton::clicked, this, &MainWindow::initDictionaryState);
}

void MainWindow::initInitialTraining()
{
    mCurrentWordsToLearn = mDBManager.generateWordsForTraining(TrainingType::Initial_Train);

    WordsWelcome* wordsWelcome = new WordsWelcome(mCurrentWordsToLearn);
    MakeWord* makeWord = new MakeWord(mCurrentWordsToLearn);
    ChooseWord* chooseWord = new ChooseWord(mCurrentWordsToLearn, mDBManager.generateRandomWords(TrainingType::ChooseWord_Train));
    ChooseTranslation* chooseTranslation = new ChooseTranslation(mCurrentWordsToLearn, mDBManager.generateRandomWords(TrainingType::ChooseTranslation_Train));

    mMultiple.trains.resize(4);
    mMultiple.trains[0] = wordsWelcome;
    mMultiple.trains[1] = makeWord;
    mMultiple.trains[2] = chooseWord;
    mMultiple.trains[3] = chooseTranslation;


    mMainStackedWidget = new QStackedWidget;
    mMainStackedWidget->addWidget(wordsWelcome);
    mMainStackedWidget->addWidget(makeWord);
    mMainStackedWidget->addWidget(chooseWord);
    mMainStackedWidget->addWidget(chooseTranslation);


    mMultiple.trainsDone.clear();
    mMultiple.trainsDone = std::vector<bool>(4, false);

    for (int i = 0; i < mMultiple.trains.size(); ++i)
    {
        auto train = mMultiple.trains[i];
        connect(train, &Training::fullCircle, [&](){
            int indx = mMainStackedWidget->currentIndex();
            int size = mMainStackedWidget->count();
            do {
                indx = (indx + 1) % size;
            } while (mMultiple.trainsDone[indx] != false);

            mMainStackedWidget->setCurrentIndex(indx);
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
                int indx = mMainStackedWidget->currentIndex();
                int size = mMainStackedWidget->count();
                do {
                    indx = (indx + 1) % size;
                } while (mMultiple.trainsDone[indx] != false);
                mMainStackedWidget->setCurrentIndex(indx);
            }
        });

        connect(train, &Training::backToMenu, this, &MainWindow::activateMenu);
    }

    setCentralWidget(mMainStackedWidget);
}

void MainWindow::initMakeWordTraining()
{
    mCurrentWordsToLearn = mDBManager.generateWordsForTraining(TrainingType::MakeWord_Train);

    MakeWord* makeWord = new MakeWord(mCurrentWordsToLearn);

    connect(makeWord, &Training::trainingDone, [&](){
        // makeWord->getTrainingType();
        this->activeTrainingDone(TrainingType::MakeWord_Train);
    });

    connect(makeWord, &Training::backToMenu, this, &MainWindow::activateMenu);

    mMainStackedWidget = new QStackedWidget;
    mMainStackedWidget->addWidget(makeWord);
    setCentralWidget(mMainStackedWidget);
}

void MainWindow::initChooseWordTraining()
{
    mCurrentWordsToLearn = mDBManager.generateWordsForTraining(TrainingType::ChooseWord_Train);

    ChooseWord* chooseWord = new ChooseWord(mCurrentWordsToLearn, mDBManager.generateRandomWords(TrainingType::ChooseWord_Train));

    connect(chooseWord, &Training::trainingDone, [&](){
        this->activeTrainingDone(TrainingType::ChooseWord_Train);
    });
    connect(chooseWord, &Training::backToMenu, this, &MainWindow::activateMenu);

    mMainStackedWidget = new QStackedWidget;
    mMainStackedWidget->addWidget(chooseWord);
    setCentralWidget(mMainStackedWidget);
}

void MainWindow::initChooseTranslation()
{
    mCurrentWordsToLearn = mDBManager.generateWordsForTraining(TrainingType::ChooseTranslation_Train);

    ChooseTranslation* chooseTranslation = new ChooseTranslation(mCurrentWordsToLearn, mDBManager.generateRandomWords(TrainingType::ChooseTranslation_Train));

    connect(chooseTranslation, &Training::trainingDone, [&](){
        this->activeTrainingDone(TrainingType::ChooseTranslation_Train);
    });
    connect(chooseTranslation, &Training::backToMenu, this, &MainWindow::activateMenu);

    mMainStackedWidget = new QStackedWidget;
    mMainStackedWidget->addWidget(chooseTranslation);
    setCentralWidget(mMainStackedWidget);
}

void MainWindow::initRainWord()
{
    mCurrentWordsToLearn = mDBManager.generateWordsForTraining(TrainingType::RainWord_Train);

    RainWord* rainWord = new RainWord(mCurrentWordsToLearn, mDBManager.generateRandomWords(TrainingType::RainWord_Train));

    connect(rainWord, &Training::trainingDone, [&](){
        this->activeTrainingDone(TrainingType::RainWord_Train);
    });
    connect(rainWord, &Training::backToMenu, this, &MainWindow::activateMenu);

    mMainStackedWidget = new QStackedWidget;
    mMainStackedWidget->addWidget(rainWord);
    setCentralWidget(mMainStackedWidget);
}

void MainWindow::initDictionaryState()
{
    DictionaryState* dictState = new DictionaryState(&mDBManager);

    connect(dictState, &DictionaryState::backToMenu, this, &MainWindow::activateMenu);

    mMainStackedWidget = new QStackedWidget;
    mMainStackedWidget->addWidget(dictState);
    setCentralWidget(mMainStackedWidget);
}

void MainWindow::activeTrainingDone(TrainingType trainType)
{
    mDBManager.writeChanges(mCurrentWordsToLearn);
    activateMenu();
}

void MainWindow::activateMenu()
{
    mCurrentWordsToLearn.clear();
    setupCoreWidgets();
    setupCoreWidgetsConnections();
    // delete mMainStackedWidget;
    // mMainStackedWidget = new QStackedWidget;
}