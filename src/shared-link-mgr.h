#ifndef _SEAF_SHARED_LINK_MGR_H
#define _SEAF_SHARED_LINK_MGR_H

#include <QObject>

#include "utils/singleton.h"

class SeafileApiRequest;
class GetSharedLinkRequest;
class CreateShareLinkRequest;
class DeleteSharedLinkRequest;
struct SharedLinkInfo;
struct SharedLinkRequestParams;


class SharedLinkManager : public QObject
{
    SINGLETON_DEFINE(SharedLinkManager)
    Q_OBJECT
public:
    SharedLinkManager();
    ~SharedLinkManager();

public slots:
    void generateShareLink(SharedLinkRequestParams& params);

signals:

private slots:
    void onGetSharedLinkSuccess(const SharedLinkInfo& shared_link_info);
    void onGetSharedLinkFailed();
    void onGetSharedLinkEmpty();
    void onDeleteSharedLinkSuccess();
    void onDeleteSharedLinkFailed();
    void onCreateSharedLinkSuccess(const SharedLinkInfo& shared_link_info);
    void onCreateSharedLinkFailed();

private:
    Q_DISABLE_COPY(SharedLinkManager)

    void getSharedLink(const SharedLinkRequestParams& params);
    void deleteSharedLink(const SharedLinkRequestParams& params, const QString &token);
    void createSharedLink(const SharedLinkRequestParams& params);

    QList<SeafileApiRequest*> reqs_;
};

#endif  // _SEAF_SHARED_LINK_MGR_H
