#ifndef WIDGET_H
#define WIDGET_H
/*
 * 这个程序写于2019年10月1日，调用百度API实现在线翻译
 * 本人于2020年10月1日重构了该代码，以便运行在linux修改的部分为以及一些内存泄漏：
 * 添加了一个实时检测网络是否在线的线程CNetworkDetectionThread 未来可能会实现离线翻译
 * 更改了一些全局定义的变量
 * 编译环境qt5.9.9 MinGW 32bit
 *
 *  by anjj 20201001
 *
*/


#include <QWidget>
#include<QCryptographicHash>//md5加密的库
#include<QDebug>
#include<QtNetwork>
#include<QJsonObject>
#include<QJsonDocument>
#include<QDesktopServices>
#include<QAction>


#include"CNetworkDetectionThread.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


private slots:
    void clear_text(); //清除显示

    void chinese_butt();//翻译中文

    void english_butt();//翻译英文

    int function_data();//翻译

    int replyFinished(QNetworkReply *reply);//获得翻译返回的结果

    void on_pushButton_2_clicked();//一个访问网站的按钮

    void on_pushButton_clicked();//一个访问网站的按钮

    /*
     * onNetworkStatus  动态检测网络状态线程信号函数
     * nRet             检测结果0网络正常 1网络掉线
     * cStatus          检测结果中文
     */
    void onNetworkStatus(int nRet,QString cStatus);
private:
    Ui::Widget *ui;
    QString from;
    QString to;
    CNetworkDetectionThread * m_NetworkDetectionThread;
    QNetworkAccessManager * m_Manager;
};

#endif // WIDGET_H
