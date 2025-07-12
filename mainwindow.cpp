#include "mainwindow.h"
#include "./ui_mainwindow.h"

Services* book1 = new Services[1000];
Appointment* book2 = new Appointment[1000];
HashTable<QString,int> ht(1000);
int step_counter;
int found_id;
AVLTree<QString, int> appointment_tree;
AVLTree<QDate, int> date_tree;
bool CLEAR_ALL_MENTIONS_OF_SERVICE = false;
int book1id = 0;
int book2id = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->Book1->setColumnCount(4);
    ui->Book1->setHorizontalHeaderLabels({"Услуга", "Цена", "Длительность", "ID"});
    ui->Book2->setColumnCount(5);
    ui->Book2->setHorizontalHeaderLabels({"Услуга", "Клиент", "Исполнитель", "Дата", "ID"});

    addBook1Action = ui->addBook1Action;
    connect(addBook1Action,&QAction::triggered,this,&MainWindow::openInputDialogBook1);
    addBook2Action = ui->addBook2Action;
    connect(addBook2Action,&QAction::triggered,this,&MainWindow::openInputDialogBook2);
    deleteBook1Action = ui->deleteBook1Action;
    connect(deleteBook1Action,&QAction::triggered,this,&MainWindow::openInpDelDialogBook1);
    deleteBook2Action = ui->deleteBook2Action;
    connect(deleteBook2Action,&QAction::triggered,this,&MainWindow::openInpDelDialogBook2);

}
bool MainWindow::addBook1(Services &value)
{
    if (ht.search(value.service_name,found_id, step_counter))return false;

    int current_row = ui->Book1->rowCount();
    ui->Book1->insertRow(current_row);

    ui->Book1->setItem(current_row,0,new QTableWidgetItem(value.service_name));
    ui->Book1->setItem(current_row,1,new QTableWidgetItem(QString::number(value.price)));
    ui->Book1->setItem(current_row,2,new QTableWidgetItem(QString::number(value.duration)));
    ui->Book1->setItem(current_row,3, new QTableWidgetItem(QString::number(value.id)));
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
    ui->Book2->setItem(current_row,4, new QTableWidgetItem(QString::number(value.id)));
    appointment_tree.insertValue(value.service_name,value.id);
    date_tree.insertValue(value.date,value.id);
    return true;
}

bool MainWindow::deleteFromBook1(Services &value){
    int idInBook =0;
    ht.search(value.service_name,idInBook, step_counter);
    if (idInBook<0)return false;
    value.id = idInBook;
    int rowInBook = searchInBook(*ui->Book1, 3, idInBook);

    Appointment temp;
    temp.service_name = value.service_name;
    CLEAR_ALL_MENTIONS_OF_SERVICE = true;
    deleteFromBook2(temp);
    CLEAR_ALL_MENTIONS_OF_SERVICE = false;
    ht.remove(value.service_name, value.id);
    ui->Book1->removeRow(rowInBook);
    return true;

}
bool MainWindow::deleteFromBook2(Appointment &value){
    int idInBook =0;
    ht.search(value.service_name,idInBook, step_counter);
    if (idInBook<0)return false;

    if (CLEAR_ALL_MENTIONS_OF_SERVICE)
    {
        DLL<int> idInNode;
        if (!appointment_tree.find(value.service_name,idInNode)) return false;

        while (!idInNode.isEmpty()){
            int rowInBook = searchInBook(*ui->Book2, 4, idInNode.getHead()->data);
            ui->Book2->removeRow(rowInBook);
            date_tree.checkAndDeleteValueInTree(idInNode.getHead()->data);
            idInNode.removeValue(idInNode.getHead()->data);
        }

        appointment_tree.removeNode(value.service_name);

    }

    else
    {
        int rowInBook = searchInBook(*ui->Book2, 4, value.id);
        QTableWidgetItem* item = ui->Book2->item(rowInBook, 4);
        QDate tempDate = QDate::fromString(item->text(), "dd-MMM-yyyy");

        appointment_tree.removeValue(value.service_name, value.id);
        date_tree.removeValue(tempDate, value.id);
        ui->Book2->removeRow(rowInBook);
    }
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

void MainWindow::openInputDialogBook1(){
    inputDialogBook1 dialog(this);
    if (dialog.exec() == QDialog::Accepted){
        Services new_service;
        new_service.service_name = dialog.getServiceNameInput();
        new_service.price = dialog.getPriceInput().toInt();
        new_service.duration = dialog.getDurationInput().toInt();
        new_service.id = book1id++;
        addBook1(new_service);
    }
}
void MainWindow::openInputDialogBook2(){
    InputDialogBook2 dialog(this);
    if (dialog.exec() == QDialog::Accepted){
        Appointment new_appointment;
        new_appointment.service_name = dialog.getServiceNameInput();
        new_appointment.customer = dialog.getCustomerInput();
        new_appointment.executer = dialog.getExecuterInput();
        new_appointment.date = dialog.getDateInput();
        new_appointment.id = book2id++;
        addBook2(new_appointment);
    }
}
void MainWindow::openInpDelDialogBook1(){
    inputDialogBook1 dialog(this);
    dialog.usingAsDelInput();
    if (dialog.exec() == QDialog::Accepted){
        Services dele_service;
        dele_service.service_name = dialog.getServiceNameInput();
        deleteFromBook1(dele_service);
    }
}

void MainWindow::openInpDelDialogBook2(){
    InputDialogBook2 dialog(this);
    dialog.usingAsDelInput();
    if (dialog.exec() == QDialog::Accepted){
        Appointment dele_appointment;
        dele_appointment.service_name = dialog.getServiceNameInput();
        dele_appointment.id = dialog.getCustomerInput().toInt();
        deleteFromBook2(dele_appointment);
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
