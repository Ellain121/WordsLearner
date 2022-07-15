#include "Training.hpp"

Training::Training(std::vector<Word>& words, int statusBit, QWidget* parent)
: QWidget(parent)
, mWords(words)
, mStatusBit(statusBit)
, mWordIndx(0)
, mDone(false)
{

}

// void Training::fullCircle()
// {
// }

void Training::nextWord()
{
    int oldIndx = mWordIndx;

    mWords[mWordIndx].status |= (1 << mStatusBit);
    do
    {
        mWordIndx++;
        if (mWordIndx >= mWords.size())
        {
            if (isAllWordsLearned())
            {
                emit trainingDone();
                mDone = true;
                // mWordIndx = oldIndx;
                break;
            }
            else
            {
                emit fullCircle();
                mWordIndx = 0;
            }
        }
    } while(mWords[mWordIndx].status & (1 << mStatusBit));
}

void Training::skipWord()
{
    do
    {
        mWordIndx++;
        if (mWordIndx >= mWords.size())
        {
            mWordIndx = 0;
            emit fullCircle();
        }
    } while(mWords[mWordIndx].status & (1 << mStatusBit));
}

bool Training::isTrainingDone() const
{
    return mDone;
}

bool Training::isAllWordsLearned() const
{
    for (auto& word : mWords)
    {
        if ( !(word.status & (1 << mStatusBit)) )       
        {
            return false;
        }
    }
    return true;
}