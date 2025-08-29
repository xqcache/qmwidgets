#include "qmqrcodeview.h"
#include <QtGui/QPainter>
#include <iostream>

extern "C" {
#include <qrencode.h>
}

QmQRCodeView::QmQRCodeView(const QString& text, QWidget* parent)
    : QWidget(parent)
{
    setText(text);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    resize(sizeHint());
}

QmQRCodeView::QmQRCodeView(QWidget* parent)
    : QmQRCodeView("", parent)
{
}

QmQRCodeView::~QmQRCodeView()
{
    if (m_qrcode) {
        QRcode_free(static_cast<QRcode*>(m_qrcode));
    }
}

const QString& QmQRCodeView::text() const
{
    return m_text;
}
void QmQRCodeView::setText(const QString& text)
{
    m_text = text;
    if (m_qrcode != nullptr) {
        QRcode_free(static_cast<QRcode*>(m_qrcode));
    }
    m_qrcode = QRcode_encodeData(m_text.size(), reinterpret_cast<const unsigned char*>(m_text.toUtf8().constData()), 0, QR_ECLEVEL_H);
    update();
}

const QColor& QmQRCodeView::lightColor() const
{
    return m_lightColor;
}

void QmQRCodeView::setLightColor(const QColor& color)
{
    m_lightColor = color;
    update();
}

const QColor& QmQRCodeView::darkColor() const
{
    return m_darkColor;
}

void QmQRCodeView::setDarkColor(const QColor& color)
{
    m_darkColor = color;
    update();
}

bool QmQRCodeView::isSquare() const
{
    return m_square;
}

void QmQRCodeView::setSquare(bool square)
{
    m_square = square;
    update();
}

const QPixmap& QmQRCodeView::logo() const
{
    return m_logo;
}

void QmQRCodeView::setLogo(const QPixmap& logo)
{
    m_logo = logo;
    update();
}

qreal QmQRCodeView::logoScaleFactor() const
{
    return m_logeScaleFactor;
}

void QmQRCodeView::setLogoScaleFactor(qreal factor)
{
    if (factor > 0.5) {
        return;
    }
    m_logeScaleFactor = factor;
    update();
}

/**
 * @brief
 *
 * @note qrcode->data编码
 *    MSB 76543210 LSB
       |||||||`- 1=black/0=white
       ||||||`-- 1=ecc/0=data code area
       |||||`--- format information
       ||||`---- version information
       |||`----- timing pattern
       ||`------ alignment pattern
       |`------- finder pattern and separator
       `-------- non-data modules (format, timing, etc.)
 */
void QmQRCodeView::paintEvent(QPaintEvent* event)
{
    if (m_qrcode == nullptr || static_cast<QRcode*>(m_qrcode)->width == 0) {
        return;
    }

    qreal w = 0;
    qreal h = 0;
    qreal dw = 0;
    qreal dh = 0;

    if (m_square) {
        w = h = qMin(width(), height());
    } else {
        w = width();
        h = height();
    }
    // 二维码每个子像素块的宽度
    dw = w / static_cast<QRcode*>(m_qrcode)->width;
    // 二维码每个子像素块的高度
    dh = h / static_cast<QRcode*>(m_qrcode)->width;

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);

    // 亮色
    QPen lightPen(m_lightColor);
    QBrush lightBrush(m_lightColor);
    // 暗色
    QPen darkPen(m_darkColor);
    QBrush darkBrush(m_darkColor);
    for (int i = 0; i < static_cast<QRcode*>(m_qrcode)->width; i++) {
        for (int j = 0; j < static_cast<QRcode*>(m_qrcode)->width; j++) {
            if (static_cast<QRcode*>(m_qrcode)->data[i * static_cast<QRcode*>(m_qrcode)->width + j] & 0x01) {
                // 绘制dark块
                painter.setPen(darkPen);
                painter.setBrush(darkBrush);
                painter.drawRect(QRectF(j * dw, i * dh, dw, dh));
            } else {
                // 绘制light块
                painter.setPen(lightPen);
                painter.setBrush(lightBrush);
                painter.drawRect(QRectF(j * dw, i * dh, dw, dh));
            }
        }
    }

    if (!m_logo.isNull()) {
        // 该参数与二维码的纠错率相关
        constexpr qreal errorCorrectionRate = 0.4;
        // 根据纠错率计算pixmap的缩放比例
        constexpr qreal scaleFactor = errorCorrectionRate * errorCorrectionRate;
        // 生成的二维码宽度
        int qw = dw * static_cast<QRcode*>(m_qrcode)->width;
        // 生成的二维码高度
        int qh = dh * static_cast<QRcode*>(m_qrcode)->width;

        // 缩放后的pixmap宽度
        int pw = w * scaleFactor;
        // 缩放后的pixmap高度
        int ph = h * scaleFactor;
        // 在二维码的中间绘制logo
        painter.drawPixmap((qw - pw) / 2, (qh - ph) / 2, m_logo.scaled(pw, ph));
    }
}

QSize QmQRCodeView::sizeHint() const
{
    // 设置二维码Widget的推荐大小
    return { 100, 100 };
}