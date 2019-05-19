#include "display.h"

Display::Display(QWidget *parent): QMainWindow(parent),m_server(this)
{
    //QDesktopWidget *desktop=QApplication::desktop();
    //this->setFixedHeight(desktop->availableGeometry().height());
    //this->setFixedWidth(desktop->availableGeometry().width());
    // Objects are initialized in order of appearing.
    m_centralWidget=new QWidget();
    m_centralWidget->setObjectName("centralWidget");
    this->setCentralWidget(m_centralWidget);
    QVBoxLayout *layoutPrincipal=new QVBoxLayout();
    m_centralWidget->setLayout(layoutPrincipal);

    m_welcome=new QLabel("<H1>MultiCHoice Server</H1>");
    m_welcome->setAlignment(Qt::AlignHCenter);
    m_welcome->setObjectName("welcome");

    m_serverLabel=new QLabel();
    m_serverLabel->setObjectName("serverLabel");

    m_chooseLabel=new QLabel("Choose the file witch contains the exam");
    m_chooseLabel->setObjectName("chooseLabel");

    m_path=new QLineEdit();
    m_path->setReadOnly(true);
    m_path->setObjectName("path");

    m_chargeFile=new QPushButton("Load file");
    m_chargeFile->setObjectName("chargeFile");
    connect(m_chargeFile,SIGNAL(clicked(bool)),this,SLOT(selectFile()));

    m_continue1=new QPushButton("Continue");
    m_continue1->setEnabled(false);
    m_continue1->setObjectName("continue");
    connect(m_continue1,SIGNAL(clicked(bool)),this,SLOT(displayExamInfo()));

    QHBoxLayout *fileChoiceLayout=new QHBoxLayout();
    fileChoiceLayout->addWidget(m_chooseLabel);
    fileChoiceLayout->addWidget(m_path);
    fileChoiceLayout->addWidget(m_chargeFile);

    layoutPrincipal->addWidget(m_welcome);
    layoutPrincipal->addWidget(m_serverLabel);
    layoutPrincipal->addLayout(fileChoiceLayout);
    layoutPrincipal->addWidget(m_continue1);

    m_infoExamLabel=new QLabel();
    m_infoExamLabel->setObjectName("infoExamLabel");

    m_startLogin=new QPushButton("Start Login Step");
    m_startLogin->setVisible(false);
    m_startLogin->setEnabled(false);
    m_startLogin->setObjectName("startLogin");
    connect(m_startLogin,SIGNAL(clicked(bool)),this,SLOT(startLoginStep()));
    m_startExam=new QPushButton("Start The Exam");
    m_startExam->setObjectName("startExam");
    connect(m_startExam,SIGNAL(clicked(bool)),this,SLOT(startExam()));
    m_startExam->setVisible(false);
    //m_startExam->setEnabled(false);

    m_loginGroupBox=new QGroupBox();
    QVBoxLayout *loginLayout=new QVBoxLayout();
    m_loginGroupBox->setLayout(loginLayout);
    m_loginGroupBox->setObjectName("loginGroupBox");

    m_numberOfStudents=new QLabel();
    loginLayout->addWidget(m_numberOfStudents);
    m_numberOfStudents->setObjectName("numberOfStudents");

    m_stConnected=new QLabel();
    loginLayout->addWidget(m_stConnected);
    m_stConnected->setObjectName("stConnected");

    m_stCnxRefused=new QLabel();
    loginLayout->addWidget(m_stCnxRefused);
    m_stCnxRefused->setObjectName("stCnxRefused");

    m_stWaitingCnx=new QLabel();
    m_stWaitingCnx->setObjectName("stWaitingCnx");
    loginLayout->addWidget(m_stWaitingCnx);
    loginLayout->addStretch(4);

    m_stReceivedQuestion=new QLabel();
    m_stReceivedQuestion->setObjectName("stReceivedQuestion");
    loginLayout->addWidget(m_stReceivedQuestion);

    m_stWaitingQuestions=new QLabel();
    m_stWaitingQuestions->setObjectName("stWaitingQuestions");
    loginLayout->addWidget(m_stWaitingQuestions);

    m_loginGroupBox->setVisible(false);

    m_examLoopGroupBox=new QGroupBox();
    m_examLoopGroupBox->setObjectName("examLoopGroupBox");
    QVBoxLayout *examLoopLayout=new QVBoxLayout();
    m_examLoopGroupBox->setLayout(examLoopLayout);

    m_stDoingExam=new QLabel();
    m_stDoingExam->setObjectName("stDoingExam");
    examLoopLayout->addWidget(m_stDoingExam);

    m_stFinishedExam=new QLabel();
    m_stFinishedExam->setObjectName("stFinishedExam");
    examLoopLayout->addWidget(m_stFinishedExam);

    m_examLoopGroupBox->setVisible(false);

    layoutPrincipal->addWidget(m_infoExamLabel);
    layoutPrincipal->addWidget(m_loginGroupBox);
    layoutPrincipal->addWidget(m_examLoopGroupBox);
    layoutPrincipal->addWidget(m_startLogin);
    layoutPrincipal->addWidget(m_startExam);

    m_loginInfoTimer=new QTimer(this);
    connect(m_loginInfoTimer,SIGNAL(timeout()),this,SLOT(refreshLoginInfo()));

    m_examLoopTimer=new QTimer(this);
    connect(m_examLoopTimer,SIGNAL(timeout()),this,SLOT(refreshExamLoopInfo()));

    QMenu *exam=this->menuBar()->addMenu("Exam");
    exam->addAction("Stop The Exam");
    QMenu *stud=this->menuBar()->addMenu("Student");
    stud->addAction("Cancel the exam of a student");

    QMenu *finance=this->menuBar()->addMenu("Finance");
    QAction *chargeStudentList=new QAction("Charge The List Of Students");
    finance->addAction(chargeStudentList);
    connect(chargeStudentList,SIGNAL(triggered(bool)),this,SLOT(selectStudentsList()));

    QMenu *cnx=this->menuBar()->addMenu("Connection");
    cnx->addAction("Change the ip or the port of the server");

    if(m_server.startTcpServer())
    {
        QMap<QString,QString> cnxTab=m_server.getCnxInfos();
        QString cnxInfos("The Server Run Correctely:<BR />");
        cnxInfos+="    I.P Address : "+cnxTab["ip"]+"<BR />"+"    Port : "+cnxTab["port"];
        m_serverLabel->setText(cnxInfos);
    }
    else
    {
        m_serverLabel->setText(" Problem with the server. The error is : <BR />"+
                           m_server.tcpMsgError());
        m_chargeFile->setEnabled(false);
    }
}

Display::~Display()
{

}


void Display::displayExamInfo()
{
    if(!m_dataClass.openDataBase(m_path->text()))
    {
        m_chooseLabel->setText(m_dataClass.lastErrors());
        return;
    }

    else
    {
        m_serverLabel->setVisible(false);
        m_chooseLabel->setVisible(false);
        m_path->setVisible(false);
        m_chargeFile->setVisible(false);
        m_continue1->setVisible(false);

        m_infoExamLabel->setText(m_dataClass.infoExam());
        m_infoExamLabel->setVisible(true);
        m_startLogin->setVisible(true);
    }
}


void Display::endOfExam()
{
    m_infoExamLabel->setText("It is The End Of Exam");
    m_dataClass.saveMarks(m_server.m_marks);
}


void Display::selectFile()
{
    m_path->setText(QFileDialog::getOpenFileName(this,"Choose the DataBase file","","Sqlite file(*.db *.db3 *.sqlite *.sqlite3)"));
    if(m_path->text()!="")
    {
        m_continue1->setEnabled(true);
    }
}


void Display::selectStudentsList()
{
    QString path=QFileDialog::getOpenFileName(this,"Choose The File Witch Contains The Loist","",
                 "Sqlite file(*.db *.db3 *.sqlite *.sqlite3)");
    m_server.m_okStudentList.clear();
    m_server.m_okStudentList=m_dataClass.getStudentsList(path);
    if(!m_server.m_okStudentList.isEmpty())
    {
        m_startLogin->setEnabled(true);
    }

}


void Display::refreshLoginInfo()
{
    m_server.getLoginInfo();
    m_stConnected->setText("Students Connected Now: "+m_server.m_loginInfoTab["connected"]);
    m_stReceivedQuestion->setText("Students Who Received Questions: "+m_server.m_loginInfoTab["receiveQuestions"]);
    m_stCnxRefused->setText("Students Not Allowed To Pass Exam: "+m_server.m_loginInfoTab["refused"]);
}


void Display::refreshExamLoopInfo()
{
    m_server.getExamLoopInfo();
    m_stDoingExam->setText("Students Doing Exam: "+m_server.m_examLoopInfoTab["stDoingExam"]);
    m_stFinishedExam->setText("Students Who Finished Exam: "+m_server.m_examLoopInfoTab["stFinishedExam"]);
    if(m_server.m_examLoopInfoTab["stDoingExam"]=="0")
    {
        m_examLoopTimer->stop();
        this->endOfExam();
    }
    else{}
}


void Display::startExam()
{
    m_loginInfoTimer->stop();
    m_examLoopGroupBox->setVisible(true);
    m_server.starExam();
    m_startExam->setVisible(false);
    m_examLoopTimer->start(5000);
}

void Display::startLoginStep()
{
    m_infoExamLabel->setText("Start of login step");
    QVector<Question> questions=m_dataClass.readQuestions();
    int numberQuestions=m_dataClass.numberOfQuestions();
    if(!questions.isEmpty())
    {
        m_startLogin->setVisible(false);
        m_infoExamLabel->setText(m_infoExamLabel->text()+"<BR /> questions loaded");
        m_server.startLoginStep(questions,numberQuestions);
        m_startExam->setVisible(true);
        m_loginGroupBox->setVisible(true);
        m_loginInfoTimer->start(5000);
    }
    else
    {
        m_infoExamLabel->setText(m_infoExamLabel->text()+"<BR />Loading of question failed."
                                 " The Error is: <BR />"+ m_dataClass.lastErrors());
    }
}
