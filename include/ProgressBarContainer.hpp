#ifndef PROGRESSBARCONTAINER_HPP
#define PROGRESSBARCONTAINER_HPP

#include "ProgressBar.hpp"
#include "NonCopyable.hpp"

#include <QSqlDatabase>
#include <QObject>

class ProgressBarContainer : public QObject, private NonCopyable
{
    Q_OBJECT
    
    public:
        ProgressBarContainer(const std::vector<ProgressBar*>& children);
        ~ProgressBarContainer();

        void    writeChanges();
    
    private:
        void    loadChildrenInfo();
        void    processChildrenResetDates();
        void    setupConnections();
    
    // private slots:
    
    private:
        std::vector<ProgressBar*>   mChildren;
        QSqlDatabase                mDBConnection;
        // std::vector<StatusBarInfo>  mChildrenInfo;
};



#endif