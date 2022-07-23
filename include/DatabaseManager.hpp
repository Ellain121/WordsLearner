#ifndef DATABASEMANAGER_HPP
#define DATABASEMANAGER_HPP

#include "Utility.hpp"
#include "NonCopyable.hpp"

#include <QSqlDatabase>

#include <vector>
#include <map>

class DatabaseManager : private NonCopyable
{
    public:
        DatabaseManager();
        ~DatabaseManager();

        std::map<TrainingType, int>   getWordsAmountForEachTraining() const;

        std::vector<Word>       generateWordsForTraining(TrainingType trainType) const;
        std::vector<SimpleWord> generateRandomWords(TrainingType trainType) const;

        void                writeChanges(const std::vector<Word>& words);


        QSqlDatabase&               getDBConnection();
        const std::string&          getDBName() const;
        const std::string&          getTableName() const;
        int                         countFullyLearnedWords() const;
        int                         countWords() const;
    
    private:
        void                updateWordsWithRepetitionDate();
        
    
    private:
        QSqlDatabase           mDBConnection;
        const std::string      mDBName;
        const std::string      mTableName;
};  

#endif