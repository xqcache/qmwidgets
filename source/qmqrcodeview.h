#ifndef __QMQRCODEVIEW_H__
#define __QMQRCODEVIEW_H__

#include "qmwidgets_global.h"
#include <QtWidgets/QWidget>

class QMWIDGETS_EXPORT QmQRCodeView : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QColor lightColor READ lightColor WRITE setLightColor)
    Q_PROPERTY(QColor darkColor READ darkColor WRITE setDarkColor)
    Q_PROPERTY(bool square READ isSquare WRITE setSquare)
    Q_PROPERTY(QPixmap logo READ logo WRITE setLogo)
    Q_PROPERTY(qreal logeScaleFactor READ logoScaleFactor WRITE setLogoScaleFactor)
public:
    explicit QmQRCodeView(QWidget* parent = nullptr);
    QmQRCodeView(const QString& text, QWidget* parent = nullptr);
    ~QmQRCodeView() override;

    const QString& text() const;
    void setText(const QString& text);

    const QColor& lightColor() const;
    void setLightColor(const QColor& color);

    const QColor& darkColor() const;
    void setDarkColor(const QColor& color);

    bool isSquare() const;
    void setSquare(bool square);

    const QPixmap& logo() const;
    void setLogo(const QPixmap& logo);

    QSize sizeHint() const override;

    qreal logoScaleFactor() const;
    void setLogoScaleFactor(qreal factor);

signals:
    void encodeFinished();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString m_text;
    void* m_qrcode = nullptr;
    QColor m_lightColor = Qt::white;
    QColor m_darkColor = Qt::black;
    QPixmap m_logo;
    qreal m_logeScaleFactor = 0.4;
    // 是否绘制为正方形
    bool m_square = false;
};

#endif // __QMQRCODEVIEW_H__
