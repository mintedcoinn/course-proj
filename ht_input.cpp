#include "ht_input.h"
#include "ui_ht_input.h"
#include <QMessageBox>
#include <QIntValidator>

HT_input::HT_input(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HT_input)
{
    ui->setupUi(this);
this->setWindowTitle("Укажите размер хэш-таблицы");
    ui->lineEdit->setPlaceholderText("Введите число 1-10000");
}

HT_input::~HT_input()
{
    delete ui;
}

int HT_input::getTableSize() const
{
    return ui->lineEdit->text().toInt();
}

void HT_input::on_pushButton_clicked()
{
    if (ui->lineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите размер таблицы");
        return;
    }
    bool ok;
    int temp  = ui->lineEdit->text().toInt(&ok);
    if (!(ok && temp >0)) {
        QMessageBox::warning(this, "Ошибка", "Размер таблицы должен быть положительным");
        return;
    }
    accept();
}
