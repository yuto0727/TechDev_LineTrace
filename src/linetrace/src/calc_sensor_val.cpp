/**********************************************************************
Include Libraries
**********************************************************************/
#include "ros/ros.h"

// #include "std_msgs/Float32.h"
#include "std_msgs/Float32MultiArray.h"
// #include "std_msgs/Int32.h"
// #include "std_msgs/Int32MultiArray.h"

/**********************************************************************
Declare MACRO
**********************************************************************/
#define CTRL_PERIOD 0.02f

/**********************************************************************
Proto_type_Declare functions
**********************************************************************/
inline void cb_msg_light_sensor_val(const std_msgs::Float32MultiArray::ConstPtr &msg);
inline std::vector<float> calc_wheel_speeds(std::vector<float> sensor_val);

/**********************************************************************
Declare variables
**********************************************************************/
// Subしたデータの代入配列はvectorの必要あり
std::vector<float> light_sensor_val_vec = {0, 0, 0, 0, 0, 0, 0, 0};

// モータースピード送信配列
std_msgs::Float32MultiArray msg_wheel_speeds;

// しきい値計算用
const float white = 0.94f;
const float black = 0.33f;
const float threshold = (white + black) / 2.0f;

// デフォルトのモーターパワー（％）
const float wheel_speed_default = 50.0f

/**********************************************************************
Declare Instances
**********************************************************************/


/**********************************************************************
Main
**********************************************************************/
int main(int argc, char **argv)
{
    ros::init(argc, argv, "calc_sensor_val");

    ros::NodeHandle nh;
    ros::Subscriber sub_msg_light_sensor_val = nh.subscribe("light_sensor", 100, cb_msg_light_sensor_val);
    ros::Publisher pub_msg_wheel_speeds = nh.advertise<std_msgs::Float32MultiArray>("wheel_speeds", 100);

    ros::Rate loop_rate(1 / CTRL_PERIOD);

    msg_wheel_speeds.data.resize(2);

    while (ros::ok()){
        // Below, running in a Control Period.
        ros::spinOnce();

        // calc_wheel_speedsの戻り値をそのまま代入してpublish
        msg_wheel_speeds.data = calc_wheel_speeds(light_sensor_val_vec);
        pub_msg_wheel_speeds.publish(msg_wheel_speeds);

        ROS_INFO("A: %f B: %f", light_sensor_val_vec[3], light_sensor_val_vec[4]);

        loop_rate.sleep();
    }
    return 0;
}

/**********************************************************************
Functions <Call Back>
**********************************************************************/
inline void cb_msg_light_sensor_val(const std_msgs::Float32MultiArray::ConstPtr &msg){
    light_sensor_val_vec = msg->data;
}

/**********************************************************************
Functions
**********************************************************************/
inline std::vector<float> calc_wheel_speeds(std::vector<float> sensor_val){
    std::vector<float> wheel_speeds = {wheel_speed_default, wheel_speed_default};
    // 引数のセンサーの値からモーターパワーを計算

    // 黒のラインがどこに有るか探査
    int black_detected_sensor = 100;
    for (int i=0; i < 8; i++){
        if (sensor_val[i] <= threshold){
            // センサーiが黒のとき
            black_detected_sensor = i;
        }
    }

    if (black_detected_sensor == 3){
        wheel_speeds[0] += 15.0f;
    }else if(black_detected_sensor == 4){
        wheel_speeds[1] += 15.0f;
    }else{
    }

    return wheel_speeds;
}
