#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   setMouseTracking(true);

   //With this call, everything will be set
   mControl.createView(this);
}

MainWindow::~MainWindow()
{
   delete ui;
}
