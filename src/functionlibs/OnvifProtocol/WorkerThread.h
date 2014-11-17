#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QObject>
#include "OnvifProtocol_global.h"
extern "C"{
#include "nvp_define.h"
#include "minirtsp.h"
};

#pragma comment(lib, "rtsplibpro.lib")

void eventHook(int eventType, int lParam, void *rParam, void *customCtx);
void dataHook(void *pdata, uint32_t dataSize, uint32_t timestamp, char *dataType, void *customCtx);
class WorkerThread : public QObject
{
	Q_OBJECT

public:
	WorkerThread();
	~WorkerThread();
	ConnectStatus getCurrentStatus();
	void setDeviceInfo(const DeviceInfo& devInfo);
	void recFrameData(void* pdata, uint32_t size, uint32_t timestamp, char* datatype);
public slots:
	int ConnectToDevice(int *ret);
	int Authority(int *ret);
	int Disconnect(int *ret);
	int GetLiveStream(int chl, int streamId, int *ret);
	int PauseStream(int *ret);
	int StopStream(int *ret);
	int GetStreamCount(int *count);
	int GetStreamInfo(int nStreamId, QVariantMap& info, int *ret);
	void setEventMap(const QMultiMap<QString,tagOnvifProInfo> &tEventMap);
	int PtzCtrl(NVP_PTZ_CMD cmd, int chl, int speed, bool bopen, int *ret);
private:
	QMultiMap<QString,tagOnvifProInfo> m_tEventMap;
	ConnectStatus m_enStatus;
	DeviceInfo m_tDeviceInfo;
	lpMINIRTSP m_rtspContext;
	lpMINIRTSP m_nvpVerify;
	lpNVP_INTERFACE m_nvpContext;
	stNVP_ARGS m_nvpArguments;
	stNVP_RTSP_STREAM m_nvpStreamUrl;
	stNVP_VENC_CONFIGS m_nvpStreamInfo;
};

#endif // WORKERTHREAD_H
