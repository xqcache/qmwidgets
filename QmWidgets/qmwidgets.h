#pragma once

#include <QtDesigner>
#include <qplugin.h>

class QmWidgets : public QObject, public QDesignerCustomWidgetCollectionInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)
#if QT_VERSION >= 0x050000
	Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetCollectionInterface")
#endif
public:
	explicit QmWidgets(QObject *parent = Q_NULLPTR);
	virtual QList<QDesignerCustomWidgetInterface *> customWidgets() const;

private:
	QList <QDesignerCustomWidgetInterface *> m_widgets;
};