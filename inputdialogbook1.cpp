#include "inputdialogbook1.h"
#include "ui_inputdialogbook1.h"

inputDialogBook1::inputDialogBook1(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::inputDialogBook1)
{
    ui->setupUi(this);
    this->setWindowTitle("Окно ввода справочника1");
}

inputDialogBook1::~inputDialogBook1()
{
    delete ui;
}

void inputDialogBook1::on_OKButton_clicked()
{
    service_input = ui->serviceNameInput->toPlainText();
    price_input = ui->priceInput->toPlainText();
    duration_input = ui->durationInput->toPlainText();
    if (CheckCorrectInput())this->accept();
}

void inputDialogBook1::on_BackButton_clicked()
{
    this->reject();
}
bool inputDialogBook1::CheckCorrectInput(){
    if (fields_hided){
        if (service_input.trimmed().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены");
            return false;
        }
        return true;
    }
    else
    {
        if (service_input.trimmed().isEmpty() || price_input.trimmed().isEmpty() || duration_input.trimmed().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены");
            return false;
        }

        bool price_int_and_correct;
        int temp = price_input.toInt(&price_int_and_correct);

        if (!price_int_and_correct) {
            QMessageBox::warning(this, "Ошибка", "Цена записывается в целочисленном количестве рублей");
            return false;
        }
        if( temp <= 0){
            QMessageBox::warning(this, "Ошибка", "Цена не может быть меньше 0");
            return false;
        }
        bool duration_int_and_correct;
        int temp2 = duration_input.toInt(&duration_int_and_correct);
        if (!duration_int_and_correct){
            QMessageBox::warning(this, "Ошибка", "Длительность записывается в целочисленном количестве минут");
            return false;
        }
        if( temp2 <= 0){
            QMessageBox::warning(this, "Ошибка", "Длительность не может быть меньше 0");
            return false;
        }
        return true;
    }
}
void inputDialogBook1::usingAsDelInput(){
    ui->priceInput->hide();
    ui->durationInput->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    fields_hided = true;
}


QString inputDialogBook1::getServiceNameInput(){
    return service_input;
}
QString inputDialogBook1::getPriceInput(){
    return price_input;
}
QString inputDialogBook1::getDurationInput(){
    return duration_input;
}
