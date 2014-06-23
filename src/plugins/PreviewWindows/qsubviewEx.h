#pragma once
#include <QWidget>
#include <QVariantMap>
#include "QSubviewRun.h"
#include <ManageWidget.h>
#include <QDebug>
#include <QEventLoop>
#include <QMouseEvent>
#include <QMenu>
#include <QTranslator>
#include <QPixmap>
#include <QPen>
#include <QPainter>
#include <QTimer>
int cbStateChangeEx(QString evName,QVariantMap evMap,void*pUser);
int cbRecordStateEx(QString evName,QVariantMap evMap,void*pUser);

class qsubviewEx:public QWidget
{
	Q_OBJECT
public:
	qsubviewEx(QWidget *parent=0);
	~qsubviewEx();
	
public:
	virtual void paintEvent(QPaintEvent *);
	virtual void mouseDoubleClickEvent(QMouseEvent *);
	virtual void resizeEvent(QResizeEvent *);
	virtual void mousePressEvent(QMouseEvent *);
	virtual void changeEvent(QEvent *);

	int openPreview(int chlId);
	int closePreview();

	int switchStream();

	int getCurrentConnectStatus();
	int setDevChannelInfo(int chlId);//unnecessary
	QVariantMap getWindowInfo();

	//�ֶ�¼��
	int startRecord();
	int stopRecord();
	//��Ƶ ����
	int setPlayWnd(int nwnd);//unnecessary
	int setVolume(unsigned int uiPersent);
	int audioEnabled(bool bEnable);
	//����
	QVariantMap screenShot();
	//��̨����
	int openPTZ(int ncmd,int nspeed);
	int closePTZ(int ncmd);
	//��������
	void loadLanguage(QString tags);
	//���õ�ǰ���㴰��
	void setCurrentFocus(bool flags);
public:
	//�ص�����
	int cbCStateChange(QVariantMap evMap);
	int cbCRecordState(QVariantMap evMap);

public slots:
	void slbackToMainThread(QVariantMap evMap);
	void slmouseMenu();
	void slswitchStreamEx();
	void slclosePreview();
signals:
	void sgbackToMainThread(QVariantMap evMap);
	void sgmouseDoubleClick(QWidget *,QMouseEvent*);
	void sgmousePressEvent(QWidget*,QMouseEvent*);
	void sgmouseLeftClick(QWidget*,QMouseEvent *);
	void sgrecordState(bool);
	void sgmouseMenu();
	void sgconnectStatus(QVariantMap,QWidget *);
private:
	void paintEventConnected(QPaintEvent *ev);
	void paintEventDisconnected(QPaintEvent *ev);
	void paintEventConnecting(QPaintEvent *ev);
	void paintEventDisconnecting(QPaintEvent *ev);
	void getDeviceInfo();
	QString getLanguageInfo(QString tags);
private:
	typedef enum __tagConnectStatus{
		STATUS_CONNECTED,
		STATUS_CONNECTING,
		STATUS_DISCONNECTED,
		STATUS_DISCONNECTING,
	}tagConnectStatus;
	tagConnectStatus m_tCurConnectStatus;
	tagConnectStatus m_tHistoryConnectStatus;
	QSubviewRun m_sSubviewRun;
	ManageWidget *m_pManageWidget;
	bool m_bIsFocus;
	int m_nchlid;
	QMenu m_mRightMenu;
	QAction *m_pClosePreviewAction;
	QAction *m_pSwitchStreamAciton;
	tagDeviceInfo m_tDeviceInfo;
	QTranslator *m_pTtanslator;
	int m_nConnectingCount;
	QTimer m_tConnectingTimer;
};
