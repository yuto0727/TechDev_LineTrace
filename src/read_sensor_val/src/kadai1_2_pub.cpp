/*********************************
daichi ofuchi

esa ros課題1-2 publish
**********************************/
/**********************************************************************
Include Libraries
**********************************************************************/

#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/Float32MultiArray.h"

/**********************************************************************
Declare MACRO
**********************************************************************/

#define CTRL_PERIOD 0.02f
/**********************************************************************
Proto_type_Declare functions
**********************************************************************/

/**********************************************************************
Declare variables
**********************************************************************/

std_msgs::Int32 msg_int;
std_msgs::Float32 msg_float;
std_msgs::Int32MultiArray msg_int_array;
std_msgs::Float32MultiArray msg_float_array;

/**********************************************************************
Main
**********************************************************************/

int main(int argc, char **argv)
{
    ros::init(argc, argv, "pub");
    ros::NodeHandle n;

    ros::Publisher pub_int_msg = n.advertise<std_msgs::Int32>("int_topic", 100);
    ros::Publisher pub_float_msg = n.advertise<std_msgs::Float32>("float_topic", 100);
    ros::Publisher pub_int_array = n.advertise<std_msgs::Int32MultiArray>("int_array_topic", 100);
    ros::Publisher pub_float_array = n.advertise<std_msgs::Float32MultiArray>("float_array_topic", 100);

    ros::Rate loop_rate(1 / CTRL_PERIOD);

    msg_int.data = 31;

    msg_float.data = 3.14;

    msg_int_array.data.resize(3);
    msg_int_array.data[0] = 2002;
    msg_int_array.data[1] = 7;
    msg_int_array.data[2] = 27;

    msg_float_array.data.resize(2);
    msg_float_array.data[0] = 180.5;
    msg_float_array.data[1] = 55.5;


    while (ros::ok())
    {
        ros::spinOnce();

        pub_int_msg.publish(msg_int);
        pub_float_msg.publish(msg_float);
        pub_int_array.publish(msg_int_array);
        pub_float_array.publish(msg_float_array);

        loop_rate.sleep();
    }

    return 0;
}
/*****************************************************************
Functions <Call Back>
*****************************************************************/