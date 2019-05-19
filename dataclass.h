#ifndef DATACLASS_H
#define DATACLASS_H

#include "question.h"
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QVector>
#include <QDebug>
#include <QSqlError>
#include <QMap>
class DataClass
{
    public:
        DataClass();
        bool openDataBase(QString);
        QString infoExam();
        QVector<Question> readQuestions();
        int numberOfQuestions();
        QString lastErrors();
        QVector<int> getStudentsList(QString);
        void saveMarks(QMap<quint32,quint16>);

    private:
        QSqlDatabase m_db;
        QSqlQuery m_query;
        QVector<Question> m_questionnaire;
        QString m_error;
};

#endif // DATACLASS_H
