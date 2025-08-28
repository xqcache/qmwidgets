#pragma once

#include <QDialog>
#include "qmwidgets_global.h"

namespace Ui {
	class QmLoginDialog;
}

class QMWIDGETS_EXPORT QmLoginDialog : public QDialog
{
	Q_OBJECT

public:
	QmLoginDialog(QWidget *parent = Q_NULLPTR);
	~QmLoginDialog();

protected:
	void showEvent(QShowEvent* event);
	void closeEvent(QCloseEvent* event);

private:
	Ui::QmLoginDialog *ui;
};
