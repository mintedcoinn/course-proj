#ifndef INPUTDIALOGBOOK2_H
#define INPUTDIALOGBOOK2_H

#include <QDialog>
#include <QDate>
#include <QMessageBox>
namespace Ui {
class InputDialogBook2;
}

class InputDialogBook2 : public QDialog
{
    Q_OBJECT

public:
    explicit InputDialogBook2(QWidget *parent = nullptr);
    ~InputDialogBook2();
    QString getServiceNameInput();
    QString getCustomerInput();
    QString getExecuterInput();
    QDate getDateInput();
    void usingAsDelInput();
    void usingAsFindInput();
private slots:

    void on_OKButton_clicked();

    void on_BackButton_clicked();

private:
    bool fields_hided_del = false;
    bool fields_hided_find = false;
    Ui::InputDialogBook2 *ui;
    QString service_input;
    QString customer_input;
    QString executer_input;
    QDate date_input;
    bool CheckCorrectInput();
};

#endif // INPUTDIALOGBOOK2_H
