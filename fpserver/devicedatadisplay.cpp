
#include <QSpacerItem>

#include "devicedatadisplay.h"
#include "globals.h"



DeviceDataDisplay::DeviceDataDisplay(QWidget *parent)
    : QFrame(parent)
{

    this->setFrameStyle(QFrame::Panel | QFrame::Raised);

    m_GridLayout = new QGridLayout(this);

    m_lblTitle = new QLabel;
    m_lblNameTag = new QLabel;
    m_lblName = new QLabel;
    m_lblDataValue = new QLabel;

    m_GridLayout->addWidget(m_lblTitle,     0, 0, 1, 2);
    m_GridLayout->addWidget(m_lblNameTag,   1, 0, 1, 1);
    m_GridLayout->addWidget(m_lblName,      1, 1, 1, 1);
    m_GridLayout->addWidget(m_lblDataValue, 2, 0, 1, 2);

    m_GridLayout->setRowStretch(0, 2);
    m_GridLayout->setRowStretch(1, 1);
    m_GridLayout->setRowStretch(2, 4);

    m_lblTitle->setText("<UNKNOWN>");
    m_lblTitle->setFrameStyle(QFrame::Panel | QFrame::Raised);
    m_lblTitle->setAlignment(Qt::AlignCenter);

    QFont fnt = m_lblTitle->font();
    fnt.setPointSize(20);
    fnt.setBold(true);
    m_lblTitle->setFont(fnt);

    m_lblNameTag->setText("Advertised as:");
    m_lblNameTag->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    fnt = m_lblNameTag->font();
    fnt.setPointSize(12);
    m_lblNameTag->setFont(fnt);

    m_lblName->setFont(fnt);
    m_lblName->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    m_lblDataValue->setAlignment(Qt::AlignCenter);
    fnt = m_lblDataValue->font();
    fnt.setPointSize(32);
    m_lblDataValue->setFont(fnt);


    m_GridLayout->setMargin(1);

    setBackgroundColour(this, "#9bff94");

    this->setFixedSize(250, 140);
}



void DeviceDataDisplay::setNames(QString identified, QString advertised)
{
    m_IdentifiedName = identified;
    m_AdvertisedName = advertised;

    m_lblTitle->setText(identified);
    m_lblName->setText(advertised);
}




void DeviceDataDisplay::dataUpdated(int data)
{
    m_Data = data;
    m_lblDataValue->setText( QString::number(data) );
}


