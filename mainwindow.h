#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void addBook1(Services *usluga);
    void addBook2(Appointment zapis);
    void deleteFromBook1(Services usluga);
    void deleteFromBooks(Appointment zapis);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
