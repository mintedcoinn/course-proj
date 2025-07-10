#include "input_window.h"
#include "ui_input_window.h"

input_window::input_window(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::input_window)
{
    ui->setupUi(this);
}

input_window::~input_window()
{
    delete ui;
}
