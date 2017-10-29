#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QThread>
#include <QTimer>

class CameraThread : public QThread
{
    Q_OBJECT
    void run() override;

public:
    QTimer* timer_show;

};

#endif // CAMERATHREAD_H
