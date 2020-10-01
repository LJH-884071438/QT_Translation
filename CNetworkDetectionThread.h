#ifndef CNETWORKDETECTIONTHREAD_H
#define CNETWORKDETECTIONTHREAD_H

#include <QTcpSocket>
#include <QThread>
#include <QWidget>

class CNetworkDetectionThread:public QThread
{
    Q_OBJECT
public:
    CNetworkDetectionThread();
    ~CNetworkDetectionThread();
    void run();//线程重写run函数


    enum{
        NETWORK_CONNECTED = 0,  //网络正常
        NETWORK_LONGTIME ,   //网络连接超时
    };

    void setThreadRun(bool bState = false);

public slots:

signals:
    void sNetworkStatus(int nRow,QString sStatus);//通知网络状态 0
private:
     bool IsWebOk();
     QTcpSocket tcpClient;
     QString liveIP;
     int livePort;
     bool m_State;
};

#endif // CNETWORKDETECTIONTHREAD_H
