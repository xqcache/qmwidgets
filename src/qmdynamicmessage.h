#pragma once

#include <QWidget>

class QLabel;
class QmDynamicMessage : public QWidget {
public:
    static void info(QWidget* parent, const QString& title, const QString& content, int timeout = 4000);
    static void warn(QWidget* parent, const QString& title, const QString& content, int timeout = 4000);
    static void error(QWidget* parent, const QString& title, const QString& content, int timeout = 4000);

    QmDynamicMessage(QWidget* parent);

protected:
    void showEvent(QShowEvent* e) override;
    void timerEvent(QTimerEvent* e) override;

private:
    void initUi();
    QPoint animOriginPosition() const;
    QPoint animDestinationPosition() const;

    static QmDynamicMessage* construct(QWidget* parent, const QString& title, const QString& content, int timeout = 4000);

private:
    QLabel* lbl_title_ { nullptr };
    QLabel* lbl_text_ { nullptr };
    int timer_id_ { -1 };
};