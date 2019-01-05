#include <utility>
#include <QSet>
#include "Searcher.h"
#include "FileIndexer.h"
#include "FileSearcher.h"

Searcher::Searcher(Index index, QString query, QObject *parent) : QThread(parent), index(index), query(std::move(query)) {
    threadPool = new QThreadPool(parent);
    threadPool->setMaxThreadCount(4);
}

void Searcher::run() {
    emit statusUpdated("Searching...");
    QSet<Index::HashWord> trigrams;
    calcHashes(query, trigrams);
    if (isInterruptionRequested()) {
        return;
    }

    auto paths = index.filePaths();

    for (const auto& path : paths) {
        threadPool->start(new FileSearcher(path, trigrams, this));

        if (isInterruptionRequested()) {
            threadPool->clear();
            break;
        }
    }
    threadPool->waitForDone();
    emit statusUpdated("Ready");
}
