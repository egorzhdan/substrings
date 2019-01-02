#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <core/Searcher.h>
#include "core/Indexer.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow();

    ~MainWindow();

public slots:
    void openDialogClicked();
    void indexerStatusUpdated(QString status, bool busy);
    void searchCalled();
    void searcherStatusUpdated(QString status);
    void matchFound(QString filePath);
    void matchDoubleClicked(int col, int row);

private:
    Indexer *indexer = nullptr;
    Searcher *searcher = nullptr;

    QPushButton *openDialogButton;
    QLabel *statusLabel;
    QLineEdit *searchField;
    QTableWidget *filesView;
};

#endif //MAINWINDOW_H
