#include "stdafx.h"
#include "qmgaugecar.h"
#include "qmgaugecarplugin.h"

#include <QtPlugin>

QmGaugeCarPlugin::QmGaugeCarPlugin(QObject *parent)
	: QObject(parent)
{
	m_initialized = false;
}

void QmGaugeCarPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
	if (m_initialized)
		return;

	// Add extension registrations, etc. here

	m_initialized = true;
}

bool QmGaugeCarPlugin::isInitialized() const
{
	return m_initialized;
}

QWidget *QmGaugeCarPlugin::createWidget(QWidget *parent)
{
	return new QmGaugeCar(parent);
}

QString QmGaugeCarPlugin::name() const
{
	return QLatin1String("QmGaugeCar");
}

QString QmGaugeCarPlugin::group() const
{
	return QLatin1String("QmWidgets");
}

QIcon QmGaugeCarPlugin::icon() const
{
	return QIcon();
}

QString QmGaugeCarPlugin::toolTip() const
{
	return QStringLiteral("Æû³µÒÇ±íÅÌ");
}

QString QmGaugeCarPlugin::whatsThis() const
{
	return QLatin1String("");
}

bool QmGaugeCarPlugin::isContainer() const
{
	return false;
}

QString QmGaugeCarPlugin::domXml() const
{
	return QLatin1String("<widget class=\"QmGaugeCar\" name=\"qmGaugeCar\">\n"
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

QString QmGaugeCarPlugin::includeFile() const
{
	return QLatin1String("qmgaugecar.h");
}

