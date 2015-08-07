#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../../goodies/roundbutton/roundbutton.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    RoundButton *bt_RoundButton;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
