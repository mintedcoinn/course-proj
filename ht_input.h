#ifndef HT_INPUT_H
#define HT_INPUT_H

#include <QDialog>

namespace Ui {
class HT_input;
}

class HT_input : public QDialog
{
    Q_OBJECT

public:
    explicit HT_input(QWidget *parent = nullptr);
    ~HT_input();
    int getTableSize() const;

private slots:
    void on_pushButton_clicked();

private:
    Ui::HT_input *ui;

};

#endif // HT_INPUT_H
