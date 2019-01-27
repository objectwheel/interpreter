#include <centralwidget.h>
#include <radarwidget.h>

#include <QVBoxLayout>
#include <QLabel>

CentralWidget::CentralWidget(QWidget* parent) : QWidget(parent)
  , m_layout(new QVBoxLayout(this))
  , m_searchingLabel(new QLabel(this))
{
    m_layout->setContentsMargins(8, 8, 8, 8);
    m_layout->setSpacing(8);
    m_layout->addStretch(1);
    m_layout->addWidget(m_searchingLabel, 0, Qt::AlignHCenter);
    m_layout->addStretch(3);

    QFont font;
    font.setPixelSize(17);
    font.setWeight(QFont::Light);
    m_searchingLabel->setFont(font);
    m_searchingLabel->setText(tr("Searching for Objectwheel..."));
}
