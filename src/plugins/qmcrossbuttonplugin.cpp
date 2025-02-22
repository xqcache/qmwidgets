#include "qmcrossbuttonplugin.h"
#include "qmcrossbutton.h"
#include "stdafx.h"
#include <QtPlugin>

QmCrossButtonPlugin::QmCrossButtonPlugin(QObject* parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QmCrossButtonPlugin::initialize(QDesignerFormEditorInterface* /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool QmCrossButtonPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget* QmCrossButtonPlugin::createWidget(QWidget* parent)
{
    return new QmCrossButton(parent);
}

QString QmCrossButtonPlugin::name() const
{
    return QLatin1String("QmCrossButton");
}

QString QmCrossButtonPlugin::group() const
{
    return QLatin1String("qmwidgets");
}

QIcon QmCrossButtonPlugin::icon() const
{
    return QIcon();
}

QString QmCrossButtonPlugin::toolTip() const
{
    return tr("Thermonmeter");
}

QString QmCrossButtonPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QmCrossButtonPlugin::isContainer() const
{
    return false;
}

QString QmCrossButtonPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QmCrossButton\" name=\"qmCrossButton\">\n"
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

QString QmCrossButtonPlugin::includeFile() const
{
    return QLatin1String("qmcrossbutton.h");
}
