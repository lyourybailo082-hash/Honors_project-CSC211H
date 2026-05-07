#include "MainWindow.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    buildUi();
    populateResources();
    setStatusMessage("Ready to support students.");
}

void MainWindow::buildUi() {
    setWindowTitle("Student Support Navigator");
    resize(1100, 720);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout* pageLayout = new QHBoxLayout(central);
    QVBoxLayout* leftColumn = new QVBoxLayout();
    QVBoxLayout* rightColumn = new QVBoxLayout();

    pageLayout->addLayout(leftColumn, 2);
    pageLayout->addLayout(rightColumn, 3);

    QGroupBox* intakeBox = new QGroupBox("Student Profile");
    QVBoxLayout* intakeLayout = new QVBoxLayout(intakeBox);

    nameInput = new QLineEdit();
    schoolInput = new QLineEdit();
    yearInput = new QSpinBox();
    incomeInput = new QDoubleSpinBox();
    aidCheck = new QCheckBox("Currently receives financial aid");
    housingCheck = new QCheckBox("Has housing insecurity concerns");
    foodCheck = new QCheckBox("Has food insecurity concerns");

    yearInput->setRange(1, 8);
    incomeInput->setRange(0.0, 250000.0);
    incomeInput->setPrefix("$");
    incomeInput->setDecimals(2);
    incomeInput->setSingleStep(1000.0);

    intakeLayout->addWidget(new QLabel("Student name"));
    intakeLayout->addWidget(nameInput);
    intakeLayout->addWidget(new QLabel("School or campus"));
    intakeLayout->addWidget(schoolInput);
    intakeLayout->addWidget(new QLabel("Year in school"));
    intakeLayout->addWidget(yearInput);
    intakeLayout->addWidget(new QLabel("Household income"));
    intakeLayout->addWidget(incomeInput);
    intakeLayout->addWidget(aidCheck);
    intakeLayout->addWidget(housingCheck);
    intakeLayout->addWidget(foodCheck);

    QHBoxLayout* actionRow = new QHBoxLayout();
    QPushButton* addButton = new QPushButton("Add Student");
    QPushButton* removeButton = new QPushButton("Remove Selected");
    QPushButton* sortButton = new QPushButton("Sort by Income");
    QPushButton* filterButton = new QPushButton("Show High Need");

    actionRow->addWidget(addButton);
    actionRow->addWidget(removeButton);
    actionRow->addWidget(sortButton);
    actionRow->addWidget(filterButton);
    intakeLayout->addLayout(actionRow);

    studentList = new QListWidget();
    intakeLayout->addWidget(new QLabel("Student records"));
    intakeLayout->addWidget(studentList);

    leftColumn->addWidget(intakeBox);

    QGroupBox* resourceBox = new QGroupBox("Support Resources");
    QVBoxLayout* resourceLayout = new QVBoxLayout(resourceBox);
    resourceList = new QListWidget();
    resourceLayout->addWidget(new QLabel("Students can use this list to find support quickly."));
    resourceLayout->addWidget(resourceList);
    leftColumn->addWidget(resourceBox);

    QGroupBox* planBox = new QGroupBox("Guided Support Plan");
    QVBoxLayout* planLayout = new QVBoxLayout(planBox);

    studentSelector = new QComboBox();
    QPushButton* planButton = new QPushButton("Build Support Plan");
    QPushButton* saveButton = new QPushButton("Save Profiles");
    QPushButton* loadButton = new QPushButton("Load Profiles");
    planOutput = new QTextEdit();
    statusLabel = new QLabel();

    planOutput->setReadOnly(true);
    planOutput->setPlaceholderText("Select a student to generate a personalized support plan.");

    planLayout->addWidget(new QLabel("Choose a student"));
    planLayout->addWidget(studentSelector);
    planLayout->addWidget(planButton);
    planLayout->addWidget(planOutput);

    QHBoxLayout* storageRow = new QHBoxLayout();
    storageRow->addWidget(saveButton);
    storageRow->addWidget(loadButton);
    planLayout->addLayout(storageRow);
    planLayout->addWidget(statusLabel);

    rightColumn->addWidget(planBox);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeSelectedStudent);
    connect(sortButton, &QPushButton::clicked, this, &MainWindow::sortStudentsByIncome);
    connect(filterButton, &QPushButton::clicked, this, &MainWindow::showHighNeedStudents);
    connect(planButton, &QPushButton::clicked, this, &MainWindow::showSelectedStudentPlan);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveStudents);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadStudents);
}

void MainWindow::populateResources() {
    resourceList->clear();
    resourceList->addItem("Emergency grants: short-term support for housing, meals, transport, or medical costs");
    resourceList->addItem("Scholarship hub: campus scholarships, local foundations, and first-generation awards");
    resourceList->addItem("Food pantry and meal vouchers: immediate grocery and dining support");
    resourceList->addItem("Low-cost textbook exchange: book lending, used copies, and open educational resources");
    resourceList->addItem("Work-study and part-time jobs: flexible income with class-friendly scheduling");
    resourceList->addItem("Academic support: tutoring, writing center, and study groups");
    resourceList->addItem("Financial aid advising: FAFSA help, appeals, and verification support");
    resourceList->addItem("Mental health and wellness: counseling for stress caused by financial pressure");
}

void MainWindow::addStudent() {
    const QString name = nameInput->text().trimmed();
    const QString school = schoolInput->text().trimmed();

    if (name.isEmpty() || school.isEmpty()) {
        setStatusMessage("Name and school are required.");
        return;
    }

    manager.addStudent(Student(name,
                               school,
                               yearInput->value(),
                               incomeInput->value(),
                               aidCheck->isChecked(),
                               housingCheck->isChecked(),
                               foodCheck->isChecked()));

    refreshStudentList();
    refreshStudentSelector();

    nameInput->clear();
    schoolInput->clear();
    yearInput->setValue(1);
    incomeInput->setValue(0.0);
    aidCheck->setChecked(false);
    housingCheck->setChecked(false);
    foodCheck->setChecked(false);

    setStatusMessage("Student profile added.");
}

void MainWindow::removeSelectedStudent() {
    const int row = studentList->currentRow();
    if (row < 0) {
        setStatusMessage("Select a student to remove.");
        return;
    }

    manager.removeStudent(row);
    refreshStudentList();
    refreshStudentSelector();
    planOutput->clear();
    setStatusMessage("Student removed.");
}

void MainWindow::sortStudentsByIncome() {
    manager.sortByIncome();
    refreshStudentList();
    refreshStudentSelector();
    setStatusMessage("Students sorted by household income.");
}

void MainWindow::showHighNeedStudents() {
    studentList->clear();
    const QVector<Student> highNeedStudents = manager.getHighNeedStudents();

    for (const Student& student : highNeedStudents) {
        studentList->addItem(QString("%1 | %2 | %3")
                                 .arg(student.getName(),
                                      student.getSchool(),
                                      student.getPriorityLabel()));
    }

    setStatusMessage(QString("Showing %1 high-need student(s).").arg(highNeedStudents.size()));
}

void MainWindow::showSelectedStudentPlan() {
    const int index = studentSelector->currentIndex();
    if (index < 0 || index >= manager.getStudents().size()) {
        setStatusMessage("Select a student to build a plan.");
        return;
    }

    const Student& student = manager.getStudents().at(index);
    planOutput->setPlainText(buildSupportPlan(student));
    setStatusMessage("Support plan generated.");
}

void MainWindow::saveStudents() {
    if (manager.saveToFile("students.txt")) {
        setStatusMessage("Profiles saved to students.txt.");
        return;
    }

    setStatusMessage("Could not save students.txt.");
}

void MainWindow::loadStudents() {
    if (manager.loadFromFile("students.txt")) {
        refreshStudentList();
        refreshStudentSelector();
        planOutput->clear();
        setStatusMessage("Profiles loaded from students.txt.");
        return;
    }

    setStatusMessage("Could not load students.txt.");
}

void MainWindow::refreshStudentList() {
    studentList->clear();
    for (const Student& student : manager.getStudents()) {
        studentList->addItem(QString("%1 | %2 | Income: $%3 | %4")
                                 .arg(student.getName(),
                                      student.getSchool(),
                                      QString::number(student.getHouseholdIncome(), 'f', 0),
                                      student.getPriorityLabel()));
    }
}

void MainWindow::refreshStudentSelector() {
    studentSelector->clear();
    for (const Student& student : manager.getStudents()) {
        studentSelector->addItem(QString("%1 (%2)").arg(student.getName(), student.getSchool()));
    }
}

void MainWindow::setStatusMessage(const QString& message) {
    statusLabel->setText(message);
}

QString MainWindow::buildSupportPlan(const Student& student) const {
    QStringList plan;

    plan << QString("Student: %1").arg(student.getName());
    plan << QString("Campus: %1").arg(student.getSchool());
    plan << QString("Priority: %1").arg(student.getPriorityLabel());
    plan << "";
    plan << "Recommended next steps:";

    if (student.getHouseholdIncome() <= 45000.0) {
        plan << "1. Meet with financial aid advising to review grants, scholarship eligibility, and appeal options.";
        plan << "2. Apply for emergency grants and need-based scholarships first.";
    } else {
        plan << "1. Review scholarship databases and campus employment options for supplemental funding.";
    }

    if (!student.getReceivesAid()) {
        plan << "3. Complete or update financial aid paperwork as soon as possible.";
    }

    if (student.getHasHousingConcern()) {
        plan << "4. Contact housing support services and ask about emergency housing funds or temporary placement.";
    }

    if (student.getHasFoodConcern()) {
        plan << "5. Use the campus food pantry, meal voucher program, or community grocery support this week.";
    }

    plan << "6. Use tutoring and academic support to protect study time while financial issues are being addressed.";
    plan << "";
    plan << "Core resources to explore:";
    plan << "- Emergency grants";
    plan << "- Scholarships";
    plan << "- Food pantry / meal assistance";
    plan << "- Textbook exchange";
    plan << "- Work-study or part-time campus jobs";
    plan << "- Counseling and wellness support";

    return plan.join("\n");
}
