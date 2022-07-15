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
};

int                         getTrainingStatusBit(TrainingType trainType);
std::vector<TrainingType>   getRequiredTrainingsFromStatus(int statusBits);

std::string getCurrentTime();

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