#ifndef INDEX_H
#define INDEX_H

#include <QString>
#include <QVector>
#include <QHash>
#include <QReadWriteLock>

class Index {
public:
    typedef uint64_t HashWord;
    typedef QVector<HashWord> FileHash;

    Index();
    Index(const Index& other);
    Index& operator=(const Index& other);

    void insert(const QString& filePath, const Index::FileHash &fileHash);
    FileHash get(const QString& filePath) const;
    QList<QString> filePaths() const;

private:
    mutable QReadWriteLock lock;
    QHash<QString, FileHash> map;
};


#endif //INDEX_H
