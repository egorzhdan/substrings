#ifndef SEARCHER_H
#define SEARCHER_H

#include <QThread>
#include "Index.h"
#include "FileIndexer.h"

class Searcher : public QThread {
Q_OBJECT

public:
    Searcher(Index index, QString query, QObject *parent);

signals:
    void statusUpdated(QString status, bool busy = true);
    void matchFound(QString filePath);

protected:
    void run() override;

private:
    friend class FileSearcher;

    Index index;
    QString query;
    QThreadPool *threadPool;
};

#endif //SEARCHER_H
