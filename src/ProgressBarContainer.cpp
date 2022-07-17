#include "ProgressBarContainer.hpp"
#include "Utility.hpp"

#include <QDebug>
#include <QtSql>
#include <cassert>

#define DB_NAME "../data/status_bars.sqlite"
#define TABLE_NAME "status_bars"

#include <iostream>

namespace
{

QString getNameFromIndx(int indx)
{
    assert(indx >= 0 && indx < 4);

    if (indx == 0) return "daily";
    if (indx == 1) return "weekly";
    if (indx == 2) return "monthly";
    return "yearly";
}

}

ProgressBarContainer::ProgressBarContainer(const std::vector<ProgressBar*>& children)
: mChildren(children)
{
    mDBConnection = QSqlDatabase::addDatabase("QSQLITE", "connection_2");
    mDBConnection.setDatabaseName(DB_NAME);
    if (!mDBConnection.open())
    {
        qDebug() << "Cannot open database!";
    }

    QSqlQuery query(mDBConnection);
    QString createTableCmd = "CREATE TABLE IF NOT EXISTS " TABLE_NAME " ("
            "Name TEXT, "
            "Counter integer, "
            "Cnt_Reset_Date VARCHAR(11), "
            "MaxValue integer);";

    if (!query.exec(createTableCmd))
    {
        qDebug() << "Cannot create table!\n";
        qDebug() << query.lastError();
    }

    QSqlQuery statement("select count(1) from " TABLE_NAME ";", mDBConnection);
    statement.next();
    int cnt = statement.value(statement.record().indexOf("count(1)")).toString().toInt();

    assert(cnt == 0 || cnt == 4);
    if (cnt == 0)
    {
        std::cout << "COUNTER EQUAL ZERO!\n";
        QString initialInsertCmd = "INSERT INTO " TABLE_NAME " (Name, Counter, Cnt_Reset_Date, MaxValue) "
        "SELECT 'daily' AS 'Name', 0 AS 'Counter', '%1' as 'Cnt_Reset_Date', 10 as 'MaxValue' "
        "UNION ALL SELECT 'weekly', 0, '%2', 70 "
        "UNION ALL SELECT 'monthly', 0, '%3', 300 "
        "UNION ALL SELECT 'yearly', 0, '%4', 3650 ";

        initialInsertCmd = initialInsertCmd
                    .arg(QString::fromStdString((getNextTimeIntervalStartDate(TimeInterval::Day))))
                    .arg(QString::fromStdString((getNextTimeIntervalStartDate(TimeInterval::Week))))
                    .arg(QString::fromStdString((getNextTimeIntervalStartDate(TimeInterval::Month))))
                    .arg(QString::fromStdString((getNextTimeIntervalStartDate(TimeInterval::Year))));

        std::cout << initialInsertCmd.toStdString() << std::endl;
        
        if (!query.exec(initialInsertCmd))
        {
            qDebug() << "Cannot insert initial values!\n";
            qDebug() << query.lastError();
        }
    }
    loadChildrenInfo();
    processChildrenResetDates();
    setupConnections();   
}

ProgressBarContainer::~ProgressBarContainer()
{
    writeChanges();
    mDBConnection.close();
    mDBConnection = QSqlDatabase();
    QSqlDatabase::removeDatabase("connection_2");
}

void ProgressBarContainer::processChildrenResetDates()
{
    QString cDate = QString::fromStdString(getCurrentTime());
    for (int i = 0; i < mChildren.size(); ++i)
    {
        auto& child = mChildren[i];

        // if reset required
        if (compare(cDate, child->getResetDate()))
        {
            // std::cout << "Expired! " << getNameFromIndx(i).toStdString() << std::endl;
            child->setResetDate(QString::fromStdString(getNextTimeIntervalStartDate( (TimeInterval)i )));
            child->setVal(0);
            // std::cout << "newResetDate: " << getNextTimeIntervalStartDate( (TimeInterval)i ) << std::endl;
        }
    }
}

void ProgressBarContainer::writeChanges()
{
    QSqlQuery update_statement(mDBConnection);
    update_statement.prepare(QString("update " TABLE_NAME " set Counter=:cnt, Cnt_Reset_Date=:resetDate, MaxValue=:maxVal WHERE Name=:name"));
    for (int i = 0; i < mChildren.size(); ++i)
    {
        auto& child = mChildren[i];

        update_statement.bindValue(":name", getNameFromIndx(i));
        update_statement.bindValue(":cnt", child->getVal());
        update_statement.bindValue(":resetDate", child->getResetDate());
        update_statement.bindValue(":maxVal", child->getMaxVal());
        if (!update_statement.exec())
        {
            qDebug() << "Cannot update name: " << getNameFromIndx(i);
            qDebug() << update_statement.lastError();
        }
        
    }
}

void ProgressBarContainer::loadChildrenInfo()
{
    QString cmd = QString("SELECT * FROM " TABLE_NAME ";");
    QSqlQuery statement(cmd, mDBConnection);
    QSqlRecord record = statement.record();

    int i = 0;
    while (statement.next())
    { 
        assert(i < mChildren.size());
        auto& prgBar = mChildren[i];


        QString name = statement.value(record.indexOf("Name")).toString();
        int cnt = statement.value(record.indexOf("Counter")).toInt();
        QString date = statement.value(record.indexOf("Cnt_Reset_Date")).toString();
        int maxVal = statement.value(record.indexOf("MaxValue")).toInt();

        prgBar->setVal(cnt);
        prgBar->setResetDate(date);
        prgBar->setMaxValue(maxVal);
        std::cout << cnt << " " << date.toStdString() << " " << maxVal << std::endl;

        ++i;
    }
}

void ProgressBarContainer::setupConnections()
{
    for (int i = 0; i < mChildren.size(); ++i)
    {
        auto child = mChildren[i];
        connect(child, &ProgressBar::maxValueChanged, [&, i](float value){
            // std::cout << value << std::endl;
            if (i == 1) value /= 7.f;
            if (i == 2) value /= 30.f;
            if (i == 3) value /= 365.f;
            for (int k = 0; k < mChildren.size(); ++k)
            {
                if (k == i) continue;
                float tmpVal = value;
                if (k == 1) tmpVal *= 7.f;
                if (k == 2) tmpVal *= 30.f;
                if (k == 3) tmpVal *= 365.f;
                mChildren[k]->setMaxValue(tmpVal);
            }
            writeChanges();
        });
        // connect(child, &ProgressBar::someValueChanged, this, &ProgressBarContainer::writeChanges);
        // connect(pClassA, &ClassA::requestToChangeRange, this,
        //     [pClassX, pClassC](voltage_range_e vr, current_range_e cr, uint16_t bits) {
        // //...
        // });
    }
}
