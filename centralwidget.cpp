#include <centralwidget.h>
#include <connectivitywidget.h>
#include <discoverymanager.h>
#include <progressbar.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QScroller>

namespace {
const QLatin1String g_buttonStyleSheet(
        "QPushButton {"
        "    color: white;"
        "    border-bottom: 1px solid #40000000;"
        "    border-radius: 3px;"
        "    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
        "                                      stop: 0 #3E474F, stop: 1 #3C454C);"
        "}"
        "QPushButton:pressed {"
        "    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
        "                                      stop: 0 #38434C, stop: 1 #364149);"
        "}"
        );

const QLatin1String g_scrollAreaStyleSheet(
        "QScrollArea {"
        "    border: none;"
        "    background: transparent;"
        "}"
        "QScrollBar {"
        "    border: none;"
        "    margin-left: 2px;"
        "    background: transparent;"
        "}"
        "QScrollBar::handle {"
        "    border: none;"
        "    border-radius: 2px;"
        "    background: #20ffffff;"
        "}"
        "QScrollBar::add-line {"
        "    border: none;"
        "    background: transparent;"
        "}"
        "QScrollBar::sub-line {"
        "    border: none;"
        "    background: transparent;"
        "}"
        );
}

CentralWidget::CentralWidget(QWidget* parent) : QWidget(parent)
  , m_layout(new QVBoxLayout(this))
  , m_connectivityWidget(new ConnectivityWidget(this))
  , m_titleLabel(new QLabel(this))
  , m_versionLabel(new QLabel(this))
  , m_progressBar(new ProgressBar(this))
  , m_scrollAreaLayout(new QHBoxLayout)
  , m_buttonsScrollArea(new QScrollArea(this))
  , m_buttonLayout(new QVBoxLayout(new QWidget(m_buttonsScrollArea)))
  , m_disableDiscoveryButton(new QPushButton(this))
  , m_connectManuallyButton(new QPushButton(this))
  , m_myProjectsButton(new QPushButton(this))
  , m_settingsButton(new QPushButton(this))
  , m_quitButton(new QPushButton(this))
  , m_infoButton(new QPushButton(this))
{
    m_layout->setSpacing(6);
    m_layout->setContentsMargins(12, 12, 12, 12);
    m_layout->addWidget(m_connectivityWidget, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_titleLabel, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_versionLabel, 0, Qt::AlignHCenter);
    m_layout->addStretch();
    m_layout->addWidget(m_progressBar, 0, Qt::AlignHCenter);
    m_layout->addSpacing(6);
    m_layout->addLayout(m_scrollAreaLayout);
    m_layout->addStretch();
    // TODO: m_layout->addWidget(m_infoButton, 0, Qt::AlignHCenter);
    m_infoButton->hide();

    m_scrollAreaLayout->setSpacing(6);
    m_scrollAreaLayout->setContentsMargins(0, 0, 0, 0);
    m_scrollAreaLayout->addStretch();
    m_scrollAreaLayout->addWidget(m_buttonsScrollArea, 2);
    m_scrollAreaLayout->addStretch();

    m_buttonLayout->setSpacing(10);
    m_buttonLayout->setContentsMargins(0, 0, 0, 0);
    m_buttonLayout->addWidget(m_disableDiscoveryButton);
    // TODO: m_buttonLayout->addWidget(m_connectManuallyButton);
    m_connectManuallyButton->hide();
    // TODO: m_buttonLayout->addWidget(m_myProjectsButton);
    m_myProjectsButton->hide();
    // TODO: m_buttonLayout->addWidget(m_settingsButton);
    m_settingsButton->hide();
    m_buttonLayout->addWidget(m_quitButton);

    QScroller::grabGesture(m_buttonsScrollArea, QScroller::LeftMouseButtonGesture);
    m_buttonsScrollArea->setFocusPolicy(Qt::NoFocus);
    m_buttonsScrollArea->viewport()->setFocusPolicy(Qt::NoFocus);
    m_buttonsScrollArea->setAttribute(Qt::WA_MacShowFocusRect, false);
    m_buttonsScrollArea->viewport()->setAttribute(Qt::WA_MacShowFocusRect, false);
    m_buttonsScrollArea->setWidget(m_buttonLayout->parentWidget());
    m_buttonsScrollArea->setMaximumWidth(260);
    m_buttonsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_buttonsScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_buttonsScrollArea->setVerticalScrollBar(new QScrollBar);
    m_buttonsScrollArea->verticalScrollBar()->setFixedWidth(4);
    m_buttonsScrollArea->setWidgetResizable(true);
    m_buttonsScrollArea->setStyleSheet(g_scrollAreaStyleSheet);
    m_buttonLayout->parentWidget()->setAutoFillBackground(false);

    QFont font;
    font.setPixelSize(16);
    m_titleLabel->setFont(font);
    m_titleLabel->setText(tr("Objectwheel Interpreter"));

    font.setPixelSize(13);
    m_versionLabel->setFont(font);
    m_versionLabel->setText("v2.9");

    m_progressBar->hide();

    m_connectivityWidget->setState(ConnectivityWidget::Searching);
    connect(DiscoveryManager::instance(), &DiscoveryManager::connected, this, [=] {
        m_connectivityWidget->setState(ConnectivityWidget::Connected);
    });
    connect(DiscoveryManager::instance(), &DiscoveryManager::disconnected, this, [=] {
        const bool isDisabled = m_disableDiscoveryButton->isChecked();
        if (!isDisabled)
            m_connectivityWidget->setState(ConnectivityWidget::Searching);
    });

    font.setPixelSize(14);
    font.setWeight(QFont::Light);

    m_disableDiscoveryButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_disableDiscoveryButton->setStyleSheet(g_buttonStyleSheet);
    m_disableDiscoveryButton->setFont(font);
    m_disableDiscoveryButton->setCursor(Qt::PointingHandCursor);
    m_disableDiscoveryButton->setFixedHeight(32);
    m_disableDiscoveryButton->setIconSize(QSize(16, 16));
    m_disableDiscoveryButton->setIcon(QIcon(":/images/locked.svg"));
    m_disableDiscoveryButton->setText(tr("Disable Discovery"));
    m_disableDiscoveryButton->setCheckable(true);
    connect(m_disableDiscoveryButton, &QPushButton::clicked, this, [=] {
        const bool isDisabled = m_disableDiscoveryButton->isChecked();
        if (isDisabled) {
            m_connectivityWidget->setState(ConnectivityWidget::Disabled);
            m_disableDiscoveryButton->setIcon(QIcon(":/images/unlocked.svg"));
            m_disableDiscoveryButton->setText(tr("Enable Discovery"));
        } else {
            m_connectivityWidget->setState(ConnectivityWidget::Searching);
            m_disableDiscoveryButton->setIcon(QIcon(":/images/locked.svg"));
            m_disableDiscoveryButton->setText(tr("Disable Discovery"));
        }
        emit disableDiscoveryButtonClicked(isDisabled);
    });

    m_connectManuallyButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_connectManuallyButton->setStyleSheet(g_buttonStyleSheet);
    m_connectManuallyButton->setFont(font);
    m_connectManuallyButton->setCursor(Qt::PointingHandCursor);
    m_connectManuallyButton->setFixedHeight(32);
    m_connectManuallyButton->setIcon(QIcon(":/images/wrench.svg"));
    m_connectManuallyButton->setText(tr("Connect Manually"));
    connect(m_connectManuallyButton, &QPushButton::clicked, this,
            &CentralWidget::connectManuallyButtonClicked);

    m_myProjectsButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_myProjectsButton->setStyleSheet(g_buttonStyleSheet);
    m_myProjectsButton->setFont(font);
    m_myProjectsButton->setCursor(Qt::PointingHandCursor);
    m_myProjectsButton->setFixedHeight(32);
    m_myProjectsButton->setIcon(QIcon(":/images/stack.svg"));
    m_myProjectsButton->setText(tr("My Projects"));
    connect(m_myProjectsButton, &QPushButton::clicked, this,
            &CentralWidget::myProjectsButtonClicked);

    m_settingsButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_settingsButton->setStyleSheet(g_buttonStyleSheet);
    m_settingsButton->setFont(font);
    m_settingsButton->setCursor(Qt::PointingHandCursor);
    m_settingsButton->setFixedHeight(32);
    m_settingsButton->setIcon(QIcon(":/images/settings.svg"));
    m_settingsButton->setText(tr("Settings"));
    connect(m_settingsButton, &QPushButton::clicked, this,
            &CentralWidget::settingsButtonClicked);

    m_quitButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_quitButton->setStyleSheet(g_buttonStyleSheet);
    m_quitButton->setFont(font);
    m_quitButton->setCursor(Qt::PointingHandCursor);
    m_quitButton->setFixedHeight(32);
    m_quitButton->setIcon(QIcon(":/images/quit.svg"));
    m_quitButton->setText(tr("Quit"));
    connect(m_quitButton, &QPushButton::clicked, this,
            &CentralWidget::quitButtonClicked);

    m_infoButton->setStyleSheet("border: node;");
    m_infoButton->setCursor(Qt::PointingHandCursor);
    m_infoButton->setFixedSize(20, 20);
    m_infoButton->setIconSize(QSize(16, 16));
    m_infoButton->setIcon(QIcon(":/images/info.svg"));
    connect(m_infoButton, &QPushButton::clicked,
            this, &CentralWidget::infoButtonClicked);
}

ProgressBar* CentralWidget::progressBar() const
{
    return m_progressBar;
}