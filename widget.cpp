#include "widget.h"
#include "ui_widget.h"

#define MY_APID 20190930000338348
#define MY_APID_KEY "haynzrywFQvJHuFYXRaJ"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("安同学的翻译官");//设置标题栏文字
    this->setWindowIcon(QIcon("://photo/p.png"));//设置标题栏左上角图片
    ui->tcp_lineEdit->setEnabled(false);//设置成不可输入

    m_NetworkDetectionThread = new CNetworkDetectionThread();//动态检测网络状态线程

    if(m_NetworkDetectionThread)
    {
         connect(m_NetworkDetectionThread,&CNetworkDetectionThread::sNetworkStatus,this,&Widget::onNetworkStatus);
         m_NetworkDetectionThread->start();//启动线程
    }

    m_Manager=new QNetworkAccessManager(this);
    if(m_Manager)
    {
        connect(m_Manager,&QNetworkAccessManager::finished,this,&Widget::replyFinished);
    }
    connect(m_NetworkDetectionThread,&CNetworkDetectionThread::sNetworkStatus,this,&Widget::onNetworkStatus);
    connect(ui->chinese_Button,&QRadioButton::toggled,this,&Widget::chinese_butt);//中英文切换
    connect(ui->english_Button,&QRadioButton::toggled,this,&Widget::english_butt);//中英文切换
    connect(ui->query_Button,&QPushButton::clicked,this,&Widget::function_data);//查询
    connect(ui->clear_Button,&QPushButton::clicked,this,&Widget::clear_text);//清除屏幕
    connect(ui->close_Button,&QPushButton::clicked,this,&Widget::close);//关闭程序
    ui->chinese_Button->toggle();


}
void Widget::clear_text()
{
    ui->text->clear();
    ui->text_send->clear();
}


void Widget::chinese_butt()
{
    from="zh";
    to="en";
   // ui->query_Button->setEnabled(true);
}

void Widget::english_butt()
{
    from="en";
    to="zh";
   // ui->query_Button->setEnabled(true);
}

int Widget::function_data()
{
    QString MD5;
    char salt[60];
    sprintf(salt,"%d",rand());  //获取随机数
    QString cText=ui->text->text(); //获取代翻译的文本
    QString sign=QString("%1%2%3%4").arg(MY_APID).arg(cText).arg(salt).arg(MY_APID_KEY);//连接加密文件
    // qDebug ()<<sign;
    QByteArray str = QCryptographicHash::hash(sign.toUtf8(),QCryptographicHash::Md5);
    MD5.append(str.toHex());//生成md5加密文件
    // qDebug()<<MD5;
    QString myurl=QString("http://api.fanyi.baidu.com/api/trans/vip/translate?"
                   "q=%1&from=%2&to=%3&appid=%4"
                   "&salt=%5&sign=%6").arg(cText).arg(from).arg(to).arg(MY_APID).arg(salt).arg(MD5);//连接上传文本
    //qDebug()<<myurl;
    m_Manager->get(QNetworkRequest(QUrl(myurl)));//发送上传；

return 1;

}
int Widget::replyFinished(QNetworkReply *reply)
{

    QJsonParseError jsonError;
    QByteArray all=reply->readAll();//获得api返回值
   // qDebug()<<all;
    QJsonDocument json = QJsonDocument::fromJson(all, &jsonError);
    QJsonObject object = json.object();//jion转码；
    QString cResult;
    if(object.contains("error_code"))
    {
        int nResult=object.value("error_code").toInt();

        switch (nResult) {
        case 52001:
            cResult ="52001 请求超时 重试";
            break;
        case 52002:
            cResult ="52002 系统错误 重试";
            break;
        case 54000:
            cResult ="54000 必填参数为空";
            break;
        case 54001:
            cResult ="54001 签名错误";
            break;
        case 54003:
            cResult ="54003 速度过快访问频率受限";
            break;
        case 54004:
            cResult ="54004 账户余额不足";
            break;
        case 54005:
            cResult ="54005 请求频繁";
            break;
        case 58002:
            cResult ="58002 服务关闭";
            break;
        default:
            cResult ="其他错误";
            break;
        }
    }
    else {
        QJsonArray value = object.value("trans_result").toArray();//一次解码
        /*
         {"from":"en","to":"zh",
           "trans_result":[{"src":"apple","dst":"\u82f9\u679c"}]}
           第一次解码
         */
        QJsonObject object1=value.at(0).toObject();//二次解码开【】括号；
        /*
           {"src":"apple","dst":"\u82f9\u679c"}
           第二次解码
         */
        //from=object.value("from").toString();
        cResult=object1.value("dst").toString();//得到翻译结果
    }

    ui->text_send->setText(cResult);
    reply->deleteLater();
    return 1;

}

Widget::~Widget()
{
    if(m_NetworkDetectionThread)
    {
       delete m_NetworkDetectionThread;
       m_NetworkDetectionThread = NULL ;
    }

    delete ui;
}

void Widget::on_pushButton_2_clicked()//一个访问网站的按钮
{
    QDesktopServices::openUrl
            (QUrl(QLatin1String("https://fanyi.baidu.com/translate?smartresult=dict")));
}

void Widget::on_pushButton_clicked()
{
    QDesktopServices::openUrl
            (QUrl(QLatin1String("https://me.csdn.net/luck_anan")));
}

void Widget::onNetworkStatus(int nRet, QString cStatus)
{
     ui->tcp_lineEdit->setText(cStatus);
}

