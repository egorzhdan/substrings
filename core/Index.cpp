#include <QMutexLocker>
#include <QDebug>
#include "Index.h"

Index::Index() = default;
Index::Index(const Index &other) : map(other.map) {}

void Index::insert(const QString &filePath, const Index::FileHash &fileHash) {
    lock.lockForWrite();
    try {
        map[filePath] = fileHash;
    } catch (...) {
        lock.unlock();
        throw;
    }
    lock.unlock();
}

Index::FileHash Index::get(const QString &filePath) const {
    lock.lockForRead();
    FileHash result;
    try {
        result = map[filePath];
    } catch (...) {
        lock.unlock();
        throw;
    }
    lock.unlock();
    return result;
}

QList<QString> Index::filePaths() const {
    return map.keys();
}

Index &Index::operator=(const Index &other) {
    this->map = other.map;
    return *this;
}

void Index::removeFile(const QFile &root) {
    qDebug() << "Index: removing under " << root.fileName();
    QMutableHashIterator it(map);
    while (it.hasNext()) {
        auto cur = it.next();
        if (cur.key().startsWith(root.fileName())) {
            qDebug() << "Index: removing " << cur.key();
            it.remove();
        }
    }
}
