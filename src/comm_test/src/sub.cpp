/*********************************
 daichiofuchi

esa ros課題1-2 subscribe
**********************************/
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
inline void cb_msg_float(const std_msgs::Float32MultiArray::ConstPtr &msg);
inline std::vector<float> calc(std::vector<float> data);

/**********************************************************************
Declare variables
**********************************************************************/
std::vector<float> val = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

/**********************************************************************
Main
**********************************************************************/

int main(int argc, char **argv)
{
    ros::init(argc, argv, "sub");
    ros::NodeHandle n;

    ros::Subscriber sub_msg_float = n.subscribe("light_sensor", 100, cb_msg_float);

    ros::Rate loop_rate(1 / CTRL_PERIOD);
    while (ros::ok())
    {
        ros::spinOnce();

        std::vector<float> v = calc(val);

        ROS_INFO("%f, %f", val[3], v[0]);

        loop_rate.sleep();
    }
}
/*****************************************************************
Functions <Call Back>
*****************************************************************/
inline void cb_msg_float(const std_msgs::Float32MultiArray::ConstPtr &msg){
    val = msg->data;
}

inline std::vector<float> calc(std::vector<float> data){
    std::vector<float> val = {0.0f, 0.0f};

    val[0] = data[3] * 10.0f;
    return val;
}