#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_N_clicked();

    void on_Random_clicked();

    QString random();

    void Calculator();

    void on_Begin_clicked();

    void on_Begin_pressed();

    void Display(int value);

    double Calculate_F();

    void Fake_situation();
    \
    void Main_loop(int u[], double** p);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
