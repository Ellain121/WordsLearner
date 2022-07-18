#ifndef DICTIONARYSTATE_HPP
#define DICTIONARYSTATE_HPP

#include "NonCopyable.hpp"

#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

class DatabaseManager;

class DictionaryState : public QWidget, private NonCopyable
{
    Q_OBJECT

    public:
        DictionaryState(DatabaseManager* dbManager, QWidget* parent = 0);
        ~DictionaryState();

    private:
        void        setupCoreWidgets();
        void        setupCoreWidgetsConnections();

        void        setupTable();
        void        setupButtons();
    
    signals:
        void        backToMenu();
        void        fullyLearnedWordEliminated(QString learnedDate);

    private slots:
        void                searchWord();
        void                addWord();
        void                deleteWord();
        void                unlearnWord();

    private:
        DatabaseManager*            mDBManager;

        QSqlTableModel*             mTableModel;
        QTableView*                 mTableView;
        QSortFilterProxyModel*      mTableSortFilter;

        QVBoxLayout*                mMainVBoxLayout;
        QHBoxLayout*                mSearchHBoxLayout;
        QLineEdit*                  mSearchLineEdit;
        QPushButton*                mWordAddButton;
        QPushButton*                mWordResetButton;
        QPushButton*                mWordDeleteButton;
        QPushButton*                mReturnButton;




};

#endif