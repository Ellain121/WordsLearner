#include "Training.hpp"

Training::Training(std::vector<Word>& words, int statusBit, QWidget* parent)
: QWidget(parent)
, mWords(words)
, mWordsTriesCnt(words.size(), 0)
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

    // if you it's your first or second try with specific word, set status as larned
    if (mWordsTriesCnt[mWordIndx] < 2)
        mWords[mWordIndx].status |= (1 << mStatusBit);
    do
    {
        mWordIndx++;
        if (mWordIndx >= mWords.size())
        {
            if (isAllWordsDone())
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
    } while (mWords[mWordIndx].status & (1 << mStatusBit));
}

void Training::skipWord()
{
    mWordsTriesCnt[mWordIndx]++;
    do
    {
        mWordIndx++;
        if (mWordIndx >= mWords.size())
        {
            mWordIndx = 0;
            emit fullCircle();
        }
    } while (mWords[mWordIndx].status & (1 << mStatusBit));
}

bool Training::isTrainingDone() const
{
    return mDone;
}

bool Training::isAllWordsDone() const
{
    for (int i = 0; i < mWords.size(); ++i)
    {
        auto& word = mWords[i];
        int triesCnt = mWordsTriesCnt[i];
        // if words not learned and triesCnt < 2
        if ( (!(word.status & (1 << mStatusBit))) && triesCnt < 2 )       
        {
            return false;
        }
    }
    return true;
}