#include "dataclass.h"

DataClass::DataClass()
{

}


QVector<int> DataClass::getStudentsList(QString path)
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","finance");
    db.setDatabaseName(path);
    db.open();

    QSqlQuery query(db);
    query.exec("SELECT rollNumbers FROM studentList");
    QVector<int> students;
    while(query.next())
    {

        students.push_back(query.value(0).toInt());
    }
    return students;
}

QString DataClass::infoExam()
{
    m_query=QSqlQuery(m_db);
    m_query.exec("SELECT dataNames,dataValues FROM infoAboutExam");
    QString infos;
    while(m_query.next())
    {
        infos+=m_query.value(0).toString()+"   :   ";
        infos+="<STRONG>"+m_query.value(1).toString()+"</STRONG>";
        infos+="<BR />";
    }
    return infos;
}

bool DataClass::openDataBase(QString name)
{
    m_db=QSqlDatabase::addDatabase("QSQLITE","mainCnx");
    m_db.setDatabaseName(name);
    m_error=m_db.lastError().text();
    return m_db.open();
}


int DataClass::numberOfQuestions()
{
    m_query.exec("SELECT dataValues FROM infoAboutExam WHERE dataNames=\"Number Of Questions\" ");
    if(m_query.next())
    {
        int i;
        i=m_query.value(0).toInt();
        return i;
    }
    else
    {
        return 5;
    }
    m_error=m_query.lastError().text();
}


QString DataClass::lastErrors()
{
    return m_error;
}

QVector<Question> DataClass::readQuestions()
{
    QVector<Question> questions;
    int i=0;
    m_query.exec("SELECT question,answer,choiceA,choiceB,choiceC,choiceD,temps FROM questionsTab");
    while(m_query.next())
    {
        questions.push_back(Question());
        questions[i].question=m_query.value(0).toString();
        questions[i].answer=m_query.value(1).toString();
        questions[i].choice1=m_query.value(2).toString();
        questions[i].choice2=m_query.value(3).toString();
        questions[i].choice3=m_query.value(4).toString();
        questions[i].choice4=m_query.value(5).toString();
        questions[i].timeQ=m_query.value(6).toInt();
        ++i;
    }
    m_error=m_query.lastError().databaseText()+"<BR />" +m_query.lastError().driverText();
    return questions;
}


void DataClass::saveMarks(QMap<quint32, quint16> marks)
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","saving");
    db.setDatabaseName("C:/marks.db3");
    db.open();
    QSqlQuery query(db);
    query.exec("CREATE TABLE studentsMarks(rollNumber integer,marks integer)");
    QMap<quint32,quint16>::ConstIterator it=marks.constBegin();

    while(it!=marks.constEnd())
    {
        query.prepare("INSERT INTO studentsMarks (rollNumber,marks) VALUES(?,?)");
        query.addBindValue(it.key());
        query.addBindValue(it.value());
        query.exec();
        ++it;
    }
}
