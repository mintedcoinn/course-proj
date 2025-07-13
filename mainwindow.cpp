#include "mainwindow.h"
#include "./ui_mainwindow.h"

Services* book1 = new Services[1000];
Appointment* book2 = new Appointment[1000];
HashTable<QString,int> ht(100);

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

    //setHTsize();
    ui->Book1->setColumnCount(4);
    ui->Book1->setHorizontalHeaderLabels({"Услуга", "Цена", "Длительность", "ID"});
    ui->Book2->setColumnCount(5);
    ui->Book2->setHorizontalHeaderLabels({"Услуга", "Клиент", "Мастер", "Дата", "ID"});
    ui->reportTable->setColumnCount(6);
    ui->reportTable->setHorizontalHeaderLabels({"Услуга", "Клиент", "Цена", "Дата", "Мастер", "Длительность"});

    addBook1Action = ui->addBook1Action;
    connect(addBook1Action,&QAction::triggered,this,&MainWindow::openInputDialogBook1);
    addBook2Action = ui->addBook2Action;
    connect(addBook2Action,&QAction::triggered,this,&MainWindow::openInputDialogBook2);
    deleteBook1Action = ui->deleteBook1Action;
    connect(deleteBook1Action,&QAction::triggered,this,&MainWindow::openInpDelDialogBook1);
    deleteBook2Action = ui->deleteBook2Action;
    connect(deleteBook2Action,&QAction::triggered,this,&MainWindow::openInpDelDialogBook2);
    findBook1Action = ui->findBook1Action;
    connect(findBook1Action,&QAction::triggered,this,&MainWindow::openInpFinDialogBook1);
    findBook2Action = ui->findBook2Action;
    connect(findBook2Action,&QAction::triggered,this,&MainWindow::openInpFinDialogBook2);
    saveResultAction = ui->saveResultAction;
    connect(saveResultAction,&QAction::triggered,this,&MainWindow::saveResult);
    saveBook1Action =  ui->saveBook1Action;
    connect(saveBook1Action,&QAction::triggered,this,&MainWindow::saveBook1ToFile);
    saveBook2Action = ui->saveBook2Action;
    connect(saveBook2Action,&QAction::triggered,this,&MainWindow::saveBook2ToFile);
    openBook1Action = ui->openBook1Action;
    connect(openBook1Action,&QAction::triggered,this,&MainWindow::openFileBook1);
    openBook2Action = ui->openBook2Action;
    connect(openBook2Action,&QAction::triggered,this,&MainWindow::openFileBook2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::addBook1(Services &value)
{
    if (ht.search(value.service_name))return false;

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
    if (!ht.search(value.service_name))return false;
    int current_row = ui->Book2->rowCount();
    ui->Book2->insertRow(current_row);

    ui->Book2->setItem(current_row,0,new QTableWidgetItem(value.service_name));
    ui->Book2->setItem(current_row,1,new QTableWidgetItem(value.customer));
    ui->Book2->setItem(current_row,2,new QTableWidgetItem(value.executer));
    ui->Book2->setItem(current_row,3,new QTableWidgetItem(QLocale("en_US").toString(value.date, "dd.MMM.yyyy")));
    ui->Book2->setItem(current_row,4, new QTableWidgetItem(QString::number(value.id)));
    appointment_tree.insertValue(value.service_name,value.id);
    date_tree.insertValue(value.date,value.id);
    return true;
}

bool MainWindow::deleteFromBook1(Services &value){
    int idInBook =0;
    ht.search(value.service_name,&idInBook);
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
    ht.search(value.service_name,&idInBook);
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

bool MainWindow::searchAndPrintBook1(Services &value){
    int idInBook = 0;
    int step_counter;
    ht.search(value.service_name,&idInBook, &step_counter);
    if (idInBook<0) return false;

    ui->stepCounter_2->setPlainText(QString::number(step_counter));
    ui->FindedBook1->clear();
    ui->FindedBook1->setRowCount(0);

    ui->FindedBook1->setColumnCount(4);
    ui->FindedBook1->setHorizontalHeaderLabels({"Услуга", "Цена", "Длительность", "ID"});
    int rowInBook = searchInBook(*ui->Book1, 3, idInBook);

    int current_row = ui->FindedBook1->rowCount();
    ui->FindedBook1->insertRow(current_row);

    ui->FindedBook1->setItem(current_row,0, ui->Book1->item(rowInBook, 0)->clone());
    ui->FindedBook1->setItem(current_row,1 ,ui->Book1->item(rowInBook, 1)->clone());
    ui->FindedBook1->setItem(current_row,2, ui->Book1->item(rowInBook, 2)->clone());
    ui->FindedBook1->setItem(current_row,3, ui->Book1->item(rowInBook, 3)->clone());
    return true;
}

bool MainWindow::searchAndPrintBook2(Appointment &value){
    int idInBook = 0;
    ht.search(value.service_name,&idInBook);
    if (idInBook<0) return false;

    DLL<int> idInNode;
    int step_counter;
    if (!appointment_tree.find(value.service_name,idInNode,&step_counter)) return false;

    ui->stepCounter_2->setPlainText(QString::number(step_counter));

    ui->FindedBook1->clear();
    ui->FindedBook1->setRowCount(0);

    ui->FindedBook1->setColumnCount(5);
    ui->FindedBook1->setHorizontalHeaderLabels({"Услуга", "Клиент", "Мастер", "Дата", "ID"});

    while (!idInNode.isEmpty())
    {
        int rowInBook = searchInBook(*ui->Book2, 4, idInNode.getHead()->data);
        int current_row = ui->FindedBook1->rowCount();
        ui->FindedBook1->insertRow(current_row);
        ui->FindedBook1->setItem(current_row,0, ui->Book2->item(rowInBook, 0)->clone());
        ui->FindedBook1->setItem(current_row,1 ,ui->Book2->item(rowInBook, 1)->clone());
        ui->FindedBook1->setItem(current_row,2, ui->Book2->item(rowInBook, 2)->clone());
        ui->FindedBook1->setItem(current_row,3, ui->Book2->item(rowInBook, 3)->clone());
        ui->FindedBook1->setItem(current_row,4, ui->Book2->item(rowInBook, 4)->clone());

        idInNode.removeValue(idInNode.getHead()->data);
    }
    return true;
}

void MainWindow::createAndShowReport(){
    QString customer = ui->customerNameReport->toPlainText();
    QDate start_period = ui->startPeriod->date();
    QDate end_period = ui->endPeriod->date();
    bool ok;
    int price = ui->servisePriceReport->toPlainText().toInt(&ok);

    if (customer.trimmed().isEmpty() || ui->servisePriceReport->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены");
        return;
    }
    if (start_period> end_period){
        QMessageBox::warning(this, "Ошибка", "Начальная дата больше конченой");
        return;
    }

    if (!ok)
    {
        QMessageBox::warning(this, "Ошибка", "Цена является целочисленным показателем");
        return;
    }

    ui->reportTable->clearContents();
    ui->reportTable->setRowCount(0);

    DLL<int> services_id;
    DLL<pair<QString, int>> namesFromId;

    for (QDate date = start_period; date <= end_period; date = date.addDays(1)){
        DLL<int> listId;
        if (!date_tree.find(date,listId)) continue;

        DLL<int> temp;
        temp = listId;
        while (!temp.isEmpty())
        {
            int rowInBook2 = searchInBook(*ui->Book2,4,temp.getHead()->data);
            QTableWidgetItem *item = ui->Book2->item(rowInBook2,1);
            QString appo_cutomer = item->text();
            if (appo_cutomer == customer){
                services_id.append(temp.getHead()->data);
            }
            temp.removeValue(temp.getHead()->data);
        }
        temp.clear();
    }

    while (!services_id.isEmpty()){
        int rowInBook2 = searchInBook(*ui->Book2,4,services_id.getHead()->data);
        QTableWidgetItem *item = ui->Book2->item(rowInBook2,0);
        QString service_name = item->text();
        namesFromId.append(make_pair(service_name, services_id.getHead()->data));
        services_id.removeValue(services_id.getHead()->data);
    }

    services_id.clear();

    while (!namesFromId.isEmpty()){
        int idInBook1 =0;

        if (!ht.search(namesFromId.getHead()->data.first, &idInBook1)){
            namesFromId.removeValue(namesFromId.getHead()->data);
            continue;
        }
        int rowInBook1 = searchInBook(*ui->Book1, 3, idInBook1);
        QTableWidgetItem *item = ui->Book1->item(rowInBook1,1);
        int serv_price = item->text().toInt();

        if (serv_price == price){
            int rowInBook2 = searchInBook(*ui->Book2,4,namesFromId.getHead()->data.second);
            int current_row = ui->reportTable->rowCount();
            ui->reportTable->insertRow(current_row);
            //"Услуга", "Клиент", "Цена", "Дата", "Мастер", "Длительность"
            ui->reportTable->setItem(current_row, 0, ui->Book1->item(rowInBook1,0)->clone());
            ui->reportTable->setItem(current_row, 1, ui->Book2->item(rowInBook2,1)->clone());
            ui->reportTable->setItem(current_row, 2, ui->Book1->item(rowInBook1,1)->clone());
            ui->reportTable->setItem(current_row, 3, ui->Book2->item(rowInBook2,3)->clone());
            ui->reportTable->setItem(current_row, 4, ui->Book2->item(rowInBook2,2)->clone());
            ui->reportTable->setItem(current_row, 5, ui->Book1->item(rowInBook1,2)->clone());
        }
        namesFromId.removeValue(namesFromId.getHead()->data);
    }
    namesFromId.clear();
}
void MainWindow::openInputDialogBook1(){
    inputDialogBook1 dialog(this);
    if (dialog.exec() == QDialog::Accepted){
        Services new_service;
        new_service.service_name = dialog.getServiceNameInput();
        new_service.price = dialog.getPriceInput().toInt();
        new_service.duration = dialog.getDurationInput().toInt();
        new_service.id = book1id++;
        if (!addBook1(new_service))
            QMessageBox::warning(this, "Ошибка", "Не удалось вставить");
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

        if (!addBook2(new_appointment))
            QMessageBox::warning(this, "Ошибка", "Не удалось вставить");
    }
}

void MainWindow::openInpDelDialogBook1(){
    inputDialogBook1 dialog(this);
    dialog.usingAsDelInput();
    if (dialog.exec() == QDialog::Accepted){
        Services dele_service;
        dele_service.service_name = dialog.getServiceNameInput();
        if (!deleteFromBook1(dele_service))
            QMessageBox::warning(this, "Ошибка", "Не удалось удалить");
    }
}

void MainWindow::openInpDelDialogBook2(){
    InputDialogBook2 dialog(this);
    dialog.usingAsDelInput();
    if (dialog.exec() == QDialog::Accepted){
        Appointment dele_appointment;
        dele_appointment.service_name = dialog.getServiceNameInput();
        dele_appointment.id = dialog.getCustomerInput().toInt();
        if (! deleteFromBook2(dele_appointment))
            QMessageBox::warning(this, "Ошибка", "Не удалось удалить");
    }
}

void MainWindow::openInpFinDialogBook1(){
    inputDialogBook1 dialog(this);
    dialog.usingAsDelInput();
    if (dialog.exec() == QDialog::Accepted){
        Services find_service;
        find_service.service_name = dialog.getServiceNameInput();
        if (!searchAndPrintBook1(find_service))
            QMessageBox::warning(this, "Ошибка", "Не удалось найти");
    }
}

void MainWindow::openInpFinDialogBook2(){
    InputDialogBook2 dialog(this);
    dialog.usingAsFindInput();
    if (dialog.exec() == QDialog::Accepted){
        Appointment find_appointment;
        find_appointment.service_name = dialog.getServiceNameInput();
        if (!searchAndPrintBook2(find_appointment))
            QMessageBox::warning(this, "Ошибка", "Не удалось найти");
    }
}

void MainWindow::openFileBook1(){
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл услуг", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }

    bool ok1;
    bool ok2;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(";");
        if (parts.size() >= 3) {
            Services service;
            service.service_name = parts[0].trimmed();
            service.price = parts[1].trimmed().toInt(&ok1);
            service.duration = parts[2].trimmed().toInt(&ok2);

            if (!(ok1 && ok2 && service.price >0 && service.duration >0)) continue;
            if (ht.search(service.service_name)) continue;
            service.id = book1id++;
            addBook1(service);
        }
    }
    file.close();
}

void MainWindow::openFileBook2(){
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл записей", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(";");
        if (parts.size() >= 4) {
            Appointment appointment;
            appointment.service_name = parts[0].trimmed();
            appointment.customer = parts[1].trimmed();
            appointment.executer = parts[2].trimmed();
            appointment.date =  QLocale("en_US").toDate(parts[3].trimmed(), "dd.MMM.yyyy");

            if (!ht.search(appointment.service_name)) continue;
            appointment.id = book2id++;
            addBook2(appointment);
        }
    }
    file.close();
}

void MainWindow::saveBook1ToFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить услуги", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать файл.");
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    for (int row = 0; row < ui->Book1->rowCount(); ++row) {
        QString service = ui->Book1->item(row, 0)->text();
        QString price = ui->Book1->item(row, 1)->text();
        QString duration = ui->Book1->item(row, 2)->text();
        QString id = ui->Book1->item(row, 3)->text();

        out << service << ";" << price << ";" << duration << ";" << id << "\n";
    }

    file.close();
    QMessageBox::information(this, "Успех", "Файл сохранён: " + fileName);
}

void MainWindow::saveBook2ToFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить записи", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать файл.");
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    for (int row = 0; row < ui->Book2->rowCount(); ++row) {
        QString service = ui->Book2->item(row, 0)->text();
        QString customer = ui->Book2->item(row, 1)->text();
        QString executer = ui->Book2->item(row, 2)->text();
        QString date = ui->Book2->item(row, 3)->text();
        QString id = ui->Book2->item(row, 4)->text();

        out << service << ";" << customer << ";" << executer << ";" << date << ";" << id << "\n";
    }

    file.close();
    QMessageBox::information(this, "Успех", "Файл сохранён: " + fileName);
}

void MainWindow::saveResult(){
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить отчета", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать файл.");
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    //"Услуга", "Клиент", "Цена", "Дата", "Мастер", "Длительность"
    for (int row = 0; row < ui->reportTable->rowCount(); ++row) {
        QString service = ui->reportTable->item(row, 0)->text();
        QString customer = ui->reportTable->item(row, 1)->text();
        QString price = ui->reportTable->item(row, 2)->text();
        QString date = ui->reportTable->item(row, 3)->text();
        QString executer = ui->reportTable->item(row, 4)->text();
        QString duration = ui->reportTable->item(row, 5)->text();

        out << service << ";" << customer << ";" <<price<< ";" << date << ";" << executer << ";" << duration << "\n";
    }

    file.close();
    QMessageBox::information(this, "Успех", "Файл сохранён: " + fileName);
}

void MainWindow::on_reportmakeButton_clicked()
{
    createAndShowReport();
}

//void MainWindow::setHTsize() {
 //   HT_input dialog(this);
  //  if (dialog.exec() == QDialog::Accepted) {
  //      int size = dialog.getHTSizeInput().toInt();
   //     if (size > 0) {
     //       ht = HashTable<QString, int>(size);
     //       QMessageBox::information(this, "Успех",
      //                               QString("Хэш-таблица создана с размером %1").arg(size));
      //  } else {
       //     QMessageBox::warning(this, "Ошибка", "Размер должен быть положительным");
//setHTsize();
 //       }
  //  }
//}
