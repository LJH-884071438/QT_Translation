#include "CNetworkDetectionThread.h"

CNetworkDetectionThread::CNetworkDetectionThread()
{
    liveIP = "14.215.177.39";//设置ip
    livePort = 80;//设置端口
    m_State = true;
}

CNetworkDetectionThread::~CNetworkDetectionThread()
{
    setThreadRun();//设置标志位线程停止
    wait();//等待线程停止
    quit();//退出线程
}

void CNetworkDetectionThread::run()
{
    for(;;)
    {
        if(!m_State)//线程退出标志位；
        {
            break;
        }
        if(IsWebOk())
        {
           emit sNetworkStatus(NETWORK_CONNECTED,"网络连接正常");//发送信号
        }else
        {
           emit sNetworkStatus(NETWORK_LONGTIME,"网络连接超时");//发送信号
        }
         sleep(10);//10秒刷新一次
    }
     tcpClient.close();//关闭tcplian连接
}

void CNetworkDetectionThread::setThreadRun(bool bState)
{
    m_State = bState;
}

bool CNetworkDetectionThread::IsWebOk()
{
    tcpClient.abort();
    tcpClient.connectToHost(liveIP, livePort);
    //100毫秒没有连接上则判断不在线
    return tcpClient.waitForConnected();
}
