/**
mbed側のプログラム 送られてきた値をそのままアクチュエータに突っ込む
web上でコンパイル
**/
/**********************************************************************
   Problems to be fixed
    - pinは未定
**********************************************************************/
/**********************************************************************
Include Libraries
**********************************************************************/
#include "mbed.h"
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>
#include "dc_motor_1.h"
#include "rotary_encoder.h"

/**********************************************************************
Declare MACRO
**********************************************************************/
#define CTRL_PERIOD 0.02f

/**********************************************************************
Proto_type_Declare functions
**********************************************************************/
inline void setOrder();
inline void cb_wheel_speeds(const std_msgs::Float32MultiArray &msg);
inline void cb_cylinder_arm(const std_msgs::Bool &msg);
inline void cb_cylinder_raise_paper(const std_msgs::Bool &msg);
inline void cb_slide_rod_speed(const std_msgs::Float32 &msg);

/**********************************************************************
Declare variables
**********************************************************************/
Timer ControlTicker;
// ↓この辺初期値要るかも
float catch_wheel_speeds[2] = {0.0f, 0.0f}; //{Lモーター, Rモーター}の値を格納
bool catch_cylinder_arm;
bool catch_cylinder_raise_paper;
float catch_slide_rod_speed;

/**********************************************************************
Declare Instances
**********************************************************************/
Serial PC(USBTX, USBRX, 115200);
dc_motor_1 wheel_L(PB_0, PF_1, 1);
dc_motor_1 wheel_R(PA_7, PB_5, 0);
rotary_encoder enco_wheel_L(D4, D5, NC, 50, rotary_encoder::X4_ENCODING);
rotary_encoder enco_wheel_R(D4, D5, NC, 50, rotary_encoder::X4_ENCODING);
DigitalOut solenoid_arm(A3);
DigitalOut solenoid_raise_paper(A3);
dc_motor_1 slide_rod(PA_7, PB_5, 0);

// スライドしきったことを感知するスイッチ
DigitalIn stop_switch1(D1);
DigitalIn stop_switch2(D2);

std_msgs::Float32MultiArray msg_enco_wheels;

ros::Publisher pub_enco_wheels("enco_wheels", &msg_enco_wheels);
ros::Subscriber<std_msgs::Float32MultiArray> sub_wheel_speeds("wheel_speeds", &cb_wheel_speeds);
ros::Subscriber<std_msgs::Bool> sub_cylinder_arm("cylinder_arm", &cb_cylinder_arm);
ros::Subscriber<std_msgs::Bool> sub_cylinder_raise_paper("cylinder_raise_paper", &cb_cylinder_raise_paper);
ros::Subscriber<std_msgs::Float32> sub_slide_rod_speed("slide_rod_speed", &cb_slide_rod_speed);

/**********************************************************************
   Main
**********************************************************************/
int main(int argc, char **argv)
{
    ControlTicker.start();
    ros::NodeHandle n;
    n.getHardware()->setBaud(115200);
    n.initNode();
    n.advertise(pub_enco_wheels);
    n.subscribe(sub_wheel_speeds);
    n.subscribe(sub_cylinder_arm);
    n.subscribe(sub_cylinder_raise_paper);
    n.subscribe(sub_slide_rod_speed);

    float temp_enco_wheels[2];
    msg_enco_wheels.data_length = 2;

    for (;;)
    {
        if (ControlTicker.read() >= CTRL_PERIOD)
        {
            ControlTicker.reset();
            setOrder();
            n.spinOnce();

            temp_enco_wheels[0] = enco_wheel_L.getRad();
            temp_enco_wheels[1] = enco_wheel_R.getRad();
            msg_enco_wheels.data = temp_enco_wheels;
        }
    }
}

/**********************************************************************
   Functions
**********************************************************************/
void setOrder()
{
    wheel_L.drive(catch_wheel_speeds[0]);
    wheel_R.drive(catch_wheel_speeds[1]);
    solenoid_arm = catch_cylinder_arm;
    solenoid_raise_paper = catch_cylinder_raise_paper;

    // スライドする機構が壊れないようにするため
    // 不等号の向き不明
    if (stop_switch1.read() == 0 && catch_slide_rod_speed > 0)
    {
        slide_rod.drive(0);
        return;
    }
    if (stop_switch2.read() == 0 && catch_slide_rod_speed < 0)
    {
        slide_rod.drive(0);
        return;
    }
    slide_rod.drive(catch_slide_rod_speed);
}

inline void cb_wheel_speeds(const std_msgs::Float32MultiArray &msg)
{
    catch_wheel_speeds[0] = msg.data[0];
    catch_wheel_speeds[1] = msg.data[1];
}

inline void cb_cylinder_arm(const std_msgs::Bool &msg)
{
    catch_cylinder_arm = msg.data;
}

inline void cb_cylinder_raise_paper(const std_msgs::Bool &msg)
{
    catch_cylinder_raise_paper = msg.data;
}

inline void cb_slide_rod_speed(const std_msgs::Float32 &msg)
{
    catch_slide_rod_speed = msg.data;
}