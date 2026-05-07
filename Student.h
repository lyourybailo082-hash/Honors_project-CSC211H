#ifndef STUDENT_H
#define STUDENT_H

#include <QString>

class Student {
public:
    Student() = default;
    Student(const QString& name,
            const QString& school,
            int year,
            double householdIncome,
            bool receivesAid,
            bool hasHousingConcern,
            bool hasFoodConcern);

    const QString& getName() const;
    const QString& getSchool() const;
    int getYear() const;
    double getHouseholdIncome() const;
    bool getReceivesAid() const;
    bool getHasHousingConcern() const;
    bool getHasFoodConcern() const;

    bool isHighNeed() const;
    QString getPriorityLabel() const;
    QString toCsv() const;
    static Student fromCsv(const QString& line, bool* ok = nullptr);

private:
    QString name;
    QString school;
    int year = 1;
    double householdIncome = 0.0;
    bool receivesAid = false;
    bool hasHousingConcern = false;
    bool hasFoodConcern = false;
};

#endif
