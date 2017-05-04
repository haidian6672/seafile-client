#include <QDir>
#include <sqlite3.h>
#include <errno.h>
#include <stdio.h>

#include <QDateTime>
#include <QCache>

#include "utils/file-utils.h"
#include "utils/utils.h"
#include "configurator.h"
#include "seafile-applet.h"
#include "data-cache.h"

namespace {

const int kDirentsCacheExpireTime = 60 * 1000;

} // namespace

SINGLETON_IMPL(DirentsCache)
DirentsCache::DirentsCache()
{
    cache_ = new QCache<QString, CacheEntry>;
}
DirentsCache::~DirentsCache()
{
    delete cache_;
}

DirentsCache::ReturnEntry
DirentsCache::getCachedDirents(const QString& repo_id,
                               const QString& path)
{
    QString cache_key = repo_id + path;
    CacheEntry *e = cache_->object(cache_key);
    if (e != NULL) {
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        if (now < e->timestamp + kDirentsCacheExpireTime) {
            return ReturnEntry(e->current_readonly, &(e->dirents));
        }
    }

    return ReturnEntry(false, NULL);
}

void DirentsCache::expireCachedDirents(const QString& repo_id, const QString& path)
{
    cache_->remove(repo_id + path);
}

void DirentsCache::saveCachedDirents(const QString& repo_id,
                                     const QString& path,
                                     bool current_readonly,
                                     const QList<SeafDirent>& dirents)
{
    CacheEntry *val = new CacheEntry;
    val->timestamp = QDateTime::currentMSecsSinceEpoch();
    val->current_readonly = current_readonly;
    val->dirents = dirents;
    QString cache_key = repo_id + path;
    cache_->insert(cache_key, val);
}

SINGLETON_IMPL(FileCache)
FileCache::FileCache()
{
    cache_ = new QCache<QString, CacheEntry>;
}

FileCache::~FileCache()
{
    delete cache_;
}

QString FileCache::getCachedFileId(const QString& repo_id,
                                     const QString& path)
{
    return getCacheEntry(repo_id, path).file_id;
}

FileCache::CacheEntry FileCache::getCacheEntry(const QString& repo_id,
                                                   const QString& path)
{
    FileCache::CacheEntry ret;
    QString cache_key = repo_id + path;
    FileCache::CacheEntry *cache_entry = cache_->object(cache_key);

    if (cache_entry) {
        ret.repo_id = cache_entry->repo_id;
        ret.path = cache_entry->path;
        ret.file_id = cache_entry->file_id;
        ret.account_sig = cache_entry->account_sig;
    }

    return ret;
}

void FileCache::saveCachedFileId(const QString& repo_id,
                                   const QString& path,
                                   const QString& file_id,
                                   const QString& account_sig)
{
    CacheEntry *val = new CacheEntry;
    QString cache_key = repo_id + path;

    val->repo_id = repo_id;
    val->path = path;
    val->file_id = file_id;
    val->account_sig = account_sig;

    cache_->insert(cache_key, val);
}

QList<FileCache::CacheEntry> FileCache::getAllCachedFiles()
{
    QList<CacheEntry> list;
    CacheEntry entry;

    foreach(const QString& cache_key, cache_->keys()) {
        entry.repo_id = cache_->object(cache_key)->repo_id;
        entry.path = cache_->object(cache_key)->path;
        entry.file_id = cache_->object(cache_key)->file_id;
        entry.account_sig = cache_->object(cache_key)->account_sig;
        list.append(entry);
    }

    return list;
}
