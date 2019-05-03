
#ifndef GLOBALS_H
#define GLOBALS_H

#include <QTcpServer>



extern QTcpServer   *g_TcpServer;

int   getIncrementalTime();
void idleAway(int millisecs);
void setBackgroundColour(QWidget *widget, const QColor& colour);
void setForegroundColour(QWidget *widget, const QColor& colour);


#endif // GLOBALS_H
