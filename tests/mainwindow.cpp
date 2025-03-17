#include "mainwindow.h"
#include "qmhardwareinfo.h"
#include "qmlogindialog.h"
#include "qmwidgets_global.h"
#include "stdafx.h"
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    QMWIDGETS_INITIALIZE_RESOURCE
    ui.setupUi(this);
    ui.frame->setRange(-1000, 1000);
    ui.frame->setMiddleValue(0);
    ui.frame->setAnimated(true);

    hardware_info_ = new QmHardwareInfo();

    auto* timer = new QTimer(this);
    timer->setInterval(1000);
    timer->callOnTimeout([this] { qDebug() << hardware_info_->totalCpuUsage() << hardware_info_->totalMemoryUsage(); });
    timer->start();
}

MainWindow::~MainWindow() noexcept
{
    delete hardware_info_;
}

// void MainWindow::on_widget_toggled(bool checked)
//{
//   qDebug() << checked;
// }

// void MainWindow::on_widget_valueChanged(double value)
//{
//   qDebug() << value;
// }
