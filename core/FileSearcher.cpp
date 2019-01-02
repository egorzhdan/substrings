#include "FileSearcher.h"

FileSearcher::FileSearcher(const QString& filePath, const QSet<Index::HashWord>& trigrams, Searcher *parent) : filePath(filePath), trigrams(trigrams), parent(parent) {}

void FileSearcher::run() {
    if (contains(filePath, trigrams)) {
        emit parent->matchFound(filePath);
    }
}

bool FileSearcher::contains(const QString &filePath, const QSet<Index::HashWord> &trigrams) {
    for (auto &x : trigrams) {
        const auto &hash = parent->index.get(filePath);

        if (!std::binary_search(hash.begin(), hash.end(), x)) {
            return false;
        }
    }
    return true;
}