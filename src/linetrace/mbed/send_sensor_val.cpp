/**********************************************************************
Include Libraries
**********************************************************************/
#include "mbed.h"
// #include "dc_motor_1.h"
// #include "rotary_encoder.h"

#include <ros.h>
// #include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>
// #include <std_msgs/Int32.h>
// #include <std_msgs/Int32MultiArray.h>

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
Timer ControlTicker;
float light_sensor_val[8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
const PinName LightSensorPin[8] = {PA_0, PA_1, PA_3, PA_4, PA_5, PA_6, PA_7, PB_1};

/**********************************************************************
Declare Instances
**********************************************************************/
Serial PC(USBTX, USBRX, 115200);

std_msgs::Float32MultiArray msg_light_sensor_val;
ros::Publisher pub_msg("light_sensor", &msg_light_sensor_val);

// AnalogIn sensor_A(LightSensorPin[0]);
AnalogIn *LightSensors[8];

/**********************************************************************
Main
**********************************************************************/
int main()
{
    ControlTicker.start();
    ros::NodeHandle nh;
    nh.getHardware()->setBaud(115200);
    nh.initNode();
    nh.advertise(pub_msg);

    msg_light_sensor_val.data_length = 8;

    for (int i=0; i < 8; i++){
        LightSensors[i] = new AnalogIn(LightSensorPin[i]);
    }

    for (;;){
        if (ControlTicker.read() >= CTRL_PERIOD){
            ControlTicker.reset();
            // Below, running in a Control Period.

            for (int i=0; i < 8; i++){
                light_sensor_val[i] = LightSensors[i]->read();
            }

            msg_light_sensor_val.data = light_sensor_val;
            pub_msg.publish(&msg_light_sensor_val);

            for (int i=0; i < 8; i++){
                PC.printf("%f ", light_sensor_val[i]);
            }
            PC.printf("\r\n");

            nh.spinOnce();
        }
    }
}

/**********************************************************************
Functions <Call Back>
**********************************************************************/


/**********************************************************************
Functions
**********************************************************************/