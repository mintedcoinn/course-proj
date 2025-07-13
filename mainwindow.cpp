#include "mainwindow.h"
#include "./ui_mainwindow.h"

Services* book1 = new Services[1000];
Appointment* book2 = new Appointment[1000];
HashTable<QString,int> ht;

QQueue<QString>main_loger;

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
    QApplication::setWindowIcon(QIcon(":/coursach.ico"));
    this->setWindowTitle("Салон Красоты");
    setHTsize();
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
    saveLogsAction = ui->saveLogsAction;
    connect(saveLogsAction,&QAction::triggered,this,&MainWindow::saveLogs);
    printAppointmentTreeAction = ui->printAppointmentTreeAction;
    printDateTreeAction = ui->printDateTreeAction;
    printHTAction = ui->printHTAction;
    connect(printAppointmentTreeAction,&QAction::triggered,this,&MainWindow::printAppointmentTree);
    connect(printDateTreeAction,&QAction::triggered,this,&MainWindow::printDateTree);
    connect(printHTAction,&QAction::triggered,this,&MainWindow::printHT);

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::addBook1(Services &value)
{
    if (ht.search(value.service_name))
    {
        updateLogs();
        main_loger.enqueue(QString("Справочник1: ошибка добавления"));
        updateLogs();
        return false;
    }

    if (!(ht.insert(value.id, value.service_name)))
    {
        updateLogs();
        main_loger.enqueue(QString("Справочник1: ошибка добавления"));
        updateLogs();
        return false;
    }

    int current_row = ui->Book1->rowCount();
    ui->Book1->insertRow(current_row);

    ui->Book1->setItem(current_row,0,new QTableWidgetItem(value.service_name));
    ui->Book1->setItem(current_row,1,new QTableWidgetItem(QString::number(value.price)));
    ui->Book1->setItem(current_row,2,new QTableWidgetItem(QString::number(value.duration)));
    ui->Book1->setItem(current_row,3, new QTableWidgetItem(QString::number(value.id)));

    updateLogs();
    main_loger.enqueue(QString("Справочник1: добавлена запись с ID %1").arg(QVariant::fromValue(value.id).toString()));
    updateLogs();
    return true;
}

bool MainWindow::addBook2(Appointment &value){
    if (!ht.search(value.service_name))
    {
        updateLogs();
        main_loger.enqueue(QString("Справочник2: ошибка добавления"));
        updateLogs();
        return false;
    }
    int current_row = ui->Book2->rowCount();
    ui->Book2->insertRow(current_row);

    ui->Book2->setItem(current_row,0,new QTableWidgetItem(value.service_name));
    ui->Book2->setItem(current_row,1,new QTableWidgetItem(value.customer));
    ui->Book2->setItem(current_row,2,new QTableWidgetItem(value.executer));
    ui->Book2->setItem(current_row,3,new QTableWidgetItem(QLocale("en_US").toString(value.date, "dd.MMM.yyyy")));
    ui->Book2->setItem(current_row,4, new QTableWidgetItem(QString::number(value.id)));
    appointment_tree.insertValue(value.service_name,value.id);
    updateLogs();
    date_tree.insertValue(value.date,value.id);
    updateLogs();

    main_loger.enqueue(QString("Справочник2: добавлена запись с ID %1").arg(QVariant::fromValue(value.id).toString()));
    updateLogs();
    return true;
}

bool MainWindow::deleteFromBook1(Services &value){
    int idInBook =0;
    ht.search(value.service_name,&idInBook);
    if (idInBook<0)    {
        updateLogs();
        main_loger.enqueue(QString("Справочник1: ошибка удаления"));
        updateLogs();
        return false;
    }
    value.id = idInBook;
    int rowInBook = searchInBook(*ui->Book1, 3, idInBook);
    updateLogs();
    Appointment temp;
    temp.service_name = value.service_name;
    CLEAR_ALL_MENTIONS_OF_SERVICE = true;
    deleteFromBook2(temp);
    CLEAR_ALL_MENTIONS_OF_SERVICE = false;

    ht.remove(value.service_name, value.id);
    updateLogs();

    ui->Book1->removeRow(rowInBook);

    main_loger.enqueue(QString("Справочник1: запись с ID %1 удалена").arg(QVariant::fromValue(value.id).toString()));
    updateLogs();
    return true;
}

bool MainWindow::deleteFromBook2(Appointment &value){
    int idInBook =0;
    ht.search(value.service_name,&idInBook);
    if (idInBook<0)    {
        updateLogs();
        main_loger.enqueue(QString("Справочник2: ошибка удаления"));
        updateLogs();
        return false;
    }

    if (CLEAR_ALL_MENTIONS_OF_SERVICE)
    {
        DLL<int> idInNode;
        if (!appointment_tree.find(value.service_name,idInNode))
        {
            updateLogs();
            main_loger.enqueue(QString("Справочник2: ошибка удаления"));
            updateLogs();
            return false;
        }
        updateLogs();
        while (!idInNode.isEmpty()){
            int tempId = idInNode.getHead()->data;
            int rowInBook = searchInBook(*ui->Book2, 4, tempId);
            date_tree.checkAndDeleteValueInTree(idInNode.getHead()->data);
            updateLogs();
            idInNode.removeValue(idInNode.getHead()->data);
            updateLogs();
            ui->Book2->removeRow(rowInBook);
            main_loger.enqueue(QString("Справочник2: запись с ID %1 удалена").arg(QVariant::fromValue(tempId).toString()));
            updateLogs();
        }
        appointment_tree.removeNode(value.service_name);
        updateLogs();
    }

    else
    {
        int rowInBook = searchInBook(*ui->Book2, 4, value.id);
        QTableWidgetItem* item = ui->Book2->item(rowInBook, 4);
        QDate tempDate = QDate::fromString(item->text(), "dd-MMM-yyyy");

        appointment_tree.removeValue(value.service_name, value.id);
        updateLogs();
        date_tree.removeValue(tempDate, value.id);
        updateLogs();
        ui->Book2->removeRow(rowInBook);
        main_loger.enqueue(QString("Справочник2: запись с ID %1 удалена").arg(QVariant::fromValue(value.id).toString()));
        updateLogs();
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

                main_loger.enqueue(QString("Значениу %2 найдено в строке %3").arg(QVariant::fromValue(book.objectName()).toString()).arg(QVariant::fromValue(desired_id).toString()).arg(QVariant::fromValue(i).toString()));
                updateLogs();

                return i;
            }
        }
    }
    main_loger.enqueue(QString("Поиск по строке справочника провален").arg(QVariant::fromValue(book.objectName()).toString()));
    updateLogs();
    return -1;
}

bool MainWindow::searchAndPrintBook1(Services &value){
    int idInBook = 0;
    int step_counter;
    ht.search(value.service_name,&idInBook, &step_counter);
    if (idInBook<0)    {
        updateLogs();
        main_loger.enqueue(QString("Справочник1: запись не найдена"));
        updateLogs();
        return false;
    }

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

    main_loger.enqueue(QString("Справочник1: поиск завершен"));
    updateLogs();
    return true;
}

bool MainWindow::searchAndPrintBook2(Appointment &value){
    int idInBook = 0;
    ht.search(value.service_name,&idInBook);
    if (idInBook<0)    {
        updateLogs();
        main_loger.enqueue(QString("Справочник2: запись не найдена"));
        updateLogs();
        return false;
    }

    DLL<int> idInNode;
    int step_counter;
    if (!appointment_tree.find(value.service_name,idInNode,&step_counter))
    {
        updateLogs();
        main_loger.enqueue(QString("Справочник2: запись не найдена"));
        updateLogs();
        return false;
    }

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
        main_loger.enqueue(QString("Таблица поиска справочника2: вставлен элемент с id =  %1").arg(QVariant::fromValue(idInNode.getHead()->data).toString()));
        idInNode.removeValue(idInNode.getHead()->data);
        updateLogs();
    }
    main_loger.enqueue(QString("Справочник2: поиск завершен"));
    updateLogs();
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
        main_loger.enqueue(QString("Отчёт: ошибка ввода данных"));
        updateLogs();
        return;
    }
    if (start_period> end_period){
        QMessageBox::warning(this, "Ошибка", "Начальная дата больше конченой");
        main_loger.enqueue(QString("Отчёт: ошибка ввода данных"));
        updateLogs();
        return;
    }

    if (!ok)
    {
        QMessageBox::warning(this, "Ошибка", "Цена является целочисленным показателем");
        main_loger.enqueue(QString("Отчёт: ошибка ввода данных"));
        updateLogs();
        return;
    }

    ui->reportTable->clearContents();
    ui->reportTable->setRowCount(0);

    DLL<int> services_id;
    DLL<pair<QString, int>> namesFromId;

    for (QDate date = start_period; date <= end_period; date = date.addDays(1)){
        DLL<int> listId;
        if (!date_tree.find(date,listId))
        {
            updateLogs();
            continue;
        }

        DLL<int> temp;
        temp = listId;
        while (!temp.isEmpty())
        {
            int rowInBook2 = searchInBook(*ui->Book2,4,temp.getHead()->data);
            QTableWidgetItem *item = ui->Book2->item(rowInBook2,1);
            QString appo_cutomer = item->text();
            if (appo_cutomer == customer){
                services_id.append(temp.getHead()->data);
                updateLogs();
            }
            temp.removeValue(temp.getHead()->data);
            updateLogs();
        }
        temp.clear();
        updateLogs();
    }

    while (!services_id.isEmpty()){
        int rowInBook2 = searchInBook(*ui->Book2,4,services_id.getHead()->data);
        QTableWidgetItem *item = ui->Book2->item(rowInBook2,0);
        QString service_name = item->text();
        namesFromId.append(make_pair(service_name, services_id.getHead()->data));
        updateLogs();
        services_id.removeValue(services_id.getHead()->data);
        updateLogs();
    }

    services_id.clear();
    updateLogs();
    while (!namesFromId.isEmpty()){
        int idInBook1 =0;

        if (!ht.search(namesFromId.getHead()->data.first, &idInBook1)){
            updateLogs();
            namesFromId.removeValue(namesFromId.getHead()->data);
            updateLogs();
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

            main_loger.enqueue(QString("Отчёт: случай найден в строке %1 в справочнике1").arg(QVariant::fromValue(rowInBook1).toString()));
            main_loger.enqueue(QString("Отчёт: случай найден в строке %1 в справочнике1").arg(QVariant::fromValue(rowInBook2).toString()));
            main_loger.enqueue(QString("Отчёт: добавлена строка %1").arg(QVariant::fromValue(current_row).toString()));
            updateLogs();
        }
        namesFromId.removeValue(namesFromId.getHead()->data);
        updateLogs();
    }
    namesFromId.clear();
    updateLogs();
    main_loger.enqueue(QString("Отчёт: создание завершено"));
    updateLogs();
}

void MainWindow::setHTsize() {
    HT_input dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        int size = dialog.getTableSize();
        if (size > 0) {
            HashTable<QString, int>temp(size) ;
            ht = temp;
            QMessageBox::information(this, "Успех", QString("Хэш-таблица создана с размером %1").arg(size));
            main_loger.enqueue(QString("Изменение размера хеш таблицы: используется размер %1").arg(size));
            updateLogs();
        } else {
            QMessageBox::warning(this, "Ошибка", "Размер должен быть положительным");
            main_loger.enqueue(QString("Изменение размера хеш таблицы: ошибка"));
            updateLogs();
            setHTsize();
        }
    }
}

void MainWindow::updateLogs() {

    QQueue<QString> allLogs;

    if (!ht.loger.isEmpty()) {
        allLogs.append(ht.loger);
        ht.loger.clear();
    }

    if (!appointment_tree.loger.isEmpty()) {
        allLogs.append(appointment_tree.loger);
        appointment_tree.loger.clear();
    }
    if (!date_tree.loger.isEmpty()) {
        allLogs.append(date_tree.loger);
        date_tree.loger.clear();
    }

    while (!allLogs.isEmpty()) {
        main_loger.enqueue(allLogs.dequeue());
    }

    while (!main_loger.isEmpty()) {
        ui->logs->appendPlainText(main_loger.dequeue());
    }
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
    if (fileName.isEmpty())
    {
        main_loger.enqueue(QString("Файл: ошибка открытия"));
        updateLogs();
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        main_loger.enqueue(QString("Файл: ошибка открытия"));
        updateLogs();
        return;
    }

    bool ok1;
    bool ok2;
    int current_row = 0;
    QTextStream in(&file);

    main_loger.enqueue(QString("Файл: начато чтение"));
    updateLogs();
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(";");
        if (parts.size() >= 3)
        {
            Services service;
            service.service_name = parts[0].trimmed();
            service.price = parts[1].trimmed().toInt(&ok1);
            service.duration = parts[2].trimmed().toInt(&ok2);

            if (!(ok1 && ok2 && service.price >0 && service.duration >0))
            {
                main_loger.enqueue(QString("Файл: пропущена строка %1 (некорректные данные)").arg(QVariant::fromValue(current_row).toString()));
                updateLogs();
                current_row++;
                continue;
            }
            if (ht.search(service.service_name))
            {
                main_loger.enqueue(QString("Файл: пропущена строка %1 (некорректные данные)").arg(QVariant::fromValue(current_row).toString()));
                updateLogs();
                current_row++;
                continue;
            }
            service.id = book1id++;
            addBook1(service);

            main_loger.enqueue(QString("Файл: обработана строка %1").arg(QVariant::fromValue(current_row).toString()));
            updateLogs();
        }
        else{
            main_loger.enqueue(QString("Файл: пропущена строка %1 (некорректные данные)").arg(QVariant::fromValue(current_row).toString()));
            updateLogs();
            current_row++;
            continue;
        }
        current_row++;
    }
    file.close();
    main_loger.enqueue(QString("Файл: закончено чтение"));
    updateLogs();
}

void MainWindow::openFileBook2(){
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл записей", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty())
    {
        main_loger.enqueue(QString("Файл: ошибка открытия"));
        updateLogs();
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        main_loger.enqueue(QString("Файл: ошибка открытия"));
        updateLogs();
        return;
    }

    int current_row = 0;
    QTextStream in(&file);
    main_loger.enqueue(QString("Файл: начато чтение"));
    updateLogs();
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(";");
        if (parts.size() >= 4) {
            Appointment appointment;
            appointment.service_name = parts[0].trimmed();
            appointment.customer = parts[1].trimmed();
            appointment.executer = parts[2].trimmed();
            appointment.date =  QLocale("en_US").toDate(parts[3].trimmed(), "dd.MMM.yyyy");
            if (!appointment.date.isValid()){
                main_loger.enqueue(QString("Файл: пропущена строка %1 (некорректные данные)").arg(QVariant::fromValue(current_row).toString()));
                updateLogs();
                current_row++;
                continue;
            }
            if (!ht.search(appointment.service_name))
            {
                main_loger.enqueue(QString("Файл: пропущена строка %1 (некорректные данные)").arg(QVariant::fromValue(current_row).toString()));
                updateLogs();
                current_row++;
                continue;
            }
            appointment.id = book2id++;
            addBook2(appointment);
            main_loger.enqueue(QString("Файл: обработана строка %1").arg(QVariant::fromValue(current_row).toString()));
            updateLogs();
        }
        else{
            main_loger.enqueue(QString("Файл: пропущена строка %1 (некорректные данные)").arg(QVariant::fromValue(current_row).toString()));
            updateLogs();
            current_row++;
            continue;
        }
        current_row++;
    }
    file.close();
    main_loger.enqueue(QString("Файл: закончено чтение"));
    updateLogs();
}

void MainWindow::saveBook1ToFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить услуги", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty())
    {
        main_loger.enqueue(QString("Файл: ошибка сохранения"));
        updateLogs();
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать файл.");
        main_loger.enqueue(QString("Файл: ошибка сохранения"));
        updateLogs();
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    main_loger.enqueue(QString("Файл: запись начата"));
    updateLogs();
    for (int row = 0; row < ui->Book1->rowCount(); ++row) {
        QString service = ui->Book1->item(row, 0)->text();
        QString price = ui->Book1->item(row, 1)->text();
        QString duration = ui->Book1->item(row, 2)->text();
        QString id = ui->Book1->item(row, 3)->text();

        out << service << ";" << price << ";" << duration << ";" << id << "\n";

        main_loger.enqueue(QString("Файл: обработана строка %1").arg(QVariant::fromValue(row).toString()));
        updateLogs();
    }

    file.close();
    main_loger.enqueue(QString("Файл: запись закончена"));
    updateLogs();
    QMessageBox::information(this, "Успех", "Файл сохранён: " + fileName);
}

void MainWindow::saveBook2ToFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить записи", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty())
    {
        main_loger.enqueue(QString("Файл: ошибка сохранения"));
        updateLogs();
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать файл.");
        main_loger.enqueue(QString("Файл: ошибка сохранения"));
        updateLogs();
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    main_loger.enqueue(QString("Файл: запись начата"));
    updateLogs();
    for (int row = 0; row < ui->Book2->rowCount(); ++row) {
        QString service = ui->Book2->item(row, 0)->text();
        QString customer = ui->Book2->item(row, 1)->text();
        QString executer = ui->Book2->item(row, 2)->text();
        QString date = ui->Book2->item(row, 3)->text();
        QString id = ui->Book2->item(row, 4)->text();

        out << service << ";" << customer << ";" << executer << ";" << date << ";" << id << "\n";
        main_loger.enqueue(QString("Файл: обработана строка %1").arg(QVariant::fromValue(row).toString()));
        updateLogs();
    }

    file.close();
    main_loger.enqueue(QString("Файл: запись закончена"));
    updateLogs();
    QMessageBox::information(this, "Успех", "Файл сохранён: " + fileName);
}

void MainWindow::saveResult(){
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить отчета", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty())
    {
        main_loger.enqueue(QString("Файл: ошибка сохранения"));
        updateLogs();
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать файл.");
        main_loger.enqueue(QString("Файл: ошибка сохранения"));
        updateLogs();
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    main_loger.enqueue(QString("Файл: запись начата"));
    updateLogs();
    //"Услуга", "Клиент", "Цена", "Дата", "Мастер", "Длительность"
    for (int row = 0; row < ui->reportTable->rowCount(); ++row) {
        QString service = ui->reportTable->item(row, 0)->text();
        QString customer = ui->reportTable->item(row, 1)->text();
        QString price = ui->reportTable->item(row, 2)->text();
        QString date = ui->reportTable->item(row, 3)->text();
        QString executer = ui->reportTable->item(row, 4)->text();
        QString duration = ui->reportTable->item(row, 5)->text();

        out << service << ";" << customer << ";" <<price<< ";" << date << ";" << executer << ";" << duration << "\n";
        main_loger.enqueue(QString("Файл: обработана строка %1").arg(QVariant::fromValue(row).toString()));
        updateLogs();
    }

    file.close();
    main_loger.enqueue(QString("Файл: запись закончена"));
    updateLogs();
    QMessageBox::information(this, "Успех", "Файл сохранён: " + fileName);
}

void MainWindow::saveLogs(){
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить события", "", "Текстовые файлы (*.txt);;Все файлы (*)");
    if (fileName.isEmpty())
    {
        main_loger.enqueue(QString("Файл: ошибка сохранения"));
        updateLogs();
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать файл.");
        main_loger.enqueue(QString("Файл: ошибка сохранения"));
        updateLogs();
        return;
    }

    main_loger.enqueue(QString("Файл: запись начата"));
    updateLogs();

    QTextStream out(&file);
    out.setCodec("UTF-8");

    QString logs = ui->logs->toPlainText();
    out <<logs;
    file.close();
    main_loger.enqueue(QString("Файл: запись закончена"));
    updateLogs();
    QMessageBox::information(this, "Успех", "Файл сохранён: " + fileName);
}

void MainWindow::printAppointmentTree() {
    QString treeContent = appointment_tree.printTree();
    ui->logs->appendPlainText(treeContent);
    main_loger.enqueue("AVL-дерево: дерево записей напечатано");
    updateLogs();
}

void MainWindow::printDateTree() {
    QString treeContent = date_tree.printTree();

    ui->logs->appendPlainText(treeContent);
    main_loger.enqueue("AVL-дерево: дерево дат напечатано");
    updateLogs();
}

void MainWindow::printHT() {
    QString htContent = ht.print();
    ui->logs->appendPlainText(htContent);
    main_loger.enqueue("Хеш-таблица: таблица услуг напечатана"
);
    updateLogs();
}

void MainWindow::on_reportmakeButton_clicked()
{
    createAndShowReport();
}

