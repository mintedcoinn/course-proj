#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QAction>
#include <QDate>
#include "appointment.h"
#include "services.h"
#include "HT.h"
#include "DLL.h"
#include "AVLTree.h"
#include "inputdialogbook1.h"
#include "inputdialogbook2.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool addBook1(Services &value);
    bool addBook2(Appointment &value);
    bool deleteFromBook1(Services &value);
    bool deleteFromBook2(Appointment &value);
    int searchInBook(QTableWidget&book,int column, int desired_id);

private slots:
    void openInputDialogBook1();
    void openInputDialogBook2();
    void openInpDelDialogBook1();
    void openInpDelDialogBook2();
private:
    Ui::MainWindow *ui;
    QAction *addBook1Action;
    QAction *addBook2Action;
    QAction *deleteBook1Action;
    QAction *deleteBook2Action;
};
#endif // MAINWINDOW_H
