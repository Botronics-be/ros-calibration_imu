#include "fmain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // Initialize ROS
    ros::init(argc, argv, "calibration_imu");

    QApplication a(argc, argv);
    fmain w;
    w.show();
    return a.exec();
}
