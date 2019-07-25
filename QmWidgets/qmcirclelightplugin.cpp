#include "stdafx.h"
#include "qmcirclelightplugin.h"
#include "qmcirclelight.h"
#include <QtPlugin>

QmCircleLightPlugin::QmCircleLightPlugin(QObject *parent)
	: QObject(parent)
{
	m_initialized = false;
}

void QmCircleLightPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
	if (m_initialized)
		return;

	// Add extension registrations, etc. here

	m_initialized = true;
}

bool QmCircleLightPlugin::isInitialized() const
{
	return m_initialized;
}

QWidget *QmCircleLightPlugin::createWidget(QWidget *parent)
{
	return new QmCircleLight(parent);
}

QString QmCircleLightPlugin::name() const
{
	return QLatin1String("QmCircleLight");
}

QString QmCircleLightPlugin::group() const
{
	return QLatin1String("QmWidgets");
}

QIcon QmCircleLightPlugin::icon() const
{
	return QIcon();
}

QString QmCircleLightPlugin::toolTip() const
{
	return QStringLiteral("Ô²ÐÎµÆ");
}

QString QmCircleLightPlugin::whatsThis() const
{
	return QLatin1String("");
}

bool QmCircleLightPlugin::isContainer() const
{
	return false;
}

QString QmCircleLightPlugin::domXml() const
{
	return QLatin1String("<widget class=\"QmCircleLight\" name=\"qmCircleLight\">\n"
		"<property name=\"geometry\">\n"
		"	<rect>\n"
		"		<x>0</x>\n"
		"		<y>0</y>\n"
		"		<width>100</width>\n"
		"		<height>100</height>\n"
		"	</rect>\n"
		"</property>\n"
		"</widget>\n");
}

QString QmCircleLightPlugin::includeFile() const
{
	return QLatin1String("qmcirclelight.h");
}
