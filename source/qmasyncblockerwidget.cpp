#include "qmasyncblockerwidget.h"
#include "qmframelesswindow.h"
#include <QFile>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMovie>
#include <QPainter>
#include <QResizeEvent>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <future>

int qInitResources_qmassets();
int qCleanupResources_qmassets();

namespace {

constexpr std::string_view kWaitGifFilePath = ":/qmwidgets/images/wait.gif";

}

struct QmAsyncBlockerWidgetPrivate {
    QLabel* lbl_text { nullptr };
    QLabel* lbl_movie { nullptr };
    QMovie* movie { nullptr };
    std::function<void(std::atomic_bool&)> task;
    std::atomic_bool stop_requested { false };
    bool is_initialized_res = false;
};

QmAsyncBlockerWidget::QmAsyncBlockerWidget(QWidget* parent)
    : QWidget(parent)
    , d_(new QmAsyncBlockerWidgetPrivate)
{
    if (!QFile::exists(kWaitGifFilePath.data())) {
        qInitResources_qmassets();
        d_->is_initialized_res = true;
    }
    setAttribute(Qt::WA_StyledBackground, true);

    initUi();
}

QmAsyncBlockerWidget::~QmAsyncBlockerWidget() noexcept
{
    if (d_->is_initialized_res) {
        qCleanupResources_qmassets();
    }
    delete d_;
}

void QmAsyncBlockerWidget::initUi()
{
    setAttribute(Qt::WA_StyledBackground);
    d_->movie = new QMovie(kWaitGifFilePath.data(), "GIF", this);

    d_->lbl_text = new QLabel(this);
    d_->lbl_movie = new QLabel(this);
    d_->lbl_text->setText("Please wait...");

    d_->lbl_text->setAlignment(Qt::AlignCenter);
    d_->lbl_movie->setMinimumSize(100, 100);

    auto* lyt_horizontal = new QVBoxLayout();
    lyt_horizontal->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Preferred, QSizePolicy::Expanding));
    lyt_horizontal->addWidget(d_->lbl_text);
    lyt_horizontal->addWidget(d_->lbl_movie);
    lyt_horizontal->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Preferred, QSizePolicy::Expanding));

    auto* lyt_main = new QHBoxLayout(this);
    lyt_main->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred));
    lyt_main->addLayout(lyt_horizontal);
    lyt_main->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred));

    connect(d_->movie, &QMovie::frameChanged, d_->lbl_movie, [this](int frame_no) {
        auto pixmap = d_->movie->currentPixmap();
        d_->lbl_movie->setPixmap(pixmap.scaled(d_->lbl_movie->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    });
}

QSize QmAsyncBlockerWidget::sizeHint() const
{
    return layout()->sizeHint();
}

void QmAsyncBlockerWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->modifiers() == Qt::NoModifier && event->key() == Qt::Key_Escape) {
        d_->stop_requested.store(true, std::memory_order_relaxed);
    }
}

void QmAsyncBlockerWidget::exec()
{
    if (d_->task) {
        d_->movie->start();
        show();
        raise();
        activateWindow();
        setFocusPolicy(Qt::StrongFocus);
        setFocus(Qt::ActiveWindowFocusReason);
        QEventLoop loop;
        auto future = std::async(std::launch::async, [&loop, this] {
            d_->task(d_->stop_requested);
            QMetaObject::invokeMethod(&loop, "quit", Qt::QueuedConnection);
        });
        loop.exec();
        close();
    }
}

void QmAsyncBlockerWidget::setText(const QString& text)
{
    d_->lbl_text->setText(text);
}

void QmAsyncBlockerWidget::setTask(const std::function<void(std::atomic_bool&)>& task)
{
    d_->task = task;
}

bool QmAsyncBlockerWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::Resize) {
        resize(static_cast<QResizeEvent*>(event)->size());
    }
    return QWidget::eventFilter(watched, event);
}

void QmAsyncBlockerWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    initPainter(&painter);
    painter.drawRect(rect());
}

void QmAsyncBlockerWidget::runTask(QWidget* parent, const QString& text, const std::function<void(std::atomic_bool&)>& task)
{
    QmAsyncBlockerWidget* view = new QmAsyncBlockerWidget(parent);
    view->setGeometry(parent->contentsRect());
    view->setAttribute(Qt::WA_DeleteOnClose);
    view->setText(text + "\n" + tr("Press ESC to abort!"));
    view->setTask(task);
    parent->installEventFilter(view);
    view->exec();
}