#include "ht_input.h"
#include "ui_ht_input.h"
#include <QMessageBox>
#include <QIntValidator>

HT_input::HT_input(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HT_input)
{
    ui->setupUi(this);

    ui->lineEdit->setValidator(new QIntValidator(1, 10000, this));
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
    accept();}
