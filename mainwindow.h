#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QAction>
#include <QDate>
#include <QFileDialog>
#include <QTextCodec>
#include <utility>
#include "appointment.h"
#include "services.h"
#include "HT.h"
#include "DLL.h"
#include "AVLTree.h"
#include "inputdialogbook1.h"
#include "inputdialogbook2.h"
#include "ht_input.h"
#include <QDebug>
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
    bool searchAndPrintBook1(Services &value);
    bool searchAndPrintBook2(Appointment &value);
    void createAndShowReport();
    //void setHTsize();

private slots:
    void openInputDialogBook1();
    void openInputDialogBook2();
    void openInpDelDialogBook1();
    void openInpDelDialogBook2();
    void openInpFinDialogBook1();
    void openInpFinDialogBook2();
    void openFileBook1();
    void openFileBook2();
    void saveBook1ToFile();
    void saveBook2ToFile();
    void saveResult();
    void on_reportmakeButton_clicked();

private:
    Ui::MainWindow *ui;
    QAction *addBook1Action;
    QAction *addBook2Action;
    QAction *deleteBook1Action;
    QAction *deleteBook2Action;
    QAction *findBook1Action;
    QAction *findBook2Action;
    QAction *saveBook1Action;
    QAction *saveBook2Action;
    QAction *saveResultAction;
    QAction *openBook1Action;
    QAction *openBook2Action;

};
#endif // MAINWINDOW_H
