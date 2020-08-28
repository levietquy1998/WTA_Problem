#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <vector>
#include "wta.h"

using namespace std;
vector<double> X ;
int n = 0;
int weapon, target;
double Ta = 0.001;
int Number_of_loop = 1000000;
bool check_box = false;
int save_cycle = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Begin->hide();
    ui->Random->hide();
    ui->checkBox->hide();
    ui->label_5->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//When click the "Set" button
void MainWindow::on_N_clicked()
{
    //Get the number of weapon and target
    weapon = ui->lineEdit->text().toInt();
    target = ui->lineEdit_2->text().toInt();
    if(weapon > target) n = weapon;
    else n = target;
    //Set enable for the Buttons
    ui->Begin->show();
    ui->Random->show();
    if(weapon == target && n<10) ui->checkBox->show();
    else ui->checkBox->hide();
    //Set the TableWidget for display data of Matrix P, U, X
    ui->P->setColumnCount(n);
    ui->P->setRowCount(n);
    ui->U->setColumnCount(1);
    ui->U->setRowCount(n);
    ui->X->setColumnCount(n);
    ui->X->setRowCount(n);
    //Scale size of each items of TableWidget
    for (int i=0; i<n; i++)
    {
        ui->P->setColumnWidth(i,(ui->P->size().width())/n);
        ui->P->setRowHeight(i,(ui->P->size().height())/n);

        ui->U->setRowHeight(i,(ui->U->size().height())/n);
        ui->U->setColumnWidth(i,ui->U->size().width());

        ui->X->setColumnWidth(i,(ui->X->size().width())/n);
        ui->X->setRowHeight(i,(ui->X->size().height())/n);

        //Clear the previous data
        ui->U->setItem(i,0,new QTableWidgetItem(""));
        for(int j=0; j<n; j++)
        {
            ui->P->setItem(i,j,new QTableWidgetItem(""));
            ui->X->setItem(i,j,new QTableWidgetItem(""));
        }
    }
    //Remove the ScrollBar
    ui->P->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->P->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->U->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->U->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->X->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->X->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
//When click the "Random" button
void MainWindow::on_Random_clicked()
{
    // Random data of P matrix
    for (int i=0; i<n*n; i++)
    {
        QTableWidgetItem *a = new QTableWidgetItem("");
        a->setText(random());
        ui->P->setItem(i/n,i%n,a);
        ui->P->item(i/n,i%n)->setTextAlignment(Qt::AlignCenter);
    }
    // Create data of U maxtrix
    for (int i=0; i<n; i++)
    {
        QTableWidgetItem *a = new QTableWidgetItem("");
        a->setText(QString::number(i+1));
        ui->U->setItem(i,0,a);
        ui->U->item(i,0)->setTextAlignment(Qt::AlignCenter);
    }
    if(weapon != target) Fake_situation();
}
//When number of target not equal number of weapon
void MainWindow::Fake_situation()
{
    //Fake weapon when weapon > target
    if(weapon<target)
    {
        for(int i=0; i<n; i++)
        {
            for(int j=weapon; j<n; j++)
            {
                ui->P->item(i,j)->setText("");
            }
        }
    }
    //Fake target when weapon < target
    else if(weapon>target)
    {
        for(int i=target; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                ui->P->item(i,j)->setText("");
            }
            ui->U->item(i,0)->setText("");
        }
    }
}
//Random function
QString MainWindow::random()
{
    double res = rand()%100 /(double)100;
    return QString::number(res);
}
//Display data
void MainWindow::Display(int value)
{
    switch(value)
    {
    case 1:
        //Display the assignment with red color
        for (int i=0; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                ui->X->setItem(i,j,new QTableWidgetItem(QString::number(X[i*n+j])));
                if(X[i*n+j]==1)
                {
                    ui->X->item(i,j)->setBackgroundColor(Qt::red);
                }
                ui->X->item(i,j)->setTextAlignment(Qt::AlignCenter);
            }
        }
        break;
    case 2:
        //Display the assignment with red color
        for (int i=0; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                ui->X->setItem(i,j,new QTableWidgetItem(QString::number(X[i*n+j])));
                if(X[i*n+j]==1)
                {
                    ui->X->item(i,j)->setBackgroundColor(Qt::red);
                }
                ui->X->item(i,j)->setTextAlignment(Qt::AlignCenter);
            }
        }
        //Hide the fake weapon
        for (int i=0; i<n; i++)
        {
            for (int j=weapon; j<n; j++)
            {
                ui->X->item(i,j)->setText("");
                ui->X->item(i,j)->setBackgroundColor(Qt::white);
            }
        }
        break;
    case 3:
        // Display result by 2-time loop with color is yellow
        for (int i=0; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                int a;
                a = ui->X->item(i,j)->text().toInt();
                if(X[i*n+j]==1)
                {
                    if(a == 0)
                    {
                       ui->X->item(i,j)->setBackgroundColor(Qt::yellow);
                       ui->X->item(i,j)->setText("1");
                    }
                }
            }
        }
        for (int i=0; i<target; i++)
        {
            for(int j=0; j<n; j++)
            {
                if(ui->X->item(i,j)->backgroundColor() == Qt::red)
                {
                    for(int m=0; m<n; m++)
                    {
                        if(m!=i)
                        {
                            ui->X->item(m,j)->setText("0");
                            ui->X->item(m,j)->setBackgroundColor(Qt::white);
                        }
                    }
                }
            }
        }
        //Hide the fake target
        for (int i=target; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                ui->X->item(i,j)->setText("");
                ui->X->item(i,j)->setBackgroundColor(Qt::white);
            }
        }
        break;
    }

}
//Calculate F from u,p and x
double MainWindow::Calculate_F()
{
    int* u = new int[n];
    double** p = new double*[n];
    //Get u and p value
    for (int i = 0; i < n; i++)
    {
        p[i] = new double[n];
        u[i] = ui->U->item(i,0)->text().toInt();
    }
    for (int i = 0; i<n*n; i++)
    {
           p[i/n][i%n] = ui->P->item(i/n,i%n)->text().toDouble();
    }

    double* g = new double[n];
    //Get X value
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
           X[i*n+j]=ui->X->item(i,j)->text().toDouble();
        }
    }
    //Cal_g
    for (int i = 0; i < n; i++)
    {
        g[i] = 1;
        for (int j = 0; j < n; j++)
        {
            g[i] = g[i] * pow((double)(1 - p[i][j]), (double)(X[i * n + j]));
        }
    }
    //Cal_F
    double F = 0;
    for (int i=0; i<n; i++)
    {
        F += g[i]*u[i];
    }
    return F;
}
//Main loop
void MainWindow::Main_loop(int u[], double** p)
{
    if(weapon == target)
    {
        save_cycle = Calculate_WTA(u,p,n,Ta,Number_of_loop,check_box);
        Display(1);
    }
    else if(weapon < target)
    {
        Calculate_WTA(u,p,n,Ta,Number_of_loop,check_box);
        Display(2);
    }
    else
    {
        Calculate_WTA(u,p,n,Ta,Number_of_loop,check_box);
        Display(1);
        //Prepare data for 2-time loop
        for(int i=0; i<target; i++)
        {
            for(int j=0; j<n; j++)
            {
                if(X[n*i+j] == 1)
                {
                    for(int m=0; m<n; m++)
                    {
                        p[m][j] = 0;
                    }
                }
            }
        }
        X.clear();
        for (int i = 0; i<n*n; i++)
        {
            X.push_back(0);
        }
        Calculate_WTA(u,p,n,Ta,Number_of_loop,0);
        Display(3);
    }
}
//Main Function
void MainWindow ::Calculator()
{
    if(weapon != target) Fake_situation();
    if(ui->checkBox->isChecked() == true) check_box=1;
    else check_box = 0;
    clock_t start = clock();
    //Define varriable
    int* u = new int[n];
    double** p = new double*[n];
    for (int i = 0; i < n; i++)
    {
        p[i] = new double[n];
        u[i] = ui->U->item(i,0)->text().toInt();
    }
    X.clear();
    for (int i = 0; i<n*n; i++)
    {
           p[i/n][i%n] = ui->P->item(i/n,i%n)->text().toDouble();
           X.push_back(0);
    }
    //Main loop
    Main_loop(u,p);
    //Display F and Time
    ui->label_5->setText("F = " + QString::number(Calculate_F())) ;
    if (save_cycle != 0) ui->time->setText( "Time = " + QString::number((double)(clock() - start) / CLOCKS_PER_SEC) + " s. Save time: " + QString::number(((double)(clock() - start) / CLOCKS_PER_SEC)*(1000000-save_cycle)/save_cycle) + "s");
    else ui->time->setText( "Time = " + QString::number((double)(clock() - start) / CLOCKS_PER_SEC) + " s");
    //Free variables
    for (int i = 0; i < n; i++)
    {
        delete []p[i];
    }
    delete []p;
    delete []u;
}

void MainWindow::on_Begin_clicked()
{
  ui->Begin->setEnabled(false);
  Calculator();
  ui->Begin->setEnabled(true);
}

void MainWindow::on_Begin_pressed()
{
    ui->time->setText("Processing ...");
}
