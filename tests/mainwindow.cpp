#include "mainwindow.h"
#include "qmlogindialog.h"
#include "qmwidgets_global.h"
#include "stdafx.h"

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    QMWIDGETS_INITIALIZE_RESOURCE
    ui.setupUi(this);

    ui.widget_3->setShadowEnabled(true);
    ui.widget_3->setShadowEnabled(false);
    // ui.widget->setInteractive(true);

    // connect(ui.widget, &QmThermometer::valueChanged, this, &MainWindow::);
    connect(ui.pushButton, &QPushButton::clicked, this, [this] {
        ui.widget_3->setShadowEnabled(!ui.widget_3->isShadowEnabled());
    });
}

// void MainWindow::on_widget_toggled(bool checked)
//{
//   qDebug() << checked;
// }

// void MainWindow::on_widget_valueChanged(double value)
//{
//   qDebug() << value;
// }
