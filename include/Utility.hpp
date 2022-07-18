#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <QKeySequence>
#include <QColor>

#include <string>
#include <vector>

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

enum TrainingType
{
    Initial_Train = 0,
    MakeWord_Train = 1,
    ChooseWord_Train = 2,
    ChooseTranslation_Train = 3,
    RainWord_Train = 4,

    Repetition_Train = 1000,
    WordsWelcome_Train = 1001,

    All = 31,
};

// enum TrainingType
// {
//     Initial_Train_1         = 0,
//     Initial_Train_2         = 1,
//     Initial_Train_3         = 2,
//     Initial_Train_4         = 3,
//     Initial_Train       = (0b1 << Initial_Train_1) | (0b1 << Initial_Train_2) | (0b1 << Initial_Train_3) | (0b1 << Initial_Train_4),

//     MakeWord_Train          = 4,
//     ChooseWord_Train        = 5,
//     ChooseTranslation_Train = 6,
//     RainWord_Train          = 7,

//     All                 = Initial_Train | (0b1 << MakeWord_Train) | (0b1 << ChooseWord_Train) 
//                                             | (0b1 << ChooseTranslation_Train) | (0b1 << RainWord_Train),
// };

int                         getTrainingStatusBit(TrainingType trainType);
std::vector<TrainingType>   getRequiredTrainingsFromStatus(int statusBits);

enum TimeInterval
{
    Day = 0,
    Week = 1,
    Month = 2,
    Year = 3,
};

std::string getCurrentTime();
std::string getNextTimeIntervalStartDate(TimeInterval timeInterval);
std::string getPreviousTimeIntervalStartDate(TimeInterval timeInterval);
bool compare(const QString& str1, const QString& str2);

struct SimpleWord
{
    SimpleWord(const std::string& word, const std::string& translation);

    std::string     word;
    std::string     translation; 
};

struct Word
{
    Word(const std::string& word, const std::string& translation, int status = 0);

    std::string     word;
    std::string     translation;
    int             status;
};

enum Color
{
    Red,
    LimeGreen,
    White,
};

QColor getQColor(Color color);


struct RandomEngine
{
    uint32_t state;

    RandomEngine();
    uint32_t getRandom();
    int getRandom(int bound);
    uint32_t operator()(int bound);
};

std::vector<SimpleWord> get120SimpleWords();

#endif