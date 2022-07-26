#include "DatabaseManager.hpp"

#include "Utility.hpp"

#include <QtSql>
#include <QDebug>

#include <vector>
#include <list>
#include <string>
#include <cassert>
#include <algorithm>

#define DATABASE_NAME "../data/dictionary.sqlite"
#define MAIN_TABLE_NAME "dictionary"

DatabaseManager::DatabaseManager()
: mDBName(DATABASE_NAME)
, mTableName(MAIN_TABLE_NAME)
{
    mDBConnection = QSqlDatabase::addDatabase("QSQLITE", "connection_1");
    mDBConnection.setDatabaseName(DATABASE_NAME);
    if (!mDBConnection.open())
    {
        qDebug() << "Cannot open database!";
    }

    QSqlQuery query(mDBConnection);
    QString createTableCmd = "CREATE TABLE IF NOT EXISTS " MAIN_TABLE_NAME " ("
            "Word VARCHAR(100), "
            "Translation TEXT, "
            "Status integer, "
            "'Added date' VARCHAR(11), "
            "'Learned date' VARCHAR(11), "
            "'Repetition time' integer);";

    if (!query.exec(createTableCmd))
    {
        qDebug() << "Cannot create table!\n";
        qDebug() << query.lastError();
    }

    updateWordsWithRepetitionDate();
}

DatabaseManager::~DatabaseManager()
{
    mDBConnection.close();
    mDBConnection = QSqlDatabase();
    QSqlDatabase::removeDatabase("connection_1");
}

void DatabaseManager::writeChanges(const std::vector<Word>& words)
{
    QSqlQuery update_statement(mDBConnection);
    update_statement.prepare(QString("update " MAIN_TABLE_NAME " set Status=:newStatus,'Learned date'=:learnedDate WHERE Word=:word"));
    for (auto& word : words)
    {
        bool fullyLearned = isFullyLearned(word.status);

        update_statement.bindValue(":newStatus", word.status);
        update_statement.bindValue(":learnedDate", (fullyLearned? Time::getCurrentTimeStr() : QString("-")));
        update_statement.bindValue(":word", QString::fromStdString(word.word));
        if (!update_statement.exec())
        {
            qDebug() << "Cannot update word: " << QString::fromStdString(word.word);
            qDebug() << update_statement.lastError();
        }
        
    }
}

void DatabaseManager::updateWordsWithRepetitionDate()
{
    QString cmd = QString("SELECT * FROM " MAIN_TABLE_NAME";");
    QSqlQuery statement(cmd, mDBConnection);
    QSqlRecord record = statement.record();
    QSqlQuery update_statement(mDBConnection);
    update_statement.prepare(QString("update " MAIN_TABLE_NAME " set Status=:newStatus,'Repetition time'=:repTime WHERE Word=:word"));
    while (statement.next())
    {
        QString word = statement.value(record.indexOf("Word")).toString();
        QString learnDate = statement.value(record.indexOf("Learned date")).toString();
        int status = statement.value(record.indexOf("Status")).toInt();
        if ( !(status & (0b1 << getTrainingStatusBit(TrainingType::Repetition_Train))) || learnDate.size() != 11) continue;

        Time learnedTime(learnDate);
        int repTimeH = statement.value(record.indexOf("Repetition time")).toInt();

        learnedTime += repTimeH;
        // std::cout << Time::getCurrentTimeStr().toStdString() << " > " << learnedTime.getStr().toStdString() << std::endl;
        if (Time::getCurrentTime() > learnedTime)
        {
            // std::cout << "inside!\n";
            repTimeH *= 2;
            status &= ~(0b1 << getTrainingStatusBit(TrainingType::Repetition_Train));

            update_statement.bindValue(":newStatus", status);
            update_statement.bindValue(":repTime", repTimeH);
            update_statement.bindValue(":word", word);
            if (!update_statement.exec())
            {
                qDebug() << "Cannot update REPETITION word: " << word;
                qDebug() << update_statement.lastError();
            }
        }
    }
}

std::map<TrainingType, int> DatabaseManager::getWordsAmountForEachTraining() const
{
    std::map<TrainingType, int> results;

    QString cmd = QString("SELECT * FROM " MAIN_TABLE_NAME ";");
    QSqlQuery statement(cmd, mDBConnection);
    QSqlRecord record = statement.record();
    while (statement.next())
    {
        int status = statement.value(record.indexOf("Status")).toInt();
        QString learnDate = statement.value(record.indexOf("Learned date")).toString();
        auto requiredTrainings = getRequiredTrainingsFromStatus(status);
        for (auto& requiredTrain : requiredTrainings)
        {
            results[requiredTrain]++;
        }
        results[TrainingType::Repetition_Train] += 
                (learnDate.size() == 11 && !(status & (0b1 << getTrainingStatusBit(TrainingType::Repetition_Train))));
    }
    return results;
}

std::vector<Word> DatabaseManager::generateWordsForTraining(TrainingType trainType) const
{
    std::vector<Word> words;

    int statusBit = getTrainingStatusBit(trainType);
    int wAmount = 10;
    if (trainType == TrainingType::RainWord_Train) wAmount = 30;
    if (trainType == TrainingType::Initial_Train) wAmount = 6;
    // WHERE SomeCol & (1 << 5) != 0;
    QString cmd = QString("SELECT * FROM " MAIN_TABLE_NAME " WHERE Status & (1 <<")
                               + QString::number(statusBit) + QString(") == 0;");
    QSqlQuery statement(cmd, mDBConnection);
    QSqlRecord record = statement.record();

    // for repetition training there must be status bit + word must be already learned
    if (trainType == TrainingType::Repetition_Train)
    {
        while (statement.next() && words.size() < wAmount)
        {
            QString learnDate = statement.value(record.indexOf("Learned date")).toString();
            if (learnDate.size() != 11) continue;

            std::string word = statement.value(record.indexOf("Word")).toString().toStdString();
            std::string meaning = statement.value(record.indexOf("Translation")).toString().toStdString();
            int status = statement.value(record.indexOf("Status")).toInt();
            
            words.push_back(Word(std::move(word), std::move(meaning), status));
        }
    }
    else
    {
        while (statement.next() && words.size() < wAmount)
        {
            std::string word = statement.value(record.indexOf("Word")).toString().toStdString();
            std::string meaning = statement.value(record.indexOf("Translation")).toString().toStdString();
            int status = statement.value(record.indexOf("Status")).toInt();
            
            words.push_back(Word(std::move(word), std::move(meaning), status));
        }
    }
    return words;
}
std::vector<SimpleWord> DatabaseManager::generateRandomWords(TrainingType trainType) const
{
    auto rWords = get120SimpleWords();

    RandomEngine rng;
    std::random_shuffle(rWords.begin(), rWords.end(), rng);

    return rWords;
}

void func(int trainStatus)
{
    std::list<Word> allWordsList;
    std::vector<Word> allWords;
    std::vector<Word> toLearn;

    // for (auto& word : allWordsList)
    for (auto itr = allWordsList.begin(); itr != allWordsList.end();)
    {
        Word& word = *itr;
        if (word.status & (0b1 << trainStatus))
        {
            toLearn.push_back(word);
            itr = allWordsList.erase(itr);
        }
        else
        {
            ++itr;
        }
    }
    for (auto& word : allWordsList)
    {
        allWords.push_back(word);
    }

    if (allWordsList.size() < 120)
    {
        // fill(allWordsList);
    }
    assert(allWordsList.size() > 120);
    RandomEngine rng;
    std::random_shuffle(allWords.begin(), allWords.end(), rng);
}

int DatabaseManager::countWords() const
{
    QSqlQuery statement("select count(1) from " MAIN_TABLE_NAME ";", mDBConnection);
    statement.next();
    int cnt = statement.value(statement.record().indexOf("count(1)")).toString().toInt();

    return cnt;
}

int DatabaseManager::countFullyLearnedWords() const
{
    int statusAll = TrainingType::All;
    QSqlQuery statement(QString("select count(1) from ") + MAIN_TABLE_NAME 
                                + QString(" where Status =") + QString::number(statusAll) + ";", mDBConnection);
    statement.next();
    int cnt = statement.value(statement.record().indexOf("count(1)")).toString().toInt();

    return cnt;
}

const std::string& DatabaseManager::getDBName() const
{
    return mDBName;
}

const std::string& DatabaseManager::getTableName() const
{
    return mTableName;
}

QSqlDatabase& DatabaseManager::getDBConnection()
{
    return mDBConnection;
}