/**********************************************************************
Include Libraries
**********************************************************************/
#include "mbed.h"
#include <ros.h>
#include <std_msgs/Float32MultiArray.h>

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

float sensor_val[4] = {0.0f, 0.0f, 0.0f, 0.0f};

/**********************************************************************
Declare Instances
**********************************************************************/
Serial PC(USBTX, USBRX, 115200);
AnalogIn LightSensor_A(A0);
AnalogIn LightSensor_B(A1);
AnalogIn LightSensor_C(A2);
AnalogIn LightSensor_D(A3);

std_msgs::Float32MultiArray msg_sensor_val;
ros::Publisher pub_msg("light_sensor", &msg_sensor_val);

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

    msg_sensor_val.data_length = 4;

    while (1)
    {
        if (ControlTicker.read() >= CTRL_PERIOD)
        {
            ControlTicker.reset();
            // Below, running in a Control Period.

            sensor_val[0] = LightSensor_A.read();
            sensor_val[1] = LightSensor_B.read();
            sensor_val[2] = LightSensor_C.read();
            sensor_val[3] = LightSensor_D.read();

            msg_sensor_val.data = sensor_val;

            pub_msg.publish(&msg_sensor_val);
            PC.printf("%f, %f, %f, %f\r\n", sensor_val[0], sensor_val[1], sensor_val[2], sensor_val[3]);

            nh.spinOnce();
        }
    }
}

/*****************************************************************
Functions
*****************************************************************/