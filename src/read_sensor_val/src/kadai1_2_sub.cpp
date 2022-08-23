/*********************************
 daichiofuchi

esa ros課題1-2 subscribe
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

inline void cb_msg_int(const std_msgs::Int32::ConstPtr &msg);
inline void cb_msg_float(const std_msgs::Float32::ConstPtr &msg);
inline void cb_msg_int_array(const std_msgs::Int32MultiArray::ConstPtr &msg);
inline void cb_msg_float_array(const std_msgs::Float32MultiArray::ConstPtr &msg);
/**********************************************************************
Declare variables
**********************************************************************/
int student_number;
float pi;
std::vector<int> int_vec = {0, 0, 0};
std::vector<float> float_vec = {0, 0};
/**********************************************************************
Main
**********************************************************************/

int main(int argc, char **argv)
{
    ros::init(argc, argv, "sub");
    ros::NodeHandle n;

    ros::Subscriber sub_msg_int = n.subscribe("int_topic", 100, cb_msg_int);
    ros::Subscriber sub_msg_float = n.subscribe("float_topic", 100, cb_msg_float);
    ros::Subscriber sub_msg_int_array = n.subscribe("int_array_topic", 100, cb_msg_int_array);
    ros::Subscriber sub_msg_float_array = n.subscribe("float_array_topic", 100, cb_msg_float_array);

    ros::Rate loop_rate(1 / CTRL_PERIOD);
    while (ros::ok())
    {
        ros::spinOnce();

        ROS_INFO("student number:%d", student_number);
        ROS_INFO("pi:%f", pi);
        ROS_INFO("%dyear%dmonth%dday", int_vec[0], int_vec[1], int_vec[2]);
        ROS_INFO("%fcm,%fkg", float_vec[0], float_vec[1]);

        loop_rate.sleep();
    }
}
/*****************************************************************
Functions <Call Back>
*****************************************************************/
inline void cb_msg_int(const std_msgs::Int32::ConstPtr &msg){
    student_number = msg->data;
}
inline void cb_msg_float(const std_msgs::Float32::ConstPtr &msg){
    pi = msg->data;
}
inline void cb_msg_int_array(const std_msgs::Int32MultiArray::ConstPtr &msg){
    int_vec = msg->data;
}
inline void cb_msg_float_array(const std_msgs::Float32MultiArray::ConstPtr &msg){
    float_vec = msg->data;
}