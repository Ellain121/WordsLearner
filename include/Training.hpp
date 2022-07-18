#ifndef TRAINING_HPP
#define TRAINING_HPP

#include "Utility.hpp"

#include <QWidget>

class Training : public QWidget
{
    Q_OBJECT

    public:
        Training(std::vector<Word>& words, int statusBit, QWidget* parent = 0);

        virtual TrainingType    getTrainingType() const = 0;


    signals:
        void        fullCircle();
        void        trainingDone();
        void        backToMenu();

    private:
        virtual void    setupCoreWidgets() = 0;
        virtual void    setupCoreWidgetsConnections() = 0;

        bool            isAllWordsDone() const;

    protected:
        bool            isTrainingDone() const;
    
    protected slots:
        void            nextWord();
        void            skipWord();
    
    protected:
        std::vector<Word>&      mWords;
        std::vector<int>        mWordsTriesCnt;
        int                     mWordIndx;
        const int               mStatusBit;
    
    private:
        bool                    mDone;
};

#endif