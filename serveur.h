#ifndef SERVEUR_H
#define SERVEUR_H

#include "question.h"
#include "student.h"
#include <QObject>
#include <QDataStream>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QDebug>
#include <cstdlib>
#include <ctime>
#include <QMap>
#include <QString>

class Serveur : public QObject
{
    Q_OBJECT

    public:
        explicit Serveur(QObject *parent = nullptr);
        void sendQuestions();
        void startLoginStep(QVector<Question>,quint16);
        void starExam();
        bool startTcpServer();
        void getLoginInfo();
        void getExamLoopInfo();
        void endOfExam();
        QString tcpMsgError();
        QVector<int> m_okStudentList;
        QMap<QString,QString> m_loginInfoTab;
        QMap<QString,QString> m_examLoopInfoTab;
        QMap<quint32,quint16> m_marks;
        QMap<QString,QString> getCnxInfos();

    private:
        QTcpServer *m_tcpServer;
        QList<Student*> m_clients;
        QVector<Question> m_questionnaire;
        quint16 m_numberQuestions;
        int m_allowedToPassExam;
        int m_notAllowedToPassExam;
        int m_stDoingExam;
        int m_stFinishedExam;
    public slots:

        void nouvelleConnexion();
        void dataReceived();
        void disconnectClient();

};

#endif // SERVEUR_H
