#include "inputdialogbook2.h"
#include "ui_inputdialogbook2.h"

InputDialogBook2::InputDialogBook2(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InputDialogBook2)
    {
        ui->setupUi(this);
    }

InputDialogBook2::~InputDialogBook2()
{
    delete ui;
}
void InputDialogBook2::on_OKButton_clicked()
{
    service_input = ui->serviceNameInput->toPlainText();
    customer_input = ui->customerInput->toPlainText();
    executer_input = ui->executerInput->toPlainText();
    date_input = ui->dateInput->date();
    if (CheckCorrectInput())this->accept();
}

void InputDialogBook2::on_BackButton_clicked()
{
    this->reject();
}

bool InputDialogBook2::CheckCorrectInput(){
    if (fields_hided){
        if (service_input.trimmed().isEmpty() || customer_input.trimmed().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены");
            return false;
        }
        bool ok;
        int temp =customer_input.toInt(&ok);
        if (!(ok && temp>=0)){
            QMessageBox::warning(this, "Ошибка", "ID заполняется целочтсленными значениями не меньше 0");
            return false;
        }
    }
    else
    {
        if (service_input.trimmed().isEmpty() || customer_input.trimmed().isEmpty() || executer_input.trimmed().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены");
            return false;
        }
    }
    return true;
}

void InputDialogBook2::usingAsDelInput(){
    ui->label_2->setText("ID");
    ui->label_3->hide();
    ui->label_4->hide();
    ui->dateInput->hide();
    ui->executerInput->hide();
    fields_hided = true;
}

QString InputDialogBook2::getServiceNameInput(){
    return service_input;
}
QString InputDialogBook2::getCustomerInput(){
    return customer_input;
}
QString InputDialogBook2::getExecuterInput(){
    return executer_input;
}
QDate InputDialogBook2::getDateInput(){
    return date_input;
}
