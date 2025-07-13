#ifndef INPUTDIALOGBOOK1_H
#define INPUTDIALOGBOOK1_H

#include <QDialog>
#include <QMessageBox>
namespace Ui {
class inputDialogBook1;
}

class inputDialogBook1 : public QDialog
{
    Q_OBJECT

public:
    explicit inputDialogBook1(QWidget *parent = nullptr);
    ~inputDialogBook1();
    QString getServiceNameInput();
    QString getPriceInput();
    QString getDurationInput();
    void usingAsDelInput();
private slots:

    void on_OKButton_clicked();

    void on_BackButton_clicked();

private:
    bool fields_hided = false;
    Ui::inputDialogBook1 *ui;
    QString service_input;
    QString price_input;
    QString duration_input;
    bool CheckCorrectInput();


};

#endif // INPUTDIALOGBOOK1_H
