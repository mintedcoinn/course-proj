#ifndef INPUT_WINDOW_H
#define INPUT_WINDOW_H

#include <QDialog>

namespace Ui {
class input_window;
}

class input_window : public QDialog
{
    Q_OBJECT

public:
    explicit input_window(QWidget *parent = nullptr);
    ~input_window();

private:
    Ui::input_window *ui;
};

#endif // INPUT_WINDOW_H
