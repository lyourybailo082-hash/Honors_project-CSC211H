#include "Student.h"

#include <QStringList>

Student::Student(const QString& name,
                 const QString& school,
                 int year,
                 double householdIncome,
                 bool receivesAid,
                 bool hasHousingConcern,
                 bool hasFoodConcern)
    : name(name),
      school(school),
      year(year),
      householdIncome(householdIncome),
      receivesAid(receivesAid),
      hasHousingConcern(hasHousingConcern),
      hasFoodConcern(hasFoodConcern) {}

const QString& Student::getName() const {
    return name;
}

const QString& Student::getSchool() const {
    return school;
}

int Student::getYear() const {
    return year;
}

double Student::getHouseholdIncome() const {
    return householdIncome;
}

bool Student::getReceivesAid() const {
    return receivesAid;
}

bool Student::getHasHousingConcern() const {
    return hasHousingConcern;
}

bool Student::getHasFoodConcern() const {
    return hasFoodConcern;
}

bool Student::isHighNeed() const {
    return householdIncome <= 45000.0 || hasHousingConcern || hasFoodConcern || !receivesAid;
}

QString Student::getPriorityLabel() const {
    if (householdIncome <= 30000.0 || (hasHousingConcern && hasFoodConcern)) {
        return "Urgent support recommended";
    }

    if (isHighNeed()) {
        return "High priority";
    }

    return "Stable";
}

QString Student::toCsv() const {
    QStringList parts;
    parts << name
          << school
          << QString::number(year)
          << QString::number(householdIncome, 'f', 2)
          << (receivesAid ? "1" : "0")
          << (hasHousingConcern ? "1" : "0")
          << (hasFoodConcern ? "1" : "0");
    return parts.join(",");
}

Student Student::fromCsv(const QString& line, bool* ok) {
    const QStringList parts = line.split(",");
    const bool valid = parts.size() == 7;

    if (ok) {
        *ok = valid;
    }

    if (!valid) {
        return {};
    }

    return Student(parts[0],
                   parts[1],
                   parts[2].toInt(),
                   parts[3].toDouble(),
                   parts[4] == "1",
                   parts[5] == "1",
                   parts[6] == "1");
}
