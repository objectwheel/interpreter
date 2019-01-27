#include <centralwidget.h>
#include <radarwidget.h>

#include <QVBoxLayout>
#include <QLabel>

CentralWidget::CentralWidget(QWidget* parent) : QWidget(parent)
  , m_layout(new QVBoxLayout(this))
  , m_radarWidget(new RadarWidget(this))
  , m_searchingLabel(new QLabel(this))
{
    m_layout->setContentsMargins(8, 8, 8, 8);
    m_layout->setSpacing(8);
    m_layout->addStretch(1);
    m_layout->addWidget(m_radarWidget, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_searchingLabel, 0, Qt::AlignHCenter);
    m_layout->addStretch(3);

    QPalette palette(m_radarWidget->palette());
    palette.setColor(QPalette::Link, "#803382B5");        // Wave color
    palette.setColor(QPalette::Base, "#3382B5");          // Base color
    palette.setColor(QPalette::Button, "#ffffff");        // Frame color
    palette.setColor(QPalette::ButtonText, "#242a2d");    // Frame border color
    palette.setColor(QPalette::LinkVisited, "#50ffffff"); // Circle color
    palette.setColor(QPalette::ToolTipBase, "#ffffff");   // Needle color
    palette.setColor(QPalette::ToolTipText, "#85cfff");   // Scanning color
    m_radarWidget->setPalette(palette);

    QFont font;
    font.setPixelSize(17);
    font.setWeight(QFont::Light);
    m_searchingLabel->setFont(font);
    m_searchingLabel->setText(tr("Searching for Objectwheel..."));
}
