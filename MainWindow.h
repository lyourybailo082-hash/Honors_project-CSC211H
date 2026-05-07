#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "StudentManager.h"

#include <QMainWindow>

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QListWidget;
class QLineEdit;
class QSpinBox;
class QTextEdit;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void addStudent();
    void removeSelectedStudent();
    void sortStudentsByIncome();
    void showHighNeedStudents();
    void showSelectedStudentPlan();
    void saveStudents();
    void loadStudents();

private:
    void buildUi();
    void populateResources();
    void refreshStudentList();
    void refreshStudentSelector();
    void setStatusMessage(const QString& message);
    QString buildSupportPlan(const Student& student) const;

    StudentManager manager;

    QLineEdit* nameInput = nullptr;
    QLineEdit* schoolInput = nullptr;
    QSpinBox* yearInput = nullptr;
    QDoubleSpinBox* incomeInput = nullptr;
    QCheckBox* aidCheck = nullptr;
    QCheckBox* housingCheck = nullptr;
    QCheckBox* foodCheck = nullptr;

    QListWidget* studentList = nullptr;
    QListWidget* resourceList = nullptr;
    QComboBox* studentSelector = nullptr;
    QTextEdit* planOutput = nullptr;
    QLabel* statusLabel = nullptr;
};

#endif
