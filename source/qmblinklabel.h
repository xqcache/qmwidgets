#pragma once

#include "qmwidgets_global.h"
#include <QLabel>

struct QmBlinkLabelPrivate;
class QMWIDGETS_EXPORT QmBlinkLabel : public QLabel {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    // Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
public:
    explicit QmBlinkLabel(QWidget* parent = nullptr);
    explicit QmBlinkLabel(const QString& text, QWidget* parent = nullptr);
    ~QmBlinkLabel() noexcept override;

    void setText(const QString& text);
    QString text() const;

    void setNormalColor(const QColor& color);
    QColor normalColor() const;

    void setBlinkColor(const QColor& color);
    QColor blinkColor() const;

    void setBlinkInterval(int ms);
    int blinkInterval() const;

    void startBlink();
    void startBlink(int ms);
    void stopBlink();

private:
    QmBlinkLabelPrivate* d_ { nullptr };
};