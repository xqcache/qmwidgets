#include "stdafx.h"
#include "qmlogindialog.h"
#include "ui_qmlogindialog.h"
#include <QPropertyAnimation>


QmLoginDialog::QmLoginDialog(QWidget *parent)
	: QDialog(parent), ui(new Ui::QmLoginDialog)
{
	ui->setupUi(this);
	setWindowFlag(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	
}

QmLoginDialog::~QmLoginDialog()
{
	delete ui;
}

void QmLoginDialog::showEvent(QShowEvent* event)
{
	QPropertyAnimation* enterAnimation = new QPropertyAnimation(this, "pos");
	enterAnimation->setStartValue(QPoint(x(), -height()));
	enterAnimation->setEndValue(QPoint(x(), 0));
	enterAnimation->setEasingCurve(QEasingCurve::OutCubic);
	enterAnimation->setDuration(1000);
	enterAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void QmLoginDialog::closeEvent(QCloseEvent* event)
{
	QPropertyAnimation* enterAnimation = new QPropertyAnimation(this, "pos");
	enterAnimation->setStartValue(QPoint(x(), 0));
	enterAnimation->setEndValue(QPoint(x(), -height()));
	enterAnimation->setEasingCurve(QEasingCurve::InCubic);
	enterAnimation->setDuration(1000);
	enterAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
