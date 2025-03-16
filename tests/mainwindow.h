#pragma once

#include "ui_mainwindow.h"
#include <QtWidgets/QWidget>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = Q_NULLPTR);

private Q_SLOTS:
    //	void on_widget_valueChanged(double value);
    // void on_widget_toggled(bool checked);

private:
    Ui::MainWindowClass ui;
};
