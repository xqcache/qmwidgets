#pragma once

#include "qmwidgets_global.h"
#include <QWidget>

struct QmAsyncBlockerWidgetPrivate;

class QMWIDGETS_EXPORT QmAsyncBlockerWidget : public QWidget {
    Q_OBJECT
public:
    explicit QmAsyncBlockerWidget(QWidget* parent = nullptr);
    ~QmAsyncBlockerWidget() noexcept override;

    void setText(const QString& text);
    void setTask(const std::function<void(std::atomic_bool&)>& task);

    QSize sizeHint() const override;

    static void runTask(QWidget* parent, const QString& text, const std::function<void(std::atomic_bool&)>& task);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void initUi();
    void exec();

private:
    QmAsyncBlockerWidgetPrivate* d_ { nullptr };
};