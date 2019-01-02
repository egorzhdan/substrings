#ifndef WATCHER_H
#define WATCHER_H

#include <QThread>
#include <QtGui>

class Watcher : public QThread {
Q_OBJECT

public:
    Watcher(QObject *parent);

private:
    QFileSystemWatcher watcher;
};

#endif //WATCHER_H
