#include "MainWindow.h"
#include <QLabel>
#include <QHeaderView>
#include <QFileDialog>
#include <QGridLayout>

MainWindow::MainWindow() : QMainWindow() {
    setWindowTitle("Substrings");
    setMinimumSize(500, 300);

    auto widget = new QWidget(this);
    auto layout = new QGridLayout(widget);
    widget->setLayout(layout);
    setCentralWidget(widget);

    openDialogButton = new QPushButton("Choose Directory", widget);
    layout->addWidget(openDialogButton, 0, 0, 1, 2, Qt::AlignVCenter);
    connect(openDialogButton, SIGNAL(released()), this, SLOT(openDialogClicked()));

    statusLabel = new QLabel(widget);
    layout->addWidget(statusLabel, 0, 2, 1, 3, Qt::AlignVCenter);

    searchField = new QLineEdit(widget);
    searchField->setEnabled(false);
    searchField->setPlaceholderText("Search");
    layout->addWidget(searchField, 1, 0, 1, 5, Qt::AlignTop);
    connect(searchField, SIGNAL(returnPressed()), this, SLOT(searchCalled()));

    filesView = new QTableWidget(0, 1, widget);
    filesView->horizontalHeader()->setVisible(false);
    filesView->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(filesView, 2, 0, 1, 5);
    connect(filesView, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(matchDoubleClicked(int, int)));
}

void MainWindow::openDialogClicked() {
    auto rootPath = QFileDialog::getExistingDirectory(this);
    openDialogButton->setEnabled(false);
    root = QDir(rootPath);
    watcher = new Watcher(root, this);
    indexer = new Indexer(this, root);
    connect(indexer, SIGNAL(statusUpdated(QString, bool)), this, SLOT(indexerStatusUpdated(QString, bool)));
    indexer->start();
}

void MainWindow::indexerStatusUpdated(QString status, bool busy) {
    statusLabel->setText(status);
    searchField->setEnabled(!busy);
    index = new Index(indexer->indexNonBlocking());
}

void MainWindow::searchCalled() {
    if (searcher) {
        searcher->requestInterruption();
        searcher->wait();
    }
    filesView->clear();
    filesView->setRowCount(0);

    auto query = searchField->text();
    qDebug() << "Searching " << query;
    searcher = new Searcher(indexer->waitForIndex(), query, this);
    searcher->start();
    connect(searcher, SIGNAL(statusUpdated(QString)), this, SLOT(searcherStatusUpdated(QString)));
    connect(searcher, SIGNAL(matchFound(QString)), this, SLOT(matchFound(QString)));
}

void MainWindow::searcherStatusUpdated(QString status) {
    statusLabel->setText(status);
}

void MainWindow::matchFound(QString filePath) {
//    statusLabel->setText(QString::number(count) + " fidles found");
    auto rootPath = root.path();
    auto shortPath = (filePath.startsWith(rootPath) ? filePath.remove(0, rootPath.size() + 1) : rootPath);

    int count = filesView->rowCount();
    filesView->setRowCount(count + 1);
    auto *item = new QTableWidgetItem(shortPath);
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    filesView->setItem(count, 0, item);
}

void MainWindow::matchDoubleClicked(int row, int col) {
    auto path = root.absoluteFilePath(filesView->item(row, col)->text());
    auto x = path.toStdString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

MainWindow::~MainWindow() {
    if (watcher) {
        if (watcher->isRunning()) {
            watcher->requestInterruption();
            watcher->wait();
        }
    }
    if (indexer) {
        if (indexer->isRunning()) {
            indexer->cancel();
            indexer->wait();
        }
    }
    if (searcher) {
        if (searcher->isRunning()) {
            searcher->requestInterruption();
            searcher->wait();
        }
    }
    delete index;
}
