#include "mdworkobject.h"
#include "bubbleprotocolex.h"

MDWorkObject::MDWorkObject(QObject * parent)
	: QThread(parent)
{
}

MDWorkObject::~MDWorkObject()
{
	stopMd();
}

void MDWorkObject::registerEvent( QString sEvent,EventProc proc,void * pUser )
{
	EventMap mapTemp;
	mapTemp.proc = proc;
	mapTemp.pUser = pUser;
	m_eventMap.insert(sEvent,mapTemp);
}

void MDWorkObject::eventProcCall( QString sEvent,QVariantMap eParam )
{
	if (m_eventMap.contains(sEvent))
	{
		EventMap mapTemp = m_eventMap[sEvent];
		mapTemp.proc(sEvent,eParam,mapTemp.pUser);
	}
}

void MDWorkObject::setHostInfo( QString sAddress,unsigned int uPort )
{
	m_sAddress = sAddress;
	m_uPort = uPort;
}

void MDWorkObject::setUserInfo( QString sUsername,QString sPassword )
{
	m_sUsername = sUsername;
	m_sPassword = sPassword;

	// for test
/*	m_sUsername = QString("admin");
	m_sPassword = QString("");*/
}

int MDWorkObject::startMd()
{
	m_bQuitMd = false;

	start();
	return 0;
}

int MDWorkObject::stopMd()
{
	m_bQuitMd = true;
	while (isRunning())
	{
		msleep(50);
	}
	return 0;
}

int MDWorkObject::mdWorkProc(QTcpSocket * s)
{
	// �����Ҫ�������������̣߳��򷵻�0�������Ҫ�����������򷵻�-1


	char sRecvBuffer[1024]; // ���ܻ���
	QString sPack; // ֡����
	QString sLR; // ��β��������ͨ������ж���β��������\n����\r\n���������ֽ�����
	QString sContent; // httpЭ�鸺������

	QDateTime last = QDateTime::currentDateTimeUtc();


	while (!m_bQuitMd)
	{
		msleep(20);
		if (QDateTime::currentDateTimeUtc().toMSecsSinceEpoch() - last.toMSecsSinceEpoch() > (qint64)1000)
		{
			last = QDateTime::currentDateTimeUtc();
			// �����û�������
			QString sUserandPwd;
			sUserandPwd = m_sUsername + QString(":") + m_sPassword;
			QString sAuthorization(sUserandPwd.toAscii().toBase64());

			// �����ַ���
			QString sReq;
			sReq.clear();
			sReq += QString("GET /NetSDK/Video/motionDetection/channel/1/status HTTP/1.1\r\n");
			sReq += QString("Host: ") + m_sAddress + QString("\r\n");
			sReq += QString("Connection: keep-alive\r\n");
			sReq += QString("Authorization: Basic ") + sAuthorization + QString("\r\n");
			sReq += QString("Cookie: juanipcam_lang=zh-cn\r\n\r\n");


			// ��������
			qint64 nTotleWrite = 0;
			do 
			{
				if (m_bQuitMd)
				{
					// �˳�Mdѭ��
					s->disconnectFromHost();
					s->close();

					return 0;
				}

				qint64 nWrite = s->write(sReq.toAscii().data() + nTotleWrite,sReq.length());
				if (-1 == nWrite)
				{
					// дʧ��,�ر�socket,�˳�����echo
					s->disconnectFromHost();
					s->close();

					// ��Ҫ��������������-1
					return -1;
				}
				nTotleWrite += nWrite;

				if ( ! s->waitForBytesWritten(1000) )
				{
					// д��ʱ,�ر�socket,�˳�����echo
					s->disconnectFromHost();
					s->close();

					// ��Ҫ��������������-1
					return -1;
				}
			} while (nTotleWrite < sReq.length());

			// ��������
			// ����ǰ����ϴεĻ���
			sPack.clear();
			while(1) // to be modified ��Ҫ����ѭ��ǿ���˳�����
			{
				if (m_bQuitMd)
				{
					// �˳�Mdѭ��
					s->disconnectFromHost();
					s->close();

					return 0;
				}

				if (s->waitForReadyRead(50))
				{
					qint64 nRead = s->read(sRecvBuffer,sizeof(sRecvBuffer) - 1); // Ԥ��������λ�ö�ȡ
					if (-1 == nRead)
					{
						// ��ʧ�ܣ��ر�socket���˳�����echo
						s->disconnectFromHost();
						s->close();

						// ��Ҫ��������,����-1
						return -1;
					}

					sRecvBuffer[nRead] = 0;
					sPack += QString(sRecvBuffer);

					// ���httpͷ�Ƿ��������,\r\n\r\n����\n\n,δ�������򷵻�ѭ���������ܣ��ѽ����������ִ�к�������
					sLR = QString("\r\n");
					if (!sPack.contains(QString("\r\n\r\n")))
					{
						// δ�ҵ�\r\n����β
						sLR = QString("\n");
						if (!sPack.contains(QString("\n\n")))
						{
							// δ�ҵ�\n����β��ͷδ����
							// �¸�ѭ����������
							continue;
						}
					}

					// ��ȡcontent-length�ֶ�
					int nContentLength = 0;
					if (sPack.contains(QString("content-length"),Qt::CaseInsensitive))
					{
						int nContentLengthStart = sPack.indexOf(QString("content-length"),0,Qt::CaseInsensitive);
						int nContentLengthEnd = sPack.indexOf(sLR,nContentLengthStart);
						QString sContentLength = sPack.mid(nContentLengthStart,nContentLengthEnd - nContentLengthStart);
						sContentLength.remove(QChar(' '));
						nContentLength = sContentLength.mid(sContentLength.indexOf(QChar(':')) + 1).toInt();
					}

					if (0 != nContentLength)
					{
						// �����д�����
						sContent = sPack.mid(sPack.indexOf(sLR + sLR) + QString(sLR + sLR).length());
						if (sContent.length() == nContentLength)
						{
							// �������
							// �˳�����ѭ��
							break;
						}
						else if (sContent.length() > nContentLength)
						{
							// ���ݲ���ȷ
							s->disconnectFromHost();
							s->close();

							// ��Ҫ��������������-1
							return -1;
						}
						else
						{
							// ��Ҫ��������
							continue;
						}
					}
					else
					{
						// ��content���������
						// �˳�����ѭ��
						break;
					}
				}
			}

			// �������ݽ���
			QString sFirstLine = sPack.left(sPack.indexOf(sLR));
			if (sFirstLine.contains(QString("200")))
			{
				// 200 OK
				if (sContent == "true")
				{
					QVariantMap eparam;
					eparam.insert("signal",QVariant(true));
					eventProcCall("mdsignal",eparam);
				}
				else if (sContent == "false")
				{
					QVariantMap eparam;
					eparam.insert("signal",QVariant(false));
					eventProcCall("mdsignal",eparam);
				}
			}
			else if (sFirstLine.contains(QString("404")))
			{
				// 404 page not found
			}
		}
	}

	// ��Ҫ���������ĵط�����ѭ����ֱ�ӷ���-1���������������Ҫ�����ƶ�������������
	if (s->isOpen())
	{
		s->close();
	}
	return 0;
}

void MDWorkObject::run()
{
	typedef enum _enMdThreadStatus{
		MTS_INIT,
		MTS_WORK,
		MTS_TERMINAT,
	}MdThreadStatus;
	MdThreadStatus nThreadStatus = MTS_INIT;

	// ����socket����MTS_TERMINATEʱ����
	QTcpSocket * s = new QTcpSocket;

	while (!m_bQuitMd)
	{
		switch(nThreadStatus)
		{
		case MTS_INIT:
			{
				// ��ʼ��socket,�������ӡ��������õȹ���
				if (NULL != s)
				{
					int nRet = InitSocket(s);
					if (0 == nRet)
					{
						nThreadStatus = MTS_WORK;
					}
				}
			}
			break;
		case MTS_WORK:
			{
				// ����socket,��ʼ����
				// �����з���0�������˳�������-1������
				if (NULL != s)
				{
					int nRet = mdWorkProc(s);
					if (0 == nRet)
					{
						nThreadStatus = MTS_TERMINAT;
					}
					else
					{
						nThreadStatus = MTS_INIT;
					}
				}
			}
			break;
		case MTS_TERMINAT:
			{
				// �ͷ�socketȻ���˳�
				delete s;
				return;
			}
			break;
		default:
			break;
		}
	}
}

int MDWorkObject::InitSocket(QTcpSocket * s)
{
	// ������������0,����ʧ�ܷ���-1
	// ����
	s->connectToHost(m_sAddress,m_uPort,QIODevice::ReadWrite);
	if (!s->waitForConnected(5000))
	{
		qDebug("[%s:%d] %s:%s",__FILE__,__LINE__,__FUNCTION__,s->errorString());
		return -1;
	}

	// keep alive
	s->setSocketOption(QAbstractSocket::KeepAliveOption,QVariant(1));

	// nodelay
	s->setSocketOption(QAbstractSocket::LowDelayOption,QVariant(1));

	return 0;
}