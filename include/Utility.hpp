#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <QKeySequence>
#include <QColor>

#include <string>
#include <vector>
#include <ctime>

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

enum TrainingType
{
    Initial_Train = 0,
    Initial_Train_1 = 0,
    Initial_Train_2,
    Initial_Train_3,
    Initial_Train_4,
    MakeWord_Train,
    ChooseWord_Train,
    ChooseTranslation_Train ,
    RainWord_Train ,
    TrainCnt,

    Repetition_Train,
    WordsWelcome_Train = 1001,

    All = (0b1 << Initial_Train_1) | (0b1 << Initial_Train_2) | (0b1 << Initial_Train_3) 
            | (0b1 << Initial_Train_4) | (0b1 << MakeWord_Train) | (0b1 << ChooseWord_Train)
            | (0b1 << ChooseTranslation_Train) | (0b1 << RainWord_Train),
};

bool isFullyLearned(int status);

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

class Time
{
    public:
            Time();
            Time(const QString& strFormat);

        bool    operator>(const Time& time);
        void    operator+=(int hrs);

        QString             getStr() const;

        static Time         getCurrentTime();
        static QString      getCurrentTimeStr();
        static QString      getNextStartDateStr(TimeInterval timeInterval);
        static QString      getPreviousStartDateStr(TimeInterval timeInterval);
        static bool         compare(const QString& str1, const QString& str2);

    private:
        std::tm    time;
};

// std::string getCurrentTime();
// std::string getNextTimeIntervalStartDate(TimeInterval timeInterval);
// std::string getPreviousTimeIntervalStartDate(TimeInterval timeInterval);
// QString addHoursToDate(QString date, int hours);
// bool compare(const QString& str1, const QString& str2);

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