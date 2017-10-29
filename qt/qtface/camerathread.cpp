#include "camerathread.h"
#include <QDebug>

void CameraThread::run()
{
    qDebug() << "threa run ";

//    timer_show = new QTimer(this); //qtimer need to be included not qtime
//    connect(timer_show, SIGNAL(timeout()), this, SLOT(threadCamera()));
//    timer_show->setInterval(40);
//    timer_show->start();

//    this->exec();
    while(1)
    {

    }
}
