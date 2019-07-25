#include "stdafx.h"
#include "qmanalogclockplugin.h"
#include "qmanalogclock.h"
#include <QtPlugin>

QmAnalogClockPlugin::QmAnalogClockPlugin(QObject *parent)
	: QObject(parent)
{
	m_initialized = false;
}

void QmAnalogClockPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
	if (m_initialized)
		return;

	// Add extension registrations, etc. here

	m_initialized = true;
}

bool QmAnalogClockPlugin::isInitialized() const
{
	return m_initialized;
}

QWidget *QmAnalogClockPlugin::createWidget(QWidget *parent)
{
	return new QmAnalogClock(parent);
}

QString QmAnalogClockPlugin::name() const
{
	return QLatin1String("QmAnalogClock");
}

QString QmAnalogClockPlugin::group() const
{
	return QLatin1String("QmWidgets");
}

QIcon QmAnalogClockPlugin::icon() const
{
	return QIcon();
}

QString QmAnalogClockPlugin::toolTip() const
{
	return QStringLiteral("¿ª¹Ø°´Å¥");
}

QString QmAnalogClockPlugin::whatsThis() const
{
	return QLatin1String("");
}

bool QmAnalogClockPlugin::isContainer() const
{
	return false;
}

QString QmAnalogClockPlugin::domXml() const
{
	return QLatin1String("<widget class=\"QmAnalogClock\" name=\"qmAnalogClock\">\n"
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

QString QmAnalogClockPlugin::includeFile() const
{
	return QLatin1String("qmanalogclock.h");
}
