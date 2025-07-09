#include "mainwindow.h"
#include "./ui_mainwindow.h"

int current_book1_row_count = -1;
int current_book2_row_count = 0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->Book1->setColumnCount(4);
    ui->Book1->setHorizontalHeaderLabels({"Услуга", "Цена", "Длительность", "ID"});
    ui->Book2->setColumnCount(5);
    ui->Book2->setHorizontalHeaderLabels({"Услуга", "Клиент", "Исполнитель", "Дата", "ID"});
}
void MainWindow::addBook1(Services *usluga)
{
    current_book1_row_count++;
    ui->Book1->setRowCount(current_book1_row_count+1);

    ui->Book1->setItem(current_book1_row_count,0,usluga->service_name);
    ui->Book1->setItem(current_book1_row_count,1,usluga->price);
    ui->Book1->setItem(current_book1_row_count,2,usluga->duration);
    ui->Book1->setItem(current_book1_row_count,3,usluga->id);
}
void MainWindow::addBook2(Appointment zapis){

}
void MainWindow::deleteFromBook1(Services usluga){

}
void MainWindow::deleteFromBooks(Appointment zapis){

}

MainWindow::~MainWindow()
{
    delete ui;
}
