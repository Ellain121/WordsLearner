#include "Utility.hpp"

#include <cassert>

bool isFullyLearned(int status)
{
    int fullyLearnedStatus = (int)TrainingType::All;

    int indx = 0;
    while (fullyLearnedStatus & (0b1 << indx))
    {
        if ( !(status & (0b1 << indx)) )
        {
            return false;
        }
        ++indx;
    }
    return true;
}

Time::Time()
{
    std::time_t t = std::time(0);   // get time now
    time = *std::localtime(&t);
}

Time::Time(const QString& strFormat)
{
    assert(strFormat.size() == 11);
    for (int i = 0; i < 11; ++i)
    {
        if ((i + 1) % 3 == 0)
            assert(strFormat[i] == ':');
    }
    
    std::string str = strFormat.toStdString();
    int year    = (str[0] - '0')*10 + (str[1] - '0');
    int mon     = (str[3] - '0')*10 + (str[4] - '0');
    int day     = (str[6] - '0')*10 + (str[7] - '0');
    int hour    = (str[9] - '0')*10 + (str[10] - '0');

    std::tm time_in = { 0, 0, hour, // second, minute, hour
        day, mon-1, year+100 }; // 1-based day, 0-based month, year since 1900
    std::time_t t = std::mktime(&time_in);
    time = *std::localtime(&t);
}

QString Time::getStr() const
{
    std::string year = std::to_string(time.tm_year - 100);
    std::string mon = std::to_string(time.tm_mon + 1);
    std::string day = std::to_string(time.tm_mday);
    std::string hour = std::to_string(time.tm_hour);
    if (year.size() == 1) year = "0" + year;
    if (mon.size() == 1) mon = "0" + mon;
    if (day.size() == 1) day = "0" + day;
    if (hour.size() == 1) hour = "0" + hour;
    std::string cTime = year + ":" + mon + ":" + day + ":" + hour;
    assert(year.size() >= 2 && mon.size() == 2 && day.size() == 2 && hour.size() == 2);

    return QString::fromStdString(cTime);
}

Time Time::getCurrentTime()
{
    return Time();
}

QString Time::getCurrentTimeStr()
{
    auto t = Time();
    return t.getStr();
}

bool Time::operator>(const Time& t)
{
    // // yy-mm-dd-hh
    // // 01-34-67-9A

    std::string date_1 = getStr().toStdString();
    std::string date_2 = t.getStr().toStdString();
    
    int i = 0;
    while (i < 11 && date_1[i] == date_2[i])
    {
        i++;
    }
    return i >= 11 ? true : (date_1[i] > date_2[i]);
}

void Time::operator+=(int hrs)
{
    time.tm_hour += hrs;
    std::mktime(&time);
}

// returns true if date1 >= date2
bool Time::compare(const QString& date1, const QString& date2)
{
    // yy-mm-dd-hh
    // 01-34-67-9A
    int i = 0;
    while (i < 11 && date1[i] == date2[i])
    {
        i++;
    }
    return i >= 11 ? true : (date1[i] > date2[i]);
}

// QString addHoursToDate(QString date, int hours)
// {
//         std::tm time_in = { 0, 0, 0, // second, minute, hour
//         9, 10, 2016 - 1900 }; // 1-based day, 0-based month, year since 1900
//         std::time_t t = std::time(0);
//         std::time_t time_temp = std::mktime(&time_in);

//         //Note: Return value of localtime is not threadsafe, because it might be
//         // (and will be) reused in subsequent calls to std::localtime!
//         const std::tm * time_out = std::localtime(&t);

//         //Sunday == 0, Monday == 1, and so on ...
//         std::cout << "Today is this day of the week: " << time_out->tm_wday << "\n";
//         std::cout << "(Sunday is 0, Monday is 1, and so on...)\n";

//         // НАПИСАТЬ СТРУКТУРУ ВРЕМЕНИ КОТОРАЯ СМОЖЕТ ВОЗВРАЩАТЬ ВРЕМЯ В ФОРМАТЕ QString, И иметь в себе все
//         // НУЖНЫЕ ФУНКЦИИ.
//         // ПОСЛЕ ДОДЕЛАТЬ РЕПЕТИТИОН

// }

QString Time::getNextStartDateStr(TimeInterval timeInterval)
{
    // yy-mm-dd-hh
    // 01-34-67-9A
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    if (timeInterval == TimeInterval::Day)
    {
        now->tm_mday++;
        now->tm_hour = 4;
    }
    else if (timeInterval == TimeInterval::Week)
    {
        int wDay = now->tm_wday;
        int day_untill_end = 6 - wDay + 1;
        now->tm_mday += day_untill_end;
        now->tm_hour = 4;
    }
    else if (timeInterval == TimeInterval::Month)
    {
        now->tm_mon++;
        now->tm_mday = 1;
        now->tm_hour = 4;
    }
    else if (timeInterval == TimeInterval::Year)
    {
        now->tm_year++;
        now->tm_mon = 0;
        now->tm_mday = 1;
        now->tm_hour = 4;
    }
    std::mktime(now);
    std::string year = std::to_string(now->tm_year - 100);
    std::string mon = std::to_string(now->tm_mon + 1);
    std::string day = std::to_string(now->tm_mday);
    std::string hour = std::to_string(now->tm_hour);
    if (year.size() == 1) year = "0" + year;
    if (mon.size() == 1) mon = "0" + mon;
    if (day.size() == 1) day = "0" + day;
    if (hour.size() == 1) hour = "0" + hour;
    assert(year.size() == 2 && mon.size() == 2 && day.size() == 2 && hour.size() == 2);
    std::string cTime = year + ":" + mon + ":" + day + ":" + hour;

    return QString::fromStdString(cTime);
}

// std::string getNextTimeIntervalStartDate(TimeInterval timeInterval)
// {
//     // yy-mm-dd-hh
//     // 01-34-67-9A
//     std::time_t t = std::time(0);
//     std::tm* now = std::localtime(&t);
//     if (timeInterval == TimeInterval::Day)
//     {
//         now->tm_mday++;
//         now->tm_hour = 4;
//     }
//     else if (timeInterval == TimeInterval::Week)
//     {
//         int wDay = now->tm_wday;
//         int day_untill_end = 6 - wDay + 1;
//         now->tm_mday += day_untill_end;
//         now->tm_hour = 4;
//     }
//     else if (timeInterval == TimeInterval::Month)
//     {
//         now->tm_mon++;
//         now->tm_mday = 1;
//         now->tm_hour = 4;
//     }
//     else if (timeInterval == TimeInterval::Year)
//     {
//         now->tm_year++;
//         now->tm_mon = 0;
//         now->tm_mday = 1;
//         now->tm_hour = 4;
//     }
//     std::mktime(now);
//     std::string year = std::to_string(now->tm_year - 100);
//     std::string mon = std::to_string(now->tm_mon + 1);
//     std::string day = std::to_string(now->tm_mday);
//     std::string hour = std::to_string(now->tm_hour);
//     if (year.size() == 1) year = "0" + year;
//     if (mon.size() == 1) mon = "0" + mon;
//     if (day.size() == 1) day = "0" + day;
//     if (hour.size() == 1) hour = "0" + hour;
//     assert(year.size() == 2 && mon.size() == 2 && day.size() == 2 && hour.size() == 2);
//     std::string cTime = year + ":" + mon + ":" + day + ":" + hour;

//     return cTime;
// }
QString Time::getPreviousStartDateStr(TimeInterval timeInterval)
{
    // yy-mm-dd-hh
    // 01-34-67-9A
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    if (timeInterval == TimeInterval::Day)
    {
        now->tm_mday--;
        now->tm_hour = 4;
    }
    else if (timeInterval == TimeInterval::Week)
    {
        int wDay = now->tm_wday;
        now->tm_mday -= wDay;
        now->tm_hour = 4;
    }
    else if (timeInterval == TimeInterval::Month)
    {
        now->tm_mon--;
        now->tm_mday = 1;
        now->tm_hour = 4;
    }
    else if (timeInterval == TimeInterval::Year)
    {
        now->tm_year--;
        now->tm_mon = 0;
        now->tm_mday = 1;
        now->tm_hour = 4;
    }
    std::mktime(now);
    std::string year = std::to_string(now->tm_year - 100);
    std::string mon = std::to_string(now->tm_mon + 1);
    std::string day = std::to_string(now->tm_mday);
    std::string hour = std::to_string(now->tm_hour);
    if (year.size() == 1) year = "0" + year;
    if (mon.size() == 1) mon = "0" + mon;
    if (day.size() == 1) day = "0" + day;
    if (hour.size() == 1) hour = "0" + hour;
    assert(year.size() == 2 && mon.size() == 2 && day.size() == 2 && hour.size() == 2);
    std::string cTime = year + ":" + mon + ":" + day + ":" + hour;

    return QString::fromStdString(cTime);
}
// std::string getPreviousTimeIntervalStartDate(TimeInterval timeInterval)
// {
//     // yy-mm-dd-hh
//     // 01-34-67-9A
//     std::time_t t = std::time(0);
//     std::tm* now = std::localtime(&t);
//     if (timeInterval == TimeInterval::Day)
//     {
//         now->tm_mday--;
//         now->tm_hour = 4;
//     }
//     else if (timeInterval == TimeInterval::Week)
//     {
//         int wDay = now->tm_wday;
//         now->tm_mday -= wDay;
//         now->tm_hour = 4;
//     }
//     else if (timeInterval == TimeInterval::Month)
//     {
//         now->tm_mon--;
//         now->tm_mday = 1;
//         now->tm_hour = 4;
//     }
//     else if (timeInterval == TimeInterval::Year)
//     {
//         now->tm_year--;
//         now->tm_mon = 0;
//         now->tm_mday = 1;
//         now->tm_hour = 4;
//     }
//     std::mktime(now);
//     std::string year = std::to_string(now->tm_year - 100);
//     std::string mon = std::to_string(now->tm_mon + 1);
//     std::string day = std::to_string(now->tm_mday);
//     std::string hour = std::to_string(now->tm_hour);
//     if (year.size() == 1) year = "0" + year;
//     if (mon.size() == 1) mon = "0" + mon;
//     if (day.size() == 1) day = "0" + day;
//     if (hour.size() == 1) hour = "0" + hour;
//     assert(year.size() == 2 && mon.size() == 2 && day.size() == 2 && hour.size() == 2);
//     std::string cTime = year + ":" + mon + ":" + day + ":" + hour;

//     return cTime;
// }



int getTrainingStatusBit(TrainingType trainType)
{
    // return (int)trainType;
    return (int)trainType;
    // if (trainType == TrainingType::Initial_Train) return 0;
    // if (trainType == TrainingType::MakeWord_Train) return 1;
    // if (trainType == TrainingType::ChooseWord_Train) return 2;
    // if (trainType == TrainingType::ChooseTranslation_Train) return 3;
    // if (trainType == TrainingType::RainWord_Train) return 4;
    // if (trainType == TrainingType::Repetition_Train) return (int)trainType;//TEMP DECISION ONLY
    // assert(false);
}

// dont count repetition train
std::vector<TrainingType> getRequiredTrainingsFromStatus(int statusBits)
{
    std::vector<TrainingType> requiredTrainings;
    for (int i = 0; i <= (int)TrainingType::TrainCnt; ++i)
    {
        if ( (statusBits & (0b1 << i)) == 0)
        {
            requiredTrainings.push_back((TrainingType)i);
        }
    }
    return requiredTrainings;
}   

SimpleWord::SimpleWord(const std::string& word, const std::string& translation)
: word(word)
, translation(translation)
{
}

Word::Word(const std::string& word, const std::string& translation, int status)
: word(word)
, translation(translation)
, status(status)
{
}

const int WINDOW_WIDTH = 1080;
const int WINDOW_HEIGHT = 720;


QColor getQColor(Color color)
{
    if (color == Color::Red)
    {
        return QColor(255, 0, 0, 255);
    }
    if (color == Color::LimeGreen)
    {
        return QColor(50, 205, 50, 255);
    }
    if (color == Color::White)
    {
        return QColor(255, 255, 255, 255);
    }

    return QColor(0, 0, 0, 0);
}

RandomEngine::RandomEngine()
{
    state = time(NULL);
}

uint32_t RandomEngine::getRandom()
{
    uint32_t x = state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    state = x;
    return x;
}

int RandomEngine::getRandom(int bound)
{
    return getRandom() % bound;
}

uint32_t RandomEngine::operator()(int bound)
{
    return getRandom(bound);
}

std::vector<SimpleWord> get120SimpleWords()
{
    std::vector<SimpleWord> sWords = {
        {"ab-", ", словах, с общим значением удаления"},
        {"ab-unit", "абсолютная единица системы СГСЭ или СГСМ,"},
        {"aba", "аба, свободная верхняя одежда из верблюжьей или козьей шерсти"},
        {"abaca", "абака"},
        {"abaci", "счеты, абак, абака, мозаичная панель, номограмма"},
        {"abaciscus", "плитка, абак, верхняя часть капители колонны"},
        {"aback", "назад, сзади, задом"},
        {"abacterial", "безмикробный, стерильный, абактериальный, безбактериальный"},
        {"abaction", "крупная кража угон скота"},
        {"abaculus", "плитка, кубик или шарик цветного стекла, абак, небольшие счеты"},
        {"abacus", "счеты, абак, абака, мозаичная панель, номограмма"},
        {"Abaddon", "Аваддон, ад, преисподняя"},
        {"abaft", "позади, сзади, на корме, с кормы, позади, сзади"},
        {"abalone", "морское ушко"},
        {"abampere", "абампер, ампер, единица силы тока СГСМ"},
        {"abandon", "отказываться от, оставлять, покидать, развязность, несдержанность"},
        {"abandoned", "заброшенный, брошенный, покинутый, оставленный, распутный"},
        {"abandonee", "страховщик, лицо, в пользу которого отказ"},
        {"abandonment", "отказ, оставление, заброшенность, одиночество"},
        {"abase", "унижать, понижать"},
        {"abasement", "унижение, уничижение, понижение"},
        {"abash", "смущать, конфузить, приводить в замешательство"},
        {"abashed", "смущенный, сконфуженный, пристыженный"},
        {"abashment", "растерянность, замешательство, смущение"},
        {"abasia", "абазия, неспособность ходить"},
        {"abask", "греясь на солнышке, находясь в приятной, доброй атмосфере"},
        {"abat-jour", "абажур, козырек, отражатель"},
        {"abat-vent", "жалюзи, односкатная крыша, зонт дымовой трубы"},
        {"abatable", "аннулируемый, отменяемый"},
        {"abate", "уменьшить, уменьшать, уменьшаться, ослабевать, утихать, ослаблять, умерять, сбавлять"},
        {"abated", "сниженный, ослабевший, ослабленный"},
        {"abatement", "снижение, уменьшение, ослабление, скидка, смягчение, прекращение, аннулирование"},
        {"abater", "прошение об аннулировании, прекращении"},
        {"abatis", "засека, завал"},
        {"abattage", "массовое уничтожение животных, чтобы предотвратить распространение инфекции"},
        {"abattoir", "скотобойня"},
        {"abatvoix", "акустический экран"},
        {"abaxial", "абаксиальный, направленный от оси, на верхней поверхности листа"},
        {"abb", "уток, основа ткани"},
        {"abba", "Авва, Отец, авва, авва"},
        {"abbacy", "аббатство"},
        {"abbatial", "аббатский, аббатовский"},
        {"abbe", "аббат, батюшка"},
        {"abbess", "аббатиса, настоятельница монастыря"},
        {"abbey", "аббатство, монастырь"},
        {"abbot", "аббат, настоятель монастыря"},
        {"abbreviate", "сокращать, использовать сокращения, урезать"},
        {"abbreviated", "сокращенный, укороченный"},
        {"abbreviating", "сокращение, сокращающий"},
        {"abbreviation", "аббревиатура, сокращение, аббревиация"},
        {"abbreviator", "составитель папского бреве, аббревиатор,"},
        {"abbreviature", "сокращение, сокращенное изложение,"},
        {"ABC", "азбука, алфавит, азы, основы, начатки, простой, простейший"},
        {"ABC-book", "букварь"},
        {"abdest", "абдест, ритуальное омовение рук перед молитвой"},
        {"abdicant", "человек, отрекающийся, отказывающийся, отрекающийся"},
        {"abdicate", "отказываться, отрекаться, слагать полномочия"},
        {"abdication", "отречение, отказ, сложение полномочий, отказ от должности, отказ от права"},
        {"abdomen", "живот, брюшная полость, брюшко, брюхо, брюшина"},
        {"abdominal", "брюшной, абдоминальный, брюхоперый"},
        {"abdominoscopy", "осмотр, ревизия брюшной полости, перитонеоскопия, абдоминоскопия"},
        {"abdominous", "толстобрюхий, пузатый, толстый"},
        {"abduce", "оттягивать, отводить"},
        {"abducens", "отводящий нерв, VI черепной нерв"},
        {"abducent", "отводящий"},
        {"abduct", "похищать, отводить, оттягивать, насильно уводить, насильно увозить"},
        {"abduction", "похищение, отведение, увоз, абдукция, насильственный увоз, увод"},
        {"abductor", "похититель, абдуктор, отводящая мышца"},
        {"abeam", "на траверзе, с траверза"},
        {"abear", "терпеть, выносить, везти, переводить"},
        {"abecedaria", "начинающий, новичок, обучающий грамоте, расположенный в алфавитном порядке"},
        {"abecedarian", "элементарный, обучающийся грамоте"},
        {"abecedarium", "учебник для начинающих, букварь, учебник"},
        {"abecedary", "обучающийся грамоте, азбучный, элементарный"},
        {"abed", "в постели, лежащий в постели"},
        {"Abel", "Абель, Авель, Эйбел"},
        {"abele", "тополь белый, тополь, серебристый тополь"},
        {"abelmosk", "мускусное дерево, мускусное зерно"},
        {"aberdeen", "Абердин, Абердин, , абердинский"},
        {"aberdeenshire", "Абердиншир"},
        {"aberdevine", "чечетка"},
        {"aberrance", "отклонение от нормы, аберрация, отклонение от правильного пути"},
        {"aberrancy", "отклонение от правильного пути, отклонение от нормы, аберрация"},
        {"aberrant", "отклоняющийся, отклоняющийся от нормы, заблуждающийся, отклонившийся"},
        {"aberrate", "аберрировать, отклоняться от нормы, отклоняться, отходить, обладать аберрацией"},
        {"aberration", "аберрация, отклонение, заблуждение, помрачение ума, отклонение от стандарта"},
        {"aberrational", "иррациональный, аберрантный, заблуждающийся"},
        {"aberrationist", "человек с отклонениями от нормы, человек с отклонениями от нормы"},
        {"aberystwyth", "Аберистуит"},
        {"abessive", "абессив, лишительный или изъятельный падеж"},
        {"abet", "подстрекать, поощрять, содействовать"},
        {"abetment", "подстрекательство, поощрение, содействие"},
        {"abetter", "подстрекатель, пособник, пособница,"},
        {"abetting", "соучастие, подстрекательский"},
        {"abettor", "подстрекатель, пособник, соучастник"},
        {"abeyance", "состояние неопределенности, временное бездействие, временное прекращение"},
        {"abeyancy", "временное бездействие, временное прекращение, состояние неопределенности"},
        {"abeyant", "бездействующий, оставшийся без владельца, бесхозный"},
        {"abhor", "ненавидеть, питать отвращение"},
        {"abhorrence", "отвращение, ненависть, омерзение, то, что вызывает ненависть"},
        {"abhorrent", "отвратительный, ненавистный, претящий, мерзкий, несовместимый"},
        {"abidance", "соблюдение, подчинение"},
        {"abide", "придерживаться, выполнять, терпеть, ждать, оставаться верным, обретаться"},
        {"abiding", "неизменный, постоянный, прочный"},
        {"abiding-place", "местожительство, местопребывание"},
        {"abidjan", "Абиджан, абиджанский"},
        {"abietene", "абиетин, хвойное масло"},
        {"abigail", "камеристка"},
        {"abilene", "Абилин, в штатах Вирджиния, абилен"},
        {"ability", "способность, возможность, умение, ловкость, дарование, компетенция"},
        {"abintestate", "законный наследник, лицо, наследник по закону"},
        {"abiocoen", "неживая природа, абиоцен"},
        {"abiogenesis", "абиогенез, возникновение жизни из неорганической природы, спонтанная генерация"},
        {"abiogenist", "сторонник теории абиогенеза, возникновения жизни из неорганической природы"},
        {"abiogenous", "абиогенный"},
        {"abiosis", "абиоз, пониженная жизнеспособность органа или системы"},
        {"abiotic", "абиотический, нежизненный"},
        {"abiotrophy", "абиотрофия"},
        {"abirritant", "средство, снимающее раздражение, успокаивающий, снимающий раздражение"},
        {"abirritate", "успокаивать, снимать раздражение, уменьшать рефлекторную возбудимость"},
        {"abject", "жалкий, презренный, низкий, несчастный, униженный"},
        {"abjection", "униженность, унижение, низость, приниженность, уничижение"},
        {"abjective", "принижающий, подавляющий, унижающий, деморализующий"},
        {"abjuration", "отречение, отказ, отступничество"},
        {"abjure", "отрекаться, отказываться, отступаться"},
        {"abjurer", "отрекшийся, отступник, отступница"},
        {"abjuror", "отрекшийся, отступник, отступница"},
        {"abkhazia", "Абхазия, территориальным статусом, Сухуми"},
        {"abkhazian", "абхаз, абхазка, абхазский язык, абхазский, абхазский, относящийся к Абхазии"},
        {"ablactate", "отнимать от груди, прививать сближением"},
        {"ablactation", "отнятие от груди"},
        {"ablare", "шумный, хриплый, сиплый"},
        {"ablastemic", "беззародышевый"},
        {"ablastous", "абластический, неспособный к прорастанию"},
        {"ablate", "ампутировать, отсекать, авиа, подвергаться абляции"},
        {"ablating", "удаление, аблирование, ампутация, абляционный"},
        {"ablation", "удаление, устранение, таяние ледников, размывание пород, вырезывание"},
        {"ablatitious", "уменьшающий, отбирающий"},
        {"ablative", "аблятивный, творительный, отделительный, аблятив, творительный падеж"},
        {"ablator", "абляционный, теплозащитный материал, инструмент для производства ампутации"},
        {"ablaut", "абляут"},
        {"ablaze", "в огне, в пламени, сверкающий, горящий, возбужденный"},
        {"able", "в состоянии, способный, умеющий, могущий, умелый, талантливый, компетентный, знающий"},
        {"able-bodied", "крепкий, здоровый, годный, трудоспособный"},
        {"able-minded", "способный, умный"},
        {"ablegate", "легат, папский посланник, аблегат"},
        {"ablepsia", "слепота"},
        {"ablet", "уклейка"},
        {"ablins", "может быть, возможно,"},
        {"abloom", "в цвету, цветущий, расцветший"},
        {"abluent", "очищающий, моющий, очищающее средство, дезинфицирующее"},
        {"ablush", "смущенный, покрасневший, зардевшийся"},
        {"abluted", "вымытый, отмытый дочиста"},
        {"ablution", "омовение, промывка, промывание, обмывание"},
        {"ablutionary", "связанный с омовением"},
        {"ably", "умело, искусно, квалифицированно, компетентно"},
        {"abmodality", "флуктуация, отклонение от среднего"},
        {"abnegate", "отказываться, отрицать, отказываться от, отказывать себе в, отвергать"},
        {"abnegation", "непринятие, отрицание, отречение, отказ, самоотречение, самопожертвование"},
        {"abner", "Авенир, Эбнер, Абнер"},
        {"abnodate", "обрезать сучья"}, 
    };
    return sWords;
}