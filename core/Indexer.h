#ifndef INDEXER_H
#define INDEXER_H

#include <QDir>
#include <QThreadPool>
#include <QTimer>
#include "Index.h"

class Indexer : public QThread {
Q_OBJECT

public:
    Indexer(QObject *parent, QDir root);
    QString rootPath();
    void setRootToReindex(const QFile &root);
    void cancel();
    void wait();
    Index waitForIndex();
    Index indexNonBlocking();

signals:
    void statusUpdated(QString status, bool busy = true);

protected:
    void run() override;

private:
    friend class FileIndexer;

    QDir root;
    QString filePathToReindex;
    bool shouldReindexFile = false;
    Index index;
    QThreadPool *threadPool;
    QAtomicInt count;
    QTimer *updateStatusTimer = nullptr;

    void updateStatus();
};


#endif //INDEXER_H
