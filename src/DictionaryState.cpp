#include "DictionaryState.hpp"
#include "DatabaseManager.hpp"
#include "TableButton.hpp"

#include <QHeaderView>
#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlRecord>

#include <ctime>

DictionaryState::DictionaryState(DatabaseManager* dbManager, QWidget* parent)
: QWidget(parent)
, mDBManager(dbManager)
{
    setupCoreWidgets();
    setupCoreWidgetsConnections();
}

void DictionaryState::setupCoreWidgets()
{
    mMainVBoxLayout = new QVBoxLayout;
    mSearchHBoxLayout = new QHBoxLayout;
    mSearchLineEdit = new QLineEdit;
    mWordAddButton = new QPushButton("ADD");
    mReturnButton = new QPushButton("Return");

    mWordAddButton->setShortcut(QKeySequence(Qt::Key_Return));
    mWordAddButton->setFixedSize(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1);

    setupTable();
    setLayout(mMainVBoxLayout);

    mSearchHBoxLayout->addWidget(mSearchLineEdit);
    mSearchHBoxLayout->addWidget(mWordAddButton);

    mMainVBoxLayout->addLayout(mSearchHBoxLayout);
    mMainVBoxLayout->addWidget(mTableView);
    mMainVBoxLayout->addWidget(mReturnButton);
}

void DictionaryState::setupCoreWidgetsConnections()
{
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &DictionaryState::searchWord);
    connect(mWordAddButton, &QPushButton::clicked, this, &DictionaryState::addWord);
    connect(mReturnButton, &QPushButton::clicked, [&](){
        emit backToMenu();
    });
}

void DictionaryState::setupTable()
{
    mTableModel = new QSqlTableModel(0, mDBManager->getDBConnection());
    mTableModel->setTable(QString::fromStdString(mDBManager->getTableName()));
    mTableModel->select();    
    // mTableModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Word"));
    // mTableModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Translation"));
    // mTableModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Status"));
    // mTableModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Word"));
    // mTableModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Translation"));
    // mTableModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Status"));
    // mTableModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Added"));
    // mTableModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Learned"));
    // mTableModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Repetition"));
    mTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    mTableView = new QTableView;
    mTableSortFilter = new QSortFilterProxyModel;

    mTableSortFilter->setSourceModel(mTableModel);
    mTableView->setModel(mTableSortFilter);
    mTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    mTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QHeaderView* header = mTableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    setupButtons();
}

void DictionaryState::setupButtons()
{
    mTableModel->insertColumn(6);
    mTableModel->insertColumn(7);
    mTableModel->setHeaderData(6, Qt::Horizontal, QObject::tr(""));
    mTableModel->setHeaderData(7, Qt::Horizontal, QObject::tr(""));

    QSqlQuery statement(QString("select * from ") + 
            QString::fromStdString(mDBManager->getTableName()), 
            mDBManager->getDBConnection());
    QSqlRecord record = statement.record();

    int i = 0;
    while (statement.next())
    {
        TableButton* resetButton = new TableButton(i, "O");
        TableButton* deleteButton = new TableButton(i, "X");
        mTableView->setIndexWidget(mTableSortFilter->index(i, 6), resetButton);
        mTableView->setIndexWidget(mTableSortFilter->index(i, 7), deleteButton);
        connect(resetButton, &QPushButton::clicked, this, &DictionaryState::unlearnWord);
        connect(deleteButton, &QPushButton::clicked, this, &DictionaryState::deleteWord);
        
        ++i;
    }

}

void DictionaryState::addWord()
{
    QString word = mSearchLineEdit->text();
    if (word.size() == 0) return;

    bool ok;
    QString translation = QInputDialog::getText(this, tr("Add translation"),
            tr("Word translation: "), QLineEdit::Normal, "", &ok);

    if (ok && !translation.isEmpty())
    {
        int row = mTableModel->rowCount();
        mTableModel->insertRow(row);
        mTableModel->setData(mTableModel->index(row, 0), word);
        mTableModel->setData(mTableModel->index(row, 1), translation);
        mTableModel->setData(mTableModel->index(row, 2), 0);
        mTableModel->setData(mTableModel->index(row, 3), QString::fromStdString(getCurrentTime())); //added time
        mTableModel->setData(mTableModel->index(row, 4), 0);
        mTableModel->setData(mTableModel->index(row, 5), 0);
        mTableModel->submitAll();

        mSearchLineEdit->setText("");
        // setupButtons();
    }
    setupButtons();
}

void DictionaryState::searchWord()
{
    QString word = mSearchLineEdit->text();
    // if (word.size() == 0) return;

    QString filter = "^" + word + ".*$";
    // mDictTableModel->setFilter(filter);
    mTableSortFilter->setFilterKeyColumn(0);
    mTableSortFilter->setFilterRegExp(filter);
}

void DictionaryState::deleteWord()
{
    TableButton* senderButton = qobject_cast<TableButton*>(sender());
    size_t row = senderButton->getRow();

    mTableSortFilter->removeRow(row);
    mTableModel->submitAll();
    setupButtons();
}

void DictionaryState::unlearnWord()
{
    TableButton* senderButton = qobject_cast<TableButton*>(sender());
    size_t row = senderButton->getRow();

    mTableModel->setData(mTableModel->index(row, 2), 0);

    mTableModel->submitAll();
    setupButtons();
}