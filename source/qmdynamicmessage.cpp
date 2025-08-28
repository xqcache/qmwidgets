#include "qmdynamicmessage.h"
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QMainWindow>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QTimerEvent>
#include <QVBoxLayout>

namespace {
constexpr int kRelativetoParentMargin = 50;
constexpr int kAnimationDuration = 200;
} // namespace

void QmDynamicMessage::info(QWidget* parent, const QString& title, const QString& content, int timeout)
{
    QmDynamicMessage* tip = construct(parent, title, content, timeout);

    tip->setStyleSheet(R"(
QWidget {
    background-color: lightskyblue;
    border-radius: 10px;
}
QLabel {
    color: black;
}
)");

    tip->timer_id_ = tip->startTimer(timeout);
    tip->showNormal();
}

void QmDynamicMessage::warn(QWidget* parent, const QString& title, const QString& content, int timeout)
{
    QmDynamicMessage* tip = construct(parent, title, content, timeout);

    tip->setStyleSheet(R"(
QWidget {
    background-color: lightsalmon;
    border-radius: 10px;
}
QLabel {
    color: black;
}
)");
    tip->timer_id_ = tip->startTimer(timeout);
    tip->showNormal();
}

void QmDynamicMessage::error(QWidget* parent, const QString& title, const QString& content, int timeout)
{
    QmDynamicMessage* tip = construct(parent, title, content, timeout);

    tip->setStyleSheet(R"(
QWidget {
    background-color: lightcoral;
    border-radius: 10px;
}
QLabel {
    color: black;
}
)");
    tip->timer_id_ = tip->startTimer(timeout);
    tip->showNormal();
}

QmDynamicMessage::QmDynamicMessage(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_InputMethodTransparent);
    initUi();
}

void QmDynamicMessage::showEvent(QShowEvent* e)
{
    QParallelAnimationGroup* anim_group = new QParallelAnimationGroup(this);
    QPropertyAnimation* size_anim = new QPropertyAnimation(this, "size", this);
    size_anim->setStartValue(QSize(0, 0));
    size_anim->setEndValue(layout()->minimumSize());
    size_anim->setDuration(kAnimationDuration);
    anim_group->addAnimation(size_anim);

    QPropertyAnimation* position_anim = new QPropertyAnimation(this, "pos", this);
    position_anim->setStartValue(animOriginPosition());
    position_anim->setEndValue(animDestinationPosition());
    position_anim->setDuration(kAnimationDuration);
    anim_group->addAnimation(position_anim);

    anim_group->start(QAbstractAnimation::DeleteWhenStopped);
}

void QmDynamicMessage::timerEvent(QTimerEvent* e)
{
    if (e->timerId() != timer_id_) {
        QWidget::timerEvent(e);
        return;
    }
    killTimer(timer_id_);

    QParallelAnimationGroup* anim_group = new QParallelAnimationGroup(this);
    QPropertyAnimation* size_anim = new QPropertyAnimation(this, "size", this);
    size_anim->setStartValue(layout()->minimumSize());
    size_anim->setEndValue(QSize(0, 0));
    size_anim->setDuration(kAnimationDuration);
    anim_group->addAnimation(size_anim);

    QPropertyAnimation* position_anim = new QPropertyAnimation(this, "pos", this);
    position_anim->setStartValue(animDestinationPosition());
    position_anim->setEndValue(animOriginPosition());
    position_anim->setDuration(kAnimationDuration);
    anim_group->addAnimation(position_anim);

    anim_group->start(QAbstractAnimation::DeleteWhenStopped);
    connect(anim_group, &QParallelAnimationGroup::finished, this, &QmDynamicMessage::deleteLater);
}

void QmDynamicMessage::initUi()
{
    lbl_title_ = new QLabel(this);
    lbl_text_ = new QLabel(this);

    QFont f = font();
    f.setPointSizeF(f.pointSizeF() * 1.2);
    f.setBold(true);
    lbl_title_->setFont(f);

    QVBoxLayout* lyt_main = new QVBoxLayout(this);
    lyt_main->addWidget(lbl_title_);
    lyt_main->addWidget(lbl_text_);
    lyt_main->setSpacing(5);

    {
        QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
        effect->setBlurRadius(10);
        effect->setColor(Qt::black);
        effect->setOffset(5, 5);
        setGraphicsEffect(effect);
    }
}

QPoint QmDynamicMessage::animOriginPosition() const
{
    const QSize& parent_size = parentWidget()->size();
    const QSize& self_size = layout()->minimumSize();

    QPoint result(parent_size.width(), parent_size.height() - self_size.height() - kRelativetoParentMargin);
    return result;
}

QPoint QmDynamicMessage::animDestinationPosition() const
{
    const QSize& parent_size = parentWidget()->size();
    const QSize& self_size = layout()->minimumSize();

    QPoint result(parent_size.width() - self_size.width() - kRelativetoParentMargin,
        parent_size.height() - self_size.height() - kRelativetoParentMargin);
    return result;
}

QmDynamicMessage* QmDynamicMessage::construct(QWidget* parent, const QString& title, const QString& content, int timeout)
{
    QmDynamicMessage* tip = new QmDynamicMessage(parent);
    tip->lbl_title_->setText(title);
    tip->lbl_text_->setText(content);

    tip->lbl_title_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    if (tip->lbl_title_->sizeHint().width() > tip->lbl_text_->sizeHint().width()) {
        tip->lbl_text_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    } else {
        tip->lbl_text_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }

    tip->resize(tip->layout()->minimumSize());
    return tip;
}
