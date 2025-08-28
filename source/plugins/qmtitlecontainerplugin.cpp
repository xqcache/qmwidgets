#include "qmtitlecontainerplugin.h"
#include "qmtitlecontainer.h"
#include "stdafx.h"
#include <QtPlugin>

QmTitleContainerPlugin::QmTitleContainerPlugin(QObject* parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QmTitleContainerPlugin::initialize(QDesignerFormEditorInterface* /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool QmTitleContainerPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget* QmTitleContainerPlugin::createWidget(QWidget* parent)
{
    return new QmTitleContainer(parent);
}

QString QmTitleContainerPlugin::name() const
{
    return QLatin1String("QmTitleContainer");
}

QString QmTitleContainerPlugin::group() const
{
    return QLatin1String("qmwidgets");
}

QIcon QmTitleContainerPlugin::icon() const
{
    return QIcon();
}

QString QmTitleContainerPlugin::toolTip() const
{
    return tr("Titled Container");
}

QString QmTitleContainerPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool QmTitleContainerPlugin::isContainer() const
{
    return true;
}

QString QmTitleContainerPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QmTitleContainer\" name=\"qmTitleContainer\">\n"
                         "<property name=\"geometry\">\n"
                         "	<rect>\n"
                         "		<x>0</x>\n"
                         "		<y>0</y>\n"
                         "		<width>200</width>\n"
                         "		<height>200</height>\n"
                         "	</rect>\n"
                         "</property>\n"
                         "</widget>\n");
}

QString QmTitleContainerPlugin::includeFile() const
{
    return QLatin1String("qmtitlecontainer.h");
}
