#ifndef FILE_INDEXER_H
#define FILE_INDEXER_H

#include <QDir>
#include <unordered_set>
#include <QRunnable>
#include "Indexer.h"

class FileIndexer : public QRunnable {
public:
    FileIndexer(Indexer *parent, QString filePath);

protected:
    void run() override;

private:
    friend class Indexer;

    Indexer *parent;
    QString filePath;
};

void calcHashes(const QString &s, QSet<Index::HashWord> &result);
Index::FileHash calcHash(const QString &filePath);

#endif //FILE_INDEXER_H
