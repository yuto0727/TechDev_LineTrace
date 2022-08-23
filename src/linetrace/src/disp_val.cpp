/**********************************************************************
Include Libraries
**********************************************************************/
#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"

/**********************************************************************
Declare MACRO
**********************************************************************/
#define CTRL_PERIOD 0.02f

/**********************************************************************
Proto_type_Declare functions
**********************************************************************/
inline void cb_msg_float_array(const std_msgs::Float32MultiArray::ConstPtr &msg);

/**********************************************************************
Declare variables
**********************************************************************/
std::vector<float> float_vec = {0, 0};

/**********************************************************************
Main
**********************************************************************/

int main(int argc, char **argv)
{
    ros::init(argc, argv, "sub");
    ros::NodeHandle n;

    ros::Subscriber sub_msg_float_array = n.subscribe("wheel_speeds", 100, cb_msg_float_array);

    ros::Rate loop_rate(1 / CTRL_PERIOD);
    while (ros::ok())
    {
        ros::spinOnce();

        ROS_INFO("%f, %f", float_vec[0], float_vec[1]);

        loop_rate.sleep();
    }
}
/*****************************************************************
Functions <Call Back>
*****************************************************************/
inline void cb_msg_float_array(const std_msgs::Float32MultiArray::ConstPtr &msg){
    float_vec = msg->data;
}
