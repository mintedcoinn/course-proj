#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "appointment.h"
#include "services.h"
#include "HT.h"
#include "DLL.h"
#include "AVLTree.h"


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

private:
    Ui::MainWindow *ui;
  //  input_book1_window *input_ui;
};
#endif // MAINWINDOW_H
