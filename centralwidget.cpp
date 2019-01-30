#include <centralwidget.h>
#include <connectivitywidget.h>
#include <discoverymanager.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

namespace {
const QLatin1String g_styleSheet(
        "QPushButton {"
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
}

CentralWidget::CentralWidget(QWidget* parent) : QWidget(parent)
  , m_layout(new QVBoxLayout(this))
  , m_buttonLayout(new QVBoxLayout)
  , m_connectivityWidget(new ConnectivityWidget(this))
  , m_titleLabel(new QLabel(this))
  , m_versionLabel(new QLabel(this))
  , m_disableDiscoveryButton(new QPushButton(this))
  , m_connectManuallyButton(new QPushButton(this))
  , m_myProjectsButton(new QPushButton(this))
  , m_settingsButton(new QPushButton(this))
  , m_infoButton(new QPushButton(this))
{
    m_layout->setContentsMargins(12, 12, 12, 12);
    m_layout->setSpacing(6);
    m_layout->addWidget(m_connectivityWidget, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_titleLabel, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_versionLabel, 0, Qt::AlignHCenter);
    m_layout->addStretch();
    m_layout->addLayout(m_buttonLayout);
    m_layout->addStretch();
    m_layout->addWidget(m_infoButton, 0, Qt::AlignHCenter);

    m_buttonLayout->setContentsMargins(0, 0, 0, 0);
    m_buttonLayout->setSpacing(12);
    m_buttonLayout->addWidget(m_disableDiscoveryButton, 0, Qt::AlignHCenter);
    m_buttonLayout->addWidget(m_connectManuallyButton, 0, Qt::AlignHCenter);
    m_buttonLayout->addWidget(m_myProjectsButton, 0, Qt::AlignHCenter);
    m_buttonLayout->addWidget(m_settingsButton, 0, Qt::AlignHCenter);

    QFont font;
    font.setPixelSize(16);
    font.setWeight(QFont::Normal);
    m_titleLabel->setFont(font);
    m_titleLabel->setText(tr("Objectwheel Interpreter"));

    font.setPixelSize(13);
    m_versionLabel->setFont(font);
    m_versionLabel->setText("v1.2");

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

    m_disableDiscoveryButton->setStyleSheet(g_styleSheet);
    m_disableDiscoveryButton->setFont(font);
    m_disableDiscoveryButton->setCursor(Qt::PointingHandCursor);
    m_disableDiscoveryButton->setFixedSize(260, 32);
    m_disableDiscoveryButton->setIconSize(QSize(16, 16));
    m_disableDiscoveryButton->setIcon(QIcon(":/images/locked.svg"));
    m_disableDiscoveryButton->setText(tr("Disable Discovery"));
    m_disableDiscoveryButton->setCheckable(true);
    connect(m_disableDiscoveryButton, &QPushButton::clicked, this,
            &CentralWidget::onDisableDiscoveryButtonClicked);

    m_connectManuallyButton->setStyleSheet(g_styleSheet);
    m_connectManuallyButton->setFont(font);
    m_connectManuallyButton->setCursor(Qt::PointingHandCursor);
    m_connectManuallyButton->setFixedSize(260, 32);
    m_connectManuallyButton->setIconSize(QSize(16, 16));
    m_connectManuallyButton->setIcon(QIcon(":/images/wrench.svg"));
    m_connectManuallyButton->setText(tr("Connect Manually"));
    connect(m_connectManuallyButton, &QPushButton::clicked, this,
            &CentralWidget::connectManuallyButtonClicked);

    m_myProjectsButton->setStyleSheet(g_styleSheet);
    m_myProjectsButton->setFont(font);
    m_myProjectsButton->setCursor(Qt::PointingHandCursor);
    m_myProjectsButton->setFixedSize(260, 32);
    m_myProjectsButton->setIconSize(QSize(16, 16));
    m_myProjectsButton->setIcon(QIcon(":/images/stack.svg"));
    m_myProjectsButton->setText(tr("My Projects"));
    connect(m_myProjectsButton, &QPushButton::clicked, this,
            &CentralWidget::myProjectsButtonClicked);

    m_settingsButton->setStyleSheet(g_styleSheet);
    m_settingsButton->setFont(font);
    m_settingsButton->setCursor(Qt::PointingHandCursor);
    m_settingsButton->setFixedSize(260, 32);
    m_settingsButton->setIconSize(QSize(16, 16));
    m_settingsButton->setIcon(QIcon(":/images/settings.svg"));
    m_settingsButton->setText(tr("Settings"));
    connect(m_settingsButton, &QPushButton::clicked, this,
            &CentralWidget::settingsButtonClicked);

    m_infoButton->setStyleSheet("border: node;");
    m_infoButton->setCursor(Qt::PointingHandCursor);
    m_infoButton->setFixedSize(20, 20);
    m_infoButton->setIconSize(QSize(16, 16));
    m_infoButton->setIcon(QIcon(":/images/info.svg"));
    connect(m_infoButton, &QPushButton::clicked,
            this, &CentralWidget::infoButtonClicked);
}

void CentralWidget::onDisableDiscoveryButtonClicked()
{
    const bool isDisabled = m_disableDiscoveryButton->isChecked();

    DiscoveryManager::setDisabled(isDisabled);

    if (isDisabled) {
        m_connectivityWidget->setState(ConnectivityWidget::Disabled);
        m_disableDiscoveryButton->setIcon(QIcon(":/images/unlocked.svg"));
        m_disableDiscoveryButton->setText(tr("Enable Discovery"));
    } else {
        m_connectivityWidget->setState(ConnectivityWidget::Searching);
        m_disableDiscoveryButton->setIcon(QIcon(":/images/locked.svg"));
        m_disableDiscoveryButton->setText(tr("Disable Discovery"));
    }
}
