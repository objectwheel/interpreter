#include <centralwidget.h>
#include <connectivitywidget.h>
#include <discoverymanager.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

CentralWidget::CentralWidget(QWidget* parent) : QWidget(parent)
  , m_layout(new QVBoxLayout(this))
  , m_buttonLayout(new QVBoxLayout)
  , m_connectivityWidget(new ConnectivityWidget(this))
  , m_titleLabel(new QLabel(this))
{
    m_layout->setContentsMargins(8, 8, 8, 8);
    m_layout->setSpacing(0);
    m_layout->addWidget(m_connectivityWidget, 0, Qt::AlignHCenter);
    m_layout->addSpacing(15);
    m_layout->addWidget(m_titleLabel, 0, Qt::AlignHCenter);
    m_layout->addLayout(m_buttonLayout);
    m_layout->addStretch(3);

    QFont font;
    font.setPixelSize(16);
    font.setWeight(QFont::Normal);
    m_titleLabel->setStyleSheet("color: #4BA086");
    m_titleLabel->setFont(font);
    m_titleLabel->setText(tr("Objectwheel Interpreter"));

    m_connectivityWidget->setState(ConnectivityWidget::Searching);
    connect(DiscoveryManager::instance(), &DiscoveryManager::connected, this, [=] {
        m_connectivityWidget->setState(ConnectivityWidget::Connected);
    });
    connect(DiscoveryManager::instance(), &DiscoveryManager::disconnected, this, [=] {
        m_connectivityWidget->setState(ConnectivityWidget::Searching);
    });


    font.setPixelSize(14);
    font.setWeight(QFont::Light);

    static const QLatin1String styleSheet(
                "QPushButton {"
                "    border-bottom: 1px solid #30000000;"
                "    border-radius: 4px;"
                "    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "                                      stop: 0 #c03076A5, stop: 1 #a03076A5);"
                "}"

                "QPushButton:pressed {"
                "    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                "                                      stop: 0 #a03076A5, stop: 1 #c03076A5);"
                "}"
                );

    auto btnNext = new QPushButton(this);
    btnNext->setStyleSheet(styleSheet);
    btnNext->setFont(font);
    btnNext->setCursor(Qt::PointingHandCursor);
    btnNext->setFixedSize(260, 32);
    btnNext->setIconSize(QSize(16, 16));
    btnNext->setIcon(QIcon(":/images/locked.svg"));
    btnNext->setText("Disable Discoverability");
//    connect(btnNext, &FlatButton::clicked, this, &ModuleSelectionWidget::handleBtnNextClicked);

    auto btnNext2 = new QPushButton(this);
    btnNext2->setStyleSheet(styleSheet);
    btnNext2->setFont(font);
    btnNext2->setCursor(Qt::PointingHandCursor);
    btnNext2->setFixedSize(260, 32);
    btnNext2->setIconSize(QSize(16, 16));
    btnNext2->setIcon(QIcon(":/images/projects.svg"));
    btnNext2->setText("Previous Projects");
//    connect(btnNext2, &FlatButton::clicked, this, &ModuleSelectionWidget::handlebtnNext2Clicked);

    auto btnNext3 = new QPushButton(this);
    btnNext3->setStyleSheet(styleSheet);
    btnNext3->setFont(font);
    btnNext3->setCursor(Qt::PointingHandCursor);
    btnNext3->setFixedSize(260, 32);
    btnNext3->setIconSize(QSize(16, 16));
    btnNext3->setIcon(QIcon(":/images/edit.svg"));
    btnNext3->setText("Connect Manually");
//    connect(btnNext3, &FlatButton::clicked, this, &ModuleSelectionWidget::handlebtnNext3Clicked);

    auto btnNext4 = new QPushButton(this);
    btnNext4->setStyleSheet(styleSheet);
    btnNext4->setFont(font);
    btnNext4->setCursor(Qt::PointingHandCursor);
    btnNext4->setFixedSize(260, 32);
    btnNext4->setIconSize(QSize(16, 16));
    btnNext4->setText("Settings");
    btnNext4->setIcon(QIcon(":/images/settings.svg"));
//    connect(btnNext3, &FlatButton::clicked, this, &ModuleSelectionWidget::handlebtnNext3Clicked);

    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(18);
    m_buttonLayout->addWidget(btnNext, 0, Qt::AlignHCenter);
    m_buttonLayout->addWidget(btnNext3, 0, Qt::AlignHCenter);
    m_buttonLayout->addWidget(btnNext2, 0, Qt::AlignHCenter);
    m_buttonLayout->addWidget(btnNext4, 0, Qt::AlignHCenter);

}
