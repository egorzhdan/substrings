#ifndef FILESEARCHER_H
#define FILESEARCHER_H

#include <QRunnable>
#include <QSet>
#include "Searcher.h"

class FileSearcher : public QRunnable {
public:
    FileSearcher(const QString& filePath, const QSet<Index::HashWord>& trigrams, Searcher* parent);

protected:
    void run() override;

private:
    const QString& filePath;
    const QSet<Index::HashWord>& trigrams;
    Searcher* parent;

    bool contains(const QString &filePath, const QSet<Index::HashWord> &trigrams);
};

#endif //FILESEARCHER_H
