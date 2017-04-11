#include <fstream>//for file save
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#include "ros/ros.h"  
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"

#define             PI                  3.1415926
#define             RANGES_MAX          15.0
#define             RANGES_MIN          0.2
#define             RANGES_LEN          360

using namespace std;
	
class SensorDataItem
{
public:
    float Dist;
    int Strength;
    int IndexInCycle;
    int Quality_Score;
    float Angle;
};

#define PRINT_READ

SensorDataItem getLatarData(int SerialCom);

int main(int argc, char **argv)
{
    //Initialize the ROS system
    ros::init(argc , argv, "SerialPort");
    //Establish this program as a ROS node
    ros::NodeHandle nh ;


    /*********************打开串口**********************************/
    const char *COMName = "/dev/ttyUSB0";
    int SerialCom = open(COMName, O_RDWR);    
    if(SerialCom == -1)
    {
        printf("Can't open serial port!\n");
    }
    struct termios options;
    if(tcgetattr(SerialCom, &options) != 0)
    {
        printf("Can't get serial port sets!\n");
    }
    tcflush(SerialCom, TCIFLUSH);
    cfsetispeed(&options, B115200); //波特率设置   
    cfsetospeed(&options, B115200);    
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_iflag &= ~(INLCR | ICRNL | IGNCR);
    options.c_oflag &= ~(ONLCR | OCRNL);
    if(tcsetattr(SerialCom, TCSANOW, &options) != 0)
    {
        printf("Can't set serial port options!\n");
    }
    
    unsigned char writebuff[] = {0x42, 0x57, 0x02, 0x00, 0x00, 0x00, 0x02, 0x02};
    printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", writebuff[0], writebuff[1],
			 writebuff[2], writebuff[3], writebuff[4], writebuff[5], writebuff[6], writebuff[7]);
    int wread = 0;
    wread = write(SerialCom, writebuff, sizeof(writebuff));
    printf("%d\n", wread);

    /********************串口初始化结束********************************/
    ros::Publisher read_pub = nh.advertise<sensor_msgs::LaserScan>("scan", 30);

    std::map<float, float> laser_point;
    sensor_msgs::LaserScan de_laser_scan;

    while(ros::ok())
    {
        ros::spinOnce();

        //读取一个雷达数据
        SensorData = getLatarData(SerialCom);

        if(SensorData.Angle < PI && SensorData.Angle > -PI)
        {
            laser_point.insert(std::make_pair(SensorData.Angle, SensorData.Dist));
        }

        if(laser_point.size() == RANGES_LEN - 1)
        {
            de_laser_scan.ranges.clear();
            std::map<float, float>::iterator it;

            it = laser_point.begin();
            de_laser_scan.angle_min = laser_point.begin()->first;
            de_laser_scan.range_min = RANGES_MIN;
            de_laser_scan.range_max = RANGES_MAX;

            //find max and min to threthold distance
            for ( ; it != laser_point.end(); it ++ )
            {
                de_laser_scan.angle_max = it->first;//角度
                //threthold_range_max
                if( it->second > de_laser_scan.range_max )
                {
                    it->second = de_laser_scan.range_max;//距离
                }
                //threthold_range_min
                if( it->second < de_laser_scan.range_min )
                {
                    it->second = de_laser_scan.range_min;
                }

                de_laser_scan.ranges.push_back(it->second);
                //de_laser_scan.intensities.push_back(0);
            }

            de_laser_scan.angle_increment = (de_laser_scan.angle_max - de_laser_scan.angle_min) / RANGES_LEN;
            //time_increment
            de_laser_scan.time_increment = 1.0f/3/360;
            //header-stamp
            de_laser_scan.header.stamp = ros::Time::now();
            //header-frame_id:1-gloable, 0-no frame
            
            //根据cartographer提示修改，缺少horizontal_laser_link
            //de_laser_scan.header.frame_id = "base_laser_link";
            de_laser_scan.header.frame_id = "horizontal_laser_link";
            //publish scan
            read_pub.publish(de_laser_scan);
            //ROS_INFO_STREAM("PUBLISHED");
            //de_laser_scan.ranges.clear();
            laser_point.clear();
        }
	}
	return 0;
}

/*
读取一帧雷达数据，serialCom为代开的USB接口
*/
SensorDataItem getLatarData(int SerialCom)
{
    static queue<unsigned char> myqueue;
    
    unsigned char readbuff[512];
    unsigned short mybuff[20];
    int num = 4;    //数值不能太大，大了会造成数据丢失
    unsigned char a = '0',b = '0';
    SensorDataItem mySensor;
	float Dist;
    float Angle;
	int Strength;
    static int frameNum = 0;

    while(read(SerialCom,readbuff, num ) > 0)
    {
        //把读入到的四个字符写入队列
        for(int i = 0; i < num; ++i)
	    {
             myqueue.push(readbuff[i]);
        }

        //判断队列内是否存在一个完整可解析结构
        //完整的一个数据是12个字节，大于23意味着队列内一定存在一个完整的雷达数据
        while(myqueue.size() > 23)
        {
            b = a;
            a = myqueue.front();
            myqueue.pop();
            
            //判断是否是信息头
            if(a == 0x59 && b == 0x59)
            {
                unsigned short checksum = 89 + 89;//加上信息头
                for(int j = 0; j < 8; ++j)
                {
                    mybuff[j] = myqueue.front();
                    myqueue.pop();
                    checksum += mybuff[j];
                }
                mybuff[8] = myqueue.front();//返回数据的校验位
                myqueue.pop();

                //校验位判断
                if((checksum & 0x00ff) == mybuff[8])
                {
                    Dist = (256.f * mybuff[1] + mybuff[0]) / 1000;//距离，单位是米
         	        Strength = 256.f * mybuff[3] + mybuff[2];//信号质量
                    Angle = (256.f * mybuff[5] + mybuff[4]) / 10 / 180 * PI - PI;//角度
#ifdef PRINT_READ                
                    printf("frameNum %d, Dist %f, Strength %u, Angle %f\n",
                            frameNum, Dist, Strength, Angle);
#endif                            
                    mySensor.Dist = Dist;
                    mySensor.Strength = Strength;
                    mySensor.Angle = Angle;
                    frameNum ++;
                    return mySensor;
                }
                a = '0'; //更新a，以防校验数与标志位误匹配
             }
         }
    }
}
