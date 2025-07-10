#include "mainwindow.h"
#include "./ui_mainwindow.h"

Services* book1 = new Services[1000];
Appointment* book2 = new Appointment[1000];
HashTable<QString,int> ht(1000);
int step_counter;
int found_id;
AVLTree<QString, int> appointment_tree;
AVLTree<QDate, int> date_tree;

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
bool MainWindow::addBook1(Services &value)
{
    if (ht.search(value.service_name,found_id, step_counter))return false;

    int current_row = ui->Book1->rowCount();
    ui->Book1->insertRow(current_row);

    ui->Book1->setItem(current_row,0,new QTableWidgetItem(value.service_name));
    ui->Book1->setItem(current_row,1,new QTableWidgetItem(value.price));
    ui->Book1->setItem(current_row,2,new QTableWidgetItem(value.duration));
    ui->Book1->setItem(current_row,3, new QTableWidgetItem(value.id));
    ht.insert(value.id, value.service_name);
    return true;
}
bool MainWindow::addBook2(Appointment &value){
    if (!ht.search(value.service_name,found_id, step_counter))return false;
    int current_row = ui->Book2->rowCount();
    ui->Book2->insertRow(current_row);

    ui->Book2->setItem(current_row,0,new QTableWidgetItem(value.service_name));
    ui->Book2->setItem(current_row,1,new QTableWidgetItem(value.customer));
    ui->Book2->setItem(current_row,2,new QTableWidgetItem(value.executer));
    ui->Book2->setItem(current_row,3,new QTableWidgetItem(value.date.toString("dd.MMM.yyyy")));
    ui->Book2->setItem(current_row,4, new QTableWidgetItem(value.id));
    appointment_tree.insertValue(value.service_name,value.id);
    date_tree.insertValue(value.date,value.id);
    return true;
}
bool MainWindow::deleteFromBook1(Services &value){
    int idInBook = ht.search(value.service_name,found_id, step_counter);
    if (!idInBook)return false;
    int rowInBook = searchInBook(*ui->Book1, 3, idInBook);

    ht.remove(value.service_name);
    ui->Book1->removeRow(rowInBook);
    return true;

}
bool MainWindow::deleteFromBook2(Appointment &value){
    int idInBook = ht.search(value.service_name,found_id, step_counter);
    if (!idInBook)return false;

    DLL<int> idInNode;
    if (!appointment_tree.find(value.service_name,idInNode)) return false;

    int rowInBook = searchInBook(*ui->Book2, 3, idInBook);
    while (!idInNode.isEmpty()){
        date_tree.checkAndDeleteValueInTree(idInNode.getHead()->data);
    }

    appointment_tree.removeNode(value.service_name);
    ui->Book1->removeRow(rowInBook);
    return true;
}

int MainWindow::searchInBook(QTableWidget&book,int column, int desired_id) {

    for (int i = 0; i < book.rowCount(); i++) {
        QTableWidgetItem* item = book.item(i, column);

        if (item) {
            bool ok;
            int current_id = item->text().toInt(&ok);
            if (ok && current_id == desired_id) {
                return i;
            }
        }
    }

    return -1;
}


MainWindow::~MainWindow()
{
    delete ui;
}
