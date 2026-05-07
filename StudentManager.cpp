#include "StudentManager.h"

#include <QFile>
#include <QTextStream>
#include <algorithm>

void StudentManager::addStudent(const Student& student) {
    students.append(student);
}

void StudentManager::removeStudent(int index) {
    if (index >= 0 && index < students.size()) {
        students.removeAt(index);
    }
}

void StudentManager::sortByIncome() {
    std::sort(students.begin(), students.end(), [](const Student& left, const Student& right) {
        return left.getHouseholdIncome() < right.getHouseholdIncome();
    });
}

const QVector<Student>& StudentManager::getStudents() const {
    return students;
}

QVector<Student> StudentManager::getHighNeedStudents() const {
    QVector<Student> filtered;

    for (const Student& student : students) {
        if (student.isHighNeed()) {
            filtered.append(student);
        }
    }

    return filtered;
}

bool StudentManager::saveToFile(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    for (const Student& student : students) {
        out << student.toCsv() << "\n";
    }

    return true;
}

bool StudentManager::loadFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QVector<Student> loadedStudents;
    QTextStream in(&file);

    while (!in.atEnd()) {
        bool ok = false;
        const QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue;
        }

        const Student student = Student::fromCsv(line, &ok);
        if (ok) {
            loadedStudents.append(student);
        }
    }

    students = loadedStudents;
    return true;
}
