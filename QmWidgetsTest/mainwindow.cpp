#include "stdafx.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//ui.widget->setInteractive(true);

	//connect(ui.widget, &QmThermometer::valueChanged, this, &MainWindow::);
}

//void MainWindow::on_widget_toggled(bool checked)
//{
//	qDebug() << checked;
//}

//void MainWindow::on_widget_valueChanged(double value)
//{
//	qDebug() << value;
//}
