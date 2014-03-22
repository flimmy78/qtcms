#pragma once
#include <QObject>
#include <QThread>
#include <QDebug>
#include <IDeviceClient.h>
#include <QtNetwork/QHostAddress>
#include <IDeviceRemotePlayback.h>
#include "RemotePlaybackThread.h"
#include <QVariantMap>


class RemotePlaybackObject:public QThread
{
	Q_OBJECT
		QThread m_workerThread;
public:
	RemotePlaybackObject(void);
	~RemotePlaybackObject(void);

public:
	int startSearchRecFile(int nChannel,int nTypes,const QString & startTime,const QString & endTime);
	int SetParm(QString m_sUserName,QString m_sUserPwd,uint m_uiPort,QString m_HostAddress,QString m_sEseeId);
	int SetIDeviceGroupRemotePlaybackParm(IDeviceGroupRemotePlayback *lpIDeviceGroupRemotePlayback);
	int GroupPlay(int nTypes,const QString & startTime,const QString & endTime);
	
	void SetrPlaybackWnd(QWidget *wnd);
signals:
	void startSearchRecFileSignals(int nChannel,int nTypes,const QString & startTime,const QString & endTime);
	void GroupPlaySignals(int nTypes,const QString & startTime,const QString & endTime);
private:
	RemotePlaybackThread *m_RemotePlaybackThread;
	QWidget *rplaybackwnd;
public slots:
	void finishSearchRecFileSlots();
};

