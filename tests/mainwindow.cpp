#include "mainwindow.h"
#include "qmlogindialog.h"
#include "qmwidgets_global.h"
#include "stdafx.h"

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    QMWIDGETS_INITIALIZE_RESOURCE
    ui.setupUi(this);
    ui.frame->setRange(-1000, 1000);
    ui.frame->setMiddleValue(0);
    ui.frame->setAnimated(true);
}

// void MainWindow::on_widget_toggled(bool checked)
//{
//   qDebug() << checked;
// }

// void MainWindow::on_widget_valueChanged(double value)
//{
//   qDebug() << value;
// }
