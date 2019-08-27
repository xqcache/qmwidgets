#include "stdafx.h"
#include "qmimageanalogclockplugin.h"
#include "qmimageanalogclock.h"


QmImageAnalogClockPlugin::QmImageAnalogClockPlugin(QObject* parent)
	: QObject(parent)
{
	m_initialized = false;
}

void QmImageAnalogClockPlugin::initialize(QDesignerFormEditorInterface* /* core */)
{
	if (m_initialized)
		return;

	// Add extension registrations, etc. here

	m_initialized = true;
}

bool QmImageAnalogClockPlugin::isInitialized() const
{
	return m_initialized;
}

QWidget* QmImageAnalogClockPlugin::createWidget(QWidget* parent)
{
	return new QmImageAnalogClock(parent);
}

QString QmImageAnalogClockPlugin::name() const
{
	return QLatin1String("QmImageAnalogClock");
}

QString QmImageAnalogClockPlugin::group() const
{
	return QLatin1String("QmWidgets");
}

QIcon QmImageAnalogClockPlugin::icon() const
{
	return QIcon();
}

QString QmImageAnalogClockPlugin::toolTip() const
{
	return QStringLiteral("¿ª¹Ø°´Å¥");
}

QString QmImageAnalogClockPlugin::whatsThis() const
{
	return QLatin1String("");
}

bool QmImageAnalogClockPlugin::isContainer() const
{
	return false;
}

QString QmImageAnalogClockPlugin::domXml() const
{
	return QLatin1String("<widget class=\"QmImageAnalogClock\" name=\"qmImageAnalogClock\">\n"
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

QString QmImageAnalogClockPlugin::includeFile() const
{
	return QLatin1String("qmimageanalogclock.h");
}
