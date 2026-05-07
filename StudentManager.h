#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include "Student.h"

#include <QVector>

class StudentManager {
public:
    void addStudent(const Student& student);
    void removeStudent(int index);
    void sortByIncome();

    const QVector<Student>& getStudents() const;
    QVector<Student> getHighNeedStudents() const;

    bool saveToFile(const QString& filePath) const;
    bool loadFromFile(const QString& filePath);

private:
    QVector<Student> students;
};

#endif
