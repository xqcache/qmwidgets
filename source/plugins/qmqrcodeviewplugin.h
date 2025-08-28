#ifndef __QRCODEVIEW_PLUGIN_H__
#define __QRCODEVIEW_PLUGIN_H__

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class QmQRCodeViewPlugin : public QObject, virtual public QDesignerCustomWidgetInterface {
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    explicit QmQRCodeViewPlugin(QObject* parent = nullptr);
    ~QmQRCodeViewPlugin() override = default;

    QString name() const override;
    QString group() const override;
    QString toolTip() const override;
    QString whatsThis() const override;
    QString includeFile() const override;
    QIcon icon() const override;
    bool isContainer() const override;
    QString domXml() const override;
    QWidget* createWidget(QWidget* parent) override;

    bool isInitialized() const override;
    void initialize(QDesignerFormEditorInterface* core) override;

private:
    bool m_initialized = false;
};

#endif // __QRCODEVIEW_PLUGIN_H__
