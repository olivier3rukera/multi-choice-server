#include "serveur.h"

Serveur::Serveur(QObject *parent) : QObject(parent)
{
    m_allowedToPassExam=0;
    m_notAllowedToPassExam=0;
    m_stDoingExam=0;
    m_stFinishedExam=0;
    m_tcpServer=new QTcpServer(this);
    m_tcpServer->setMaxPendingConnections(500);
    connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(nouvelleConnexion()));
    srand(time(0));
}


void Serveur::dataReceived()
{
    QTcpSocket *socket=qobject_cast<QTcpSocket *>(sender());
    if(socket==0)
    {
        return;
    }
    Student *currStudent;
    for(auto &stud:m_clients)
    {
        if(stud->tcpSocket==socket)
        {
            currStudent=stud;
        }
    }
    QDataStream in(socket);
    if(currStudent->sizePack==0||currStudent->stepCnx==0)
    {
        if(socket->bytesAvailable()<(int)2*sizeof(quint16))
        {
            return;
        }
        else
        {
            in>>currStudent->sizePack;
            in>>currStudent->stepCnx;
        }
    }


    if(currStudent->stepCnx==1)
    {
        if(socket->bytesAvailable()<sizeof(qint32))
        {
            return;
        }
        in>>currStudent->rollNumber;
        QByteArray packet;
        QDataStream out(&packet,QIODevice::WriteOnly);
        out<<(qint16)0;// size of paquet
        out<<(qint16)2;  // step 2
        bool canPassExam=false;
        if(m_okStudentList.contains(currStudent->rollNumber))
        {
            canPassExam=true;
            out<<canPassExam;// canPassExam

            QVector<int> indices;
            int i=0;
            int siz=m_questionnaire.size();
            while(i<siz)
            {
                indices.push_back(i);
                ++i;
            }

            QVector<Question> questionnaire;
            int pos=0;
            int questTempSize=0;
            while(questTempSize<m_numberQuestions)
            {
                pos=rand() % indices.size();
                questionnaire<<m_questionnaire[pos];
                indices.remove(pos);
                ++questTempSize;
            }
            out<<questionnaire;
        }
        else
        {
            canPassExam=false;
            out<<canPassExam;
        }
        currStudent->sizePack=0;
        currStudent->stepCnx=0;
        out.device()->seek(0);
        out<<(qint16)(packet.size()-2*sizeof(qint16));
        socket->write(packet);
        if(canPassExam)
        {
            ++m_allowedToPassExam;
        }
        else
        {
            ++m_notAllowedToPassExam;
        }
    }
    else if(currStudent->stepCnx==2)
    {
        if(socket->bytesAvailable()<sizeof(quint16))
        {
            return;
        }
        quint16 mark;
        QDataStream in(socket);
        in>>mark;
        m_marks[currStudent->rollNumber]=mark;
        --m_stDoingExam;
        ++m_stFinishedExam;
    }
    else{}

}


QMap<QString,QString> Serveur::getCnxInfos()
{
    QMap<QString,QString> cnxInfos;
    cnxInfos["ip"]=m_tcpServer->serverAddress().toString();
    cnxInfos["port"]=QString::number(m_tcpServer->serverPort());
    return cnxInfos;
}


void Serveur::disconnectClient()
{
    QTcpSocket *socket=qobject_cast<QTcpSocket *>(sender());
    if(socket==0)
    {
        return;
    }
    for (auto &student:m_clients)
    {
        if(student->tcpSocket==socket)
        {
            m_clients.removeOne(student);
        }
    }
    socket->deleteLater();
}


void Serveur::endOfExam()
{

}


void Serveur::getExamLoopInfo()
{
    m_examLoopInfoTab["stDoingExam"]=QString::number(m_stDoingExam);
    m_examLoopInfoTab["stFinishedExam"]=QString::number(m_stFinishedExam);
}


void Serveur::getLoginInfo()
{
    m_loginInfoTab["connected"]=QString::number(m_clients.size());
    m_loginInfoTab["receiveQuestions"]=QString::number(m_allowedToPassExam);
    m_loginInfoTab["refused"]=QString::number(m_notAllowedToPassExam);
}


void Serveur::nouvelleConnexion()
{
    QTcpSocket *newClient=m_tcpServer->nextPendingConnection();
    Student *newStudent=new Student;
    newStudent->tcpSocket=newClient;
    newStudent->stepCnx=0;
    newStudent->rollNumber=0;
    m_clients<<newStudent;
    connect(newClient,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(newClient,SIGNAL(disconnected()),this,SLOT(disconnectClient()));
}


void Serveur::sendQuestions()
{
    QByteArray packet;
    QDataStream out(&packet,QIODevice::WriteOnly);
    out<<(quint16)0;
    out.device()->seek(0);
    out<<(quint16)(packet.size()-sizeof(quint16));

}


void Serveur::starExam()
{
    QByteArray packet;
    QDataStream out(&packet,QIODevice::WriteOnly);
    out<<(qint16)2; // size of pack witch is size of qin16
    out<<(qint16)3;
    for(auto &stud: m_clients)
    {
        stud->tcpSocket->write(packet);
    }
    m_stDoingExam=m_allowedToPassExam;
}


bool Serveur::startTcpServer()
{
    return m_tcpServer->listen(QHostAddress::Any,65432);
}


void Serveur::startLoginStep(QVector<Question> questions,quint16 numberQuestions)
{
    m_questionnaire=questions;
    m_numberQuestions=numberQuestions;
    QByteArray packet;
    QDataStream out(&packet,QIODevice::WriteOnly);
    out<<(qint16)2;
    out<<(qint16)1;
    for(auto &stud: m_clients)
    {
        stud->tcpSocket->write(packet);
    }
}


QString Serveur::tcpMsgError()
{
    return m_tcpServer->errorString();
}
