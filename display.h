#ifndef DISPLAY_H
#define DISPLAY_H
#include "serveur.h"
#include "dataclass.h"
#include <QApplication>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QGroupBox>
#include <QTimer>

class Display : public QMainWindow
{
    Q_OBJECT

    public:
        Display(QWidget *parent = 0);
        ~Display();
        DataClass m_dataClass;
        QLabel *m_infoExamLabel;
        void endOfExam();
    private:
        QLabel *m_welcome, *m_chooseLabel;
        QLabel *m_serverLabel;
        QLineEdit *m_path;
        QPushButton *m_chargeFile, *m_continue1;
        QPushButton *m_startLogin, *m_startExam;
        QWidget *m_centralWidget;
        QGroupBox *m_examLoopGroupBox,*m_loginGroupBox;
        Serveur m_server;
        QLabel *m_stConnected, *m_stWaitingCnx, *m_stReceivedQuestion, *m_stCnxRefused;
        QLabel *m_stWaitingQuestions, *m_numberOfStudents;
        QLabel *m_stDoingExam, *m_stFinishedExam;
        QTimer *m_loginInfoTimer;
        QTimer *m_examLoopTimer;
    public slots:
        void selectFile();
        void displayExamInfo();
        void startLoginStep();
        void startExam();
        void selectStudentsList();
        void refreshLoginInfo();
        void refreshExamLoopInfo();

};

#endif // DISPLAY_H
