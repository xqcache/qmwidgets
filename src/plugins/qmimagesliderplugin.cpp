#include "qmimagesliderplugin.h"
#include "qmimageslider.h"
#include "stdafx.h"
#include <QtPlugin>

QmImageSliderPlugin::QmImageSliderPlugin(QObject* parent)
    : QObject(parent)
{
    m_initialized = false;
}

void QmImageSliderPlugin::initialize(QDesignerFormEditorInterface* /* core */)
{
    if (m_initialized)
        return;

    m_initialized = true;
}

bool QmImageSliderPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget* QmImageSliderPlugin::createWidget(QWidget* parent)
{
    return new QmImageSlider(parent);
}

QString QmImageSliderPlugin::name() const
{
    return QLatin1String("QmImageSlider");
}

QString QmImageSliderPlugin::group() const
{
    return QLatin1String("qmwidgets");
}

QIcon QmImageSliderPlugin::icon() const
{
    return QIcon();
}

QString QmImageSliderPlugin::toolTip() const
{
    return tr("Image Slider");
}

QString QmImageSliderPlugin::whatsThis() const
{
    return tr("Image Slider");
}

bool QmImageSliderPlugin::isContainer() const
{
    return false;
}

QString QmImageSliderPlugin::domXml() const
{
    return R"(<widget class="QmImageSlider" name="slider">
<property name="geometry">
	<rect>
		<x>0</x>
		<y>0</y>
		<width>98</width>
		<height>330</height>
	</rect>
</property>
</widget>
)";
}

QString QmImageSliderPlugin::includeFile() const
{
    return QLatin1String("qmimageslider.h");
}
