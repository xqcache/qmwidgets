#pragma once

#include "ui_mainwindow.h"
#include <QtWidgets/QWidget>

class QmHardwareInfo;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = Q_NULLPTR);
    ~MainWindow() noexcept override;

private Q_SLOTS:
    //	void on_widget_valueChanged(double value);
    // void on_widget_toggled(bool checked);

private:
    Ui::MainWindowClass ui;
};
