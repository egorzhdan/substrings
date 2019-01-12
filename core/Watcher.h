#ifndef WATCHER_H
#define WATCHER_H

#include <QThread>
#include <QtGui>
#include <QMainWindow>

class Watcher : public QThread {
Q_OBJECT

public:
    Watcher(QDir root, QMainWindow *parent);

private:
    QDir root;
    QFileSystemWatcher *watcher = nullptr;

    void updateRoots();
    void reindex(const QString& path);

private slots:
    void fileChanged(const QString &path);
};

#endif //WATCHER_H
