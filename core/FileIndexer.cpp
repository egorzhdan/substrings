#include <utility>

#include "FileIndexer.h"
#include <QDir>
#include <QVector>
#include <QtCore>
#include <QCryptographicHash>
#include <iostream>
#include <memory>
#include <unordered_set>

FileIndexer::FileIndexer(Indexer *parent, QString filePath) : parent(parent), filePath(std::move(filePath)) {}

void FileIndexer::run() {
    qDebug() << "Initial indexing: " << filePath;

    auto trigrams = calcHash(filePath);
    parent->index.insert(filePath, trigrams);
    parent->count++;
}

template <size_t SIZE>
Index::HashWord calcHash(const std::array<Index::HashWord, SIZE> &word) {
    Index::HashWord hash = 0;
    for (size_t i = 0; i < SIZE; i++) {
        hash ^= (word[i] << (10 * i));
    }
    return hash;
}

void calcHashes(const QString &s, QSet<Index::HashWord> &result) {
    if (s.isEmpty()) {
        result.insert(0);
        return;
    }
    if (s.size() == 1) {
        result.insert(calcHash(std::array<Index::HashWord, 1>{s[0].unicode()}));
        return;
    }
    if (s.size() == 2) {
        result.insert(calcHash(std::array<Index::HashWord, 2>{s[0].unicode(), s[1].unicode()}));
        return;
    }

    std::array<Index::HashWord, 3> tmp{};
    for (auto i = 0; i < s.size() - 2; ++i) {
        tmp[0] = s[i].unicode();
        tmp[1] = s[i + 1].unicode();
        tmp[2] = s[i + 2].unicode();
        result.insert(calcHash(tmp));
    }
}

Index::FileHash calcHash(const QString &filePath) {
    QSet<Index::HashWord> result;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        QString buffer;
        while (true) {
            buffer.append(stream.read(4 << 20));
            if (buffer.size() < 3) {
                break;
            }
            calcHashes(buffer, result);
            if (result.empty()) {
                break;
            }
            buffer = buffer.mid(buffer.size() - 2, 2);
        }
        file.close();
    }

    std::vector<Index::HashWord> tmp(result.begin(), result.end());
    std::sort(tmp.begin(), tmp.end());
    return Index::FileHash::fromStdVector(tmp);
}
