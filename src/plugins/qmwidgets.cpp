#include "qmwidgets.h"
#include "qmanalogclockplugin.h"
#include "qmcirclelightplugin.h"
#include "qmgaugecarplugin.h"
#include "qmimageanalogclockplugin.h"
#include "qmimagesliderplugin.h"
#include "qmswitchplugin.h"
#include "qmthermometerplugin.h"
#include "stdafx.h"

QmWidgets::QmWidgets(QObject* parent /*= Q_NULLPTR*/)
{
    m_widgets.append(new QmGaugeCarPlugin(this));
    m_widgets.append(new QmSwitchPlugin(this));
    m_widgets.append(new QmThermometerPlugin(this));
    m_widgets.append(new QmAnalogClockPlugin(this));
    m_widgets.append(new QmCircleLightPlugin(this));
    m_widgets.append(new QmImageAnalogClockPlugin(this));
    m_widgets.append(new QmImageSliderPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> QmWidgets::customWidgets() const
{
    return m_widgets;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qmwidgetsplugin, QmWidgets)
#endif
