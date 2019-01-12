#include <QTextStream>
#include "FileSearcher.h"

FileSearcher::FileSearcher(const QString& filePath, const QSet<Index::HashWord>& trigrams, Searcher *parent) : filePath(filePath), trigrams(trigrams), parent(parent) {}

void FileSearcher::run() {
    if (contains(filePath, trigrams)) {
        emit parent->matchFound(filePath);
    }
}

bool FileSearcher::contains(const QString &filePath, const QSet<Index::HashWord> &trigrams) {
    const QString &query = parent->query;
    if (query.size() < 3) {
        for (auto &x : trigrams) {
            const auto &hash = parent->index.get(filePath);

            if (!std::binary_search(hash.begin(), hash.end(), x)) {
                return false;
            }
        }
    }

    const auto BUFFER_SIZE = std::max(1000, query.size() + 1);
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        QString oldBuffer = stream.read(BUFFER_SIZE);
        QString newBuffer = stream.read(BUFFER_SIZE);
        do {
            if ((oldBuffer + newBuffer).contains(query)) {
                file.close();
                return true;
            }
            oldBuffer = newBuffer;
            newBuffer = stream.read(BUFFER_SIZE);
        } while (!newBuffer.isEmpty());
    }
    file.close();
    return false;
}