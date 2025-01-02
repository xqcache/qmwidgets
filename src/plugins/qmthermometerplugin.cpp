#include "qmthermometerplugin.h"
#include "qmthermometer.h"
#include "stdafx.h"
#include <QtPlugin>

QmThermometerPlugin::QmThermometerPlugin(QObject* parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QmThermometerPlugin::initialize(QDesignerFormEditorInterface* /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool QmThermometerPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget* QmThermometerPlugin::createWidget(QWidget* parent)
{
    return new QmThermometer(parent);
}

QString QmThermometerPlugin::name() const
{
    return QLatin1String("QmThermometer");
}

QString QmThermometerPlugin::group() const
{
    return QLatin1String("qmwidgets");
}

QIcon QmThermometerPlugin::icon() const
{
    return QIcon();
}

QString QmThermometerPlugin::toolTip() const
{
    return tr("Thermonmeter");
}

QString QmThermometerPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QmThermometerPlugin::isContainer() const
{
    return false;
}

QString QmThermometerPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QmThermometer\" name=\"qmThermometer\">\n"
                         "<property name=\"geometry\">\n"
                         "	<rect>\n"
                         "		<x>0</x>\n"
                         "		<y>0</y>\n"
                         "		<width>100</width>\n"
                         "		<height>250</height>\n"
                         "	</rect>\n"
                         "</property>\n"
                         "</widget>\n");
}

QString QmThermometerPlugin::includeFile() const
{
    return QLatin1String("qmthermometer.h");
}
