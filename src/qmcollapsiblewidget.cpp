#include "qmcollapsiblewidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QSpacerItem>
#include <QToolButton>
#include <QVBoxLayout>

struct QmCollapsibleWidgetPrivate {
    QLabel* lbl_title { nullptr };
    QToolButton* btn_fold { nullptr };
    QWidget* widget { nullptr };
};

QmCollapsibleWidget::QmCollapsibleWidget(QWidget* parent)
    : QWidget(parent)
    , d_(new QmCollapsibleWidgetPrivate)
{
    initUi();
    setupSignals();
}

QmCollapsibleWidget::~QmCollapsibleWidget() noexcept
{
    delete d_;
}

void QmCollapsibleWidget::setWidget(QWidget* widget)
{
    if (d_->widget == widget) {
        return;
    }
    widget->setParent(this);
    layout()->replaceWidget(d_->widget, widget);
    if (d_->widget) {
        d_->widget->deleteLater();
    }
    d_->widget = widget;
}

bool QmCollapsibleWidget::event(QEvent* event)
{
    switch (event->type()) {
    case QEvent::WindowTitleChange:
        d_->lbl_title->setText(windowTitle());
        break;
    default:
        break;
    }
    return QWidget::event(event);
}

void QmCollapsibleWidget::initUi()
{
    d_->lbl_title = new QLabel(tr("Unnamed"), this);

    d_->btn_fold = new QToolButton(this);
    d_->widget = new QWidget(this);
    d_->widget->setProperty("Style", "Container");

    // QIcon icon;
    // icon.addFile(QString::fromUtf8(":/qmwidgets/assets/icons/collapse.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
    // icon.addFile(QString::fromUtf8(":/qmwidgets/assets/icons/expand.svg"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
    // d_->btn_fold->setIcon(icon);
    d_->btn_fold->setCheckable(true);
    d_->btn_fold->setChecked(true);

    d_->lbl_title->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    d_->btn_fold->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    d_->widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    QWidget* wgt_top = new QWidget(this);
    wgt_top->setAttribute(Qt::WA_StyledBackground);
    wgt_top->setProperty("Style", "Title");
    wgt_top->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    QHBoxLayout* lyt_top = new QHBoxLayout(wgt_top);
    lyt_top->addWidget(d_->lbl_title);
    lyt_top->addWidget(d_->btn_fold);

    QVBoxLayout* lyt_main = new QVBoxLayout(this);
    lyt_main->addWidget(wgt_top);
    lyt_main->addWidget(d_->widget);
    lyt_main->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Preferred, QSizePolicy::Expanding));

    lyt_main->setContentsMargins(0, 0, 0, 0);
    lyt_main->setSpacing(0);
    lyt_main->setStretch(1, 1);
}

void QmCollapsibleWidget::setupSignals()
{
    connect(d_->btn_fold, &QToolButton::toggled, this, &QmCollapsibleWidget::onCollapsibleChanged);
}

void QmCollapsibleWidget::collapse()
{
    QPropertyAnimation* anim = new QPropertyAnimation(d_->widget, "maximumHeight", this);
    d_->widget->setProperty("heightBackup", d_->widget->height());
    anim->setStartValue(d_->widget->height());
    anim->setEndValue(0);
    anim->setDuration(300);
    connect(anim, &QPropertyAnimation::finished, anim, &QObject::deleteLater);
    anim->start();
}

void QmCollapsibleWidget::expand()
{
    QPropertyAnimation* anim = new QPropertyAnimation(d_->widget, "maximumHeight", this);
    int height = d_->widget->property("heightBackup").toInt();
    anim->setStartValue(0);
    anim->setEndValue(height);
    anim->setDuration(300);
    connect(anim, &QPropertyAnimation::finished, anim, [anim, height, this] {
        d_->widget->setMaximumHeight(d_->widget->maximumWidth());
        anim->deleteLater();
    });
    anim->start();
}

void QmCollapsibleWidget::onCollapsibleChanged(bool collapse)
{
    if (collapse) {
        expand();
    } else {
        this->collapse();
    }
}