#include "stdafx.h"
#include "qmswitch.h"
#include "qmswitchplugin.h"
#include <QtPlugin>

QmSwitchPlugin::QmSwitchPlugin(QObject *parent)
	: QObject(parent)
{
	m_initialized = false;
}

void QmSwitchPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
	if (m_initialized)
		return;

	// Add extension registrations, etc. here

	m_initialized = true;
}

bool QmSwitchPlugin::isInitialized() const
{
	return m_initialized;
}

QWidget *QmSwitchPlugin::createWidget(QWidget *parent)
{
	return new QmSwitch(parent);
}

QString QmSwitchPlugin::name() const
{
	return QLatin1String("QmSwitch");
}

QString QmSwitchPlugin::group() const
{
	return QLatin1String("QmWidgets");
}

QIcon QmSwitchPlugin::icon() const
{
	return QIcon();
}

QString QmSwitchPlugin::toolTip() const
{
	return QStringLiteral("¿ª¹Ø°´Å¥");
}

QString QmSwitchPlugin::whatsThis() const
{
	return QLatin1String("");
}

bool QmSwitchPlugin::isContainer() const
{
	return false;
}

QString QmSwitchPlugin::domXml() const
{
	return QLatin1String("<widget class=\"QmSwitch\" name=\"qmSwitch\">\n"
		"<property name=\"geometry\">\n"
		"	<rect>\n"
		"		<x>0</x>\n"
		"		<y>0</y>\n"
		"		<width>60</width>\n"
		"		<height>30</height>\n"
		"	</rect>\n"
		"</property>\n"
		"</widget>\n");
}

QString QmSwitchPlugin::includeFile() const
{
	return QLatin1String("qmswitch.h");
}

