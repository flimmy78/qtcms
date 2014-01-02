#ifndef __RPLAYBACKWND_H__
#define __RPLAYBACKWND_H__

#include <QWidget>
#include <QMutex>
#include <QtNetwork/QHostAddress>
#include "qwfw.h"
#include "IWindowDivMode.h"
#include "IDeviceRemotePlayback.h"
#include "IDeviceClient.h"
#include "rSubview.h"

class RPlaybackWnd : public QWidget,
	public QWebPluginFWBase
{
	Q_OBJECT

public:
	RPlaybackWnd(QWidget *parent = 0);
	~RPlaybackWnd();

	virtual void resizeEvent( QResizeEvent * );

public slots:
	void AddEventProc( const QString sEvent,QString sProc ){m_mapEventProc.insertMulti(sEvent,sProc);};

    // �����豸������Ϣ
    int setDeviceHostInfo(const QString & sAddress,unsigned int uiPort,const QString &eseeID);
    // �����豸������Ϣ
    int setDeviceVendor(const QString & vendor);
    // ���ûط�ͬ����
    int AddChannelIntoPlayGroup(uint uiWndId,unsigned int uiChannel);

    void setUserVerifyInfo(const QString & sUsername,const QString & sPassword);

	int startSearchRecFile(int nChannel,int nTypes,const QString & startTime,const QString & endTime);
    QString getGroupPlayedTime();

    int   GroupPlay(int nTypes,const QString & start,const QString & end);
    int   GroupPause();
    int   GroupContinue();
    int   GroupStop();
    bool  GroupEnableAudio(bool bEnable);
    int   GroupSpeedFast() ;
    int   GroupSpeedSlow();
    int   GroupSpeedNormal();
public:
    int   GetCurrentWnd();
    void  CurrentStateChangePlugin(int stateValue);
    void  SocketErrorPlugin(int stateValue);
    int   GetRecFileNum(uint uiNum);
private slots:
    void  OnSubWindowDblClick(QWidget *,QMouseEvent *);
    void  SetCurrentWind(QWidget *);
private:
	RSubView         m_PlaybackWnd[4];
	IWindowDivMode * m_DivMode;
    IDeviceGroupRemotePlayback * m_GroupPlayback;
    IDeviceClient *  m_DeviceClient;
	QList<QWidget *> m_PlaybackWndList;
    QList<QVariantMap>m_SelectedRecList;

    QHostAddress  m_HostAddress;
    QString       m_sEseeId;
    QString       m_sVendor;
    QString       m_sUserName;
    QString       m_sUserPwd;
    uint          m_uiPort;
    int           m_nCurrentWnd;
    uint          m_uiRecFileSearched;
private:
    bool bIsInitFlags;
    bool bIsCaseInitFlags;
    int  cbInit();
};


#endif // __RPLAYBACKWND_H__