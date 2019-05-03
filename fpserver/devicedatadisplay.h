#ifndef DEVICEDATADISPLAY_H
#define DEVICEDATADISPLAY_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>


class DeviceDataDisplay : public QFrame
{
    Q_OBJECT

    friend class MainWindow;

private:
    QLabel      *m_lblTitle;
    QLabel      *m_lblNameTag;
    QLabel      *m_lblName;
    QLabel      *m_lblDataValue;

    QGridLayout *m_GridLayout;

    //device details
    QString      m_IdentifiedName;
    QString      m_AdvertisedName;
    int          m_Data;

public:
    DeviceDataDisplay(QWidget *parent = nullptr);

public slots:
    void setNames(QString identified, QString advertised);
    void dataUpdated(int data);

};

#endif // DEVICEDATADISPLAY_H


