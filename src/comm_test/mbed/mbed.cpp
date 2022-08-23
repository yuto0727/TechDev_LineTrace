/*********************
編集履歴
daichi ofuchi

esa ros課題2　mbed
**********************/
/**********************************************************************
Include Libraries
**********************************************************************/
#include "mbed.h"
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
/**********************************************************************
Declare MACRO
**********************************************************************/
#define CTRL_PERIOD 0.02f
/**********************************************************************
Proto_type_Declare functions
**********************************************************************/
inline void cb_led(const std_msgs::Float32MultiArray &msg);

/**********************************************************************
Declare variables
**********************************************************************/
Timer ControlTicker;

float led_msg = 0;
std_msgs::Float32 msg_float;

ros::Publisher pub_2_msg("led2", &msg_float);
ros::Subscriber<std_msgs::Float32MultiArray> led_sub("led", &cb_led);
/**********************************************************************
Declare Instances
**********************************************************************/
DigitalOut led(LED1);
/**********************************************************************
Main
**********************************************************************/

int main(int argc, char **argv)
{
    ControlTicker.start();
    ros::NodeHandle nh;
    nh.getHardware()->setBaud(115200);
    nh.initNode();
    nh.advertise(pub_2_msg);
    nh.subscribe(led_sub);
    msg_float.data = 0;
    for (;;)
    {
        if (ControlTicker.read() >= CTRL_PERIOD)
        {
            ControlTicker.reset();

            msg_float.data = led_msg*2;
            pub_2_msg.publish(&msg_float);

            led = !led;
            nh.spinOnce();
            wait(led_msg);
        }
    }
}

/*****************************************************************
Functions
*****************************************************************/
inline void cb_led(const std_msgs::Float32MultiArray &msg)
{
    led_msg = msg.data[0];
}