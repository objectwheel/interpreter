#include <centralwidget.h>
#include <radarwidget.h>

#include <QVBoxLayout>
#include <QLabel>

CentralWidget::CentralWidget(QWidget* parent) : QWidget(parent)
  , m_layout(new QVBoxLayout(this))
  , m_searchingIcon(new RadarWidget(this))
  , m_searchingText(new QLabel(this))
{
    m_layout->setContentsMargins(8, 8, 8, 8);
    m_layout->setSpacing(8);
    m_layout->addStretch(1);
    m_layout->addWidget(m_searchingIcon, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_searchingText, 0, Qt::AlignHCenter);
    m_layout->addStretch(3);

    QFont font;
    font.setPixelSize(18);
    font.setWeight(QFont::ExtraLight);
    m_searchingText->setFont(font);
    m_searchingText->setText(tr("Searching for Objectwheel..."));
}
