#pragma once

#include <QtWidgets/QWidget>
#include "ui_mainwindow.h"

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

private Q_SLOTS:
//	void on_widget_valueChanged(double value);
	//void on_widget_toggled(bool checked);

private:
	Ui::MainWindowClass ui;
};
