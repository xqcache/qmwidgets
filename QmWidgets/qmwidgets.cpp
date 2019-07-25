#include "stdafx.h"
#include "qmwidgets.h"
#include "qmgaugecarplugin.h"
#include "qmswitchplugin.h"
#include "qmthermometerplugin.h"
#include "qmanalogclockplugin.h"
#include "qmcirclelightplugin.h"
#include "qmimageanalogclockplugin.h"


QmWidgets::QmWidgets(QObject *parent /*= Q_NULLPTR*/)
{
	m_widgets.append(new QmGaugeCarPlugin(this));
	m_widgets.append(new QmSwitchPlugin(this));
	m_widgets.append(new QmThermometerPlugin(this));
	m_widgets.append(new QmAnalogClockPlugin(this));
	m_widgets.append(new QmCircleLightPlugin(this));
	m_widgets.append(new QmImageAnalogClockPlugin(this));
}

QList<QDesignerCustomWidgetInterface *> QmWidgets::customWidgets() const
{
	return m_widgets;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qmwidgetsplugin, QmWidgets)
#endif
