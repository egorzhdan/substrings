#include "Watcher.h"
#include "Indexer.h"
#include "gui/MainWindow.h"

Watcher::Watcher(QDir root, QMainWindow *parent) : QThread(dynamic_cast<QObject *>(parent)), root(root) {
    updateRoots();
}

void Watcher::updateRoots() {
    auto it = QDirIterator(root.path(), QDir::Files, QDirIterator::Subdirectories);
    auto pathList = QStringList();
    while (it.hasNext()) {
        auto path = it.next();
        pathList.append(path);
    }
    delete watcher;
    watcher = new QFileSystemWatcher(pathList, this);
    connect(watcher, SIGNAL(directoryChanged(const QString &)), this, SLOT(directoryChanged(const QString &)));
    connect(watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(fileChanged(const QString &)));
}

void Watcher::reindex(const QString &path) {
    auto *par = dynamic_cast<MainWindow *>(parent());
    QDir currentRoot;
    QFileInfo fileInfo(path);
    if (fileInfo.isDir()) {
        currentRoot = QDir(path);
    } else {
        currentRoot = fileInfo.dir();
    }
    delete par->indexer;
    par->indexer = new Indexer(par, currentRoot);
    par->indexer->start();
}

void Watcher::directoryChanged(const QString &path) {
    qDebug() << "Watcher: dir changed: " << path;
    reindex(path);
}

void Watcher::fileChanged(const QString &path) {
    qDebug() << "Watcher: file changed: " << path;
    reindex(path);
}
