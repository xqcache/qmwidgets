#include "qmqrcodeviewplugin.h"
#include "qmqrcodeview.h"

#include <QtPlugin>

QmQRCodeViewPlugin::QmQRCodeViewPlugin(QObject* parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QmQRCodeViewPlugin::initialize(QDesignerFormEditorInterface* /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool QmQRCodeViewPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget* QmQRCodeViewPlugin::createWidget(QWidget* parent)
{
    return new QmQRCodeView(parent);
}

QString QmQRCodeViewPlugin::name() const
{
    return QLatin1String("QRcodeView");
}

QString QmQRCodeViewPlugin::group() const
{
    return QLatin1String("My Widgets");
}

QIcon QmQRCodeViewPlugin::icon() const
{
    return QIcon();
}

QString QmQRCodeViewPlugin::toolTip() const
{
    return QLatin1String("");
}

QString QmQRCodeViewPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QmQRCodeViewPlugin::isContainer() const
{
    return false;
}

QString QmQRCodeViewPlugin::domXml() const
{
    return QDesignerCustomWidgetInterface::domXml();
}

QString QmQRCodeViewPlugin::includeFile() const
{
    return QLatin1String("qwebview.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qmqrcodeviewplugin, QmQRCodeViewPlugin)
#endif // QT_VERSION < 0x050000
