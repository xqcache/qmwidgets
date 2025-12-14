#include "mainwindow.h"
#include "qmhardwareinfo.h"
#include "qmlogindialog.h"
#include "qmwidgets_global.h"
#include "stdafx.h"
#include <QPlainTextEdit>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
    : QmFramelessWindow(parent)
{
    QMWIDGETS_INITIALIZE_RESOURCE
    ui.setupUi(this);
    setTitleBar(ui.menubar);
}

MainWindow::~MainWindow() noexcept
{
}

// void MainWindow::on_widget_toggled(bool checked)
//{
//   qDebug() << checked;
// }

// void MainWindow::on_widget_valueChanged(double value)
//{
//   qDebug() << value;
// }
