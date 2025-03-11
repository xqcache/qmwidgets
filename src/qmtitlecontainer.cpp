#include "qmtitlecontainer.h"
#include <QVBoxLayout>

struct QmTitleContainerPrivate {
    QWidget* title_widget { nullptr };
    QWidget* widget { nullptr };
    QVBoxLayout* layout { nullptr };

    void initUi(QmTitleContainer* continer)
    {
        layout = new QVBoxLayout(continer);
        widget = new QWidget(continer);
        widget->setProperty("Style", "Filler");

        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);

        auto* label = new QLabel(QmTitleContainer::tr("Unnamed"), continer);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        label->setAlignment(Qt::AlignCenter);

        title_widget = label;
        layout->addWidget(title_widget);
        layout->addWidget(widget);
    }
};

QmTitleContainer::QmTitleContainer(QWidget* parent)
    : QFrame(parent)
    , d_(new QmTitleContainerPrivate)
{
    d_->initUi(this);
}

QmTitleContainer::~QmTitleContainer() noexcept
{
    delete d_;
}

void QmTitleContainer::setTitle(const QString& text)
{
    if (auto* lbl_title = qobject_cast<QLabel*>(d_->title_widget); lbl_title) {
        lbl_title->setText(text);
    }
}

void QmTitleContainer::setTitleWidget(QWidget* widget)
{
    if (!widget) {
        return;
    }
    d_->layout->removeWidget(d_->title_widget);
    if (!d_->title_widget) {
        d_->title_widget->deleteLater();
    }

    d_->title_widget = widget;
    d_->title_widget->setParent(this);
    d_->layout->insertWidget(0, d_->title_widget);
}

void QmTitleContainer::setWidget(QWidget* widget)
{
    if (!widget) {
        return;
    }
    d_->widget = widget;
    d_->widget->setParent(this);
    d_->layout->removeWidget(d_->widget);
    d_->layout->addWidget(widget);
}
