#include <QDebug>
#include <QDirIterator>
#include "Indexer.h"
#include "FileIndexer.h"

Indexer::Indexer(QObject *parent, QDir root) : QThread(parent), root(root) {
    threadPool = new QThreadPool(parent);
    threadPool->setMaxThreadCount(4);
}

QString Indexer::rootPath() {
    return root.path();
}

void Indexer::run() {
    emit statusUpdated("Indexing...");
    count = 0;

    auto rootPath = (shouldReindexFile ? filePathToReindex : root.absolutePath());
    auto it = QDirIterator(rootPath, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        auto path = it.next();
        threadPool->start(new FileIndexer(this, path));
        updateStatus();

        if (isInterruptionRequested()) return;
    }

    threadPool->waitForDone();
    updateStatus();
    emit statusUpdated("Ready", false);
}

void Indexer::cancel() {
    if (updateStatusTimer) {
        updateStatusTimer->stop();
    }
    emit statusUpdated("Cancelling...");
    requestInterruption();
    threadPool->clear();
}

void Indexer::wait() {
    threadPool->waitForDone();
}

Index Indexer::waitForIndex() {
    if (threadPool->activeThreadCount() > 0)
        wait();
    return index;
}

void Indexer::updateStatus() {
    emit statusUpdated("Indexing... " + QString::number(count.load()) + " done");
}

void Indexer::setRootToReindex(const QFile &root) {
    shouldReindexFile = true;
    index.removeFile(root);
    filePathToReindex = root.fileName();
}

Index Indexer::indexNonBlocking() {
    return index;
}