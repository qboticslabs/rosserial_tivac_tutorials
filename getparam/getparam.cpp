/*********************************************************************
 *
 *  Copyright (c) 2016 Robosavvy Ltd.
 *  Author: Vitor Matos
 *
 *  rosserial_tivac getparam tutorial
 *
 *  On this demo your TivaC Launchpad will publish the param string
 * over the topic "/param".
 *
 * Full guide: http://wiki.ros.org/rosserial_tivac/Tutorials
 *
 *********************************************************************/

#include <stdbool.h>
#include <stdint.h>
// TivaC specific includes
extern "C"
{
  #include <driverlib/sysctl.h>
  #include <driverlib/gpio.h>
}
// ROS includes
#include <ros.h>
#include <std_msgs/Float32.h>

// ROS nodehandle
ros::NodeHandle nh;

std_msgs::Float32 float_msg;
ros::Publisher param_pub("param", &float_msg);

int main(void)
{
  // TivaC application specific code
  MAP_FPUEnable();
  MAP_FPULazyStackingEnable();
  // TivaC system clock configuration. Set to 80MHz.
  MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

  // ROS nodehandle initialization and topic registration
  nh.initNode();
  nh.advertise(param_pub);
  
  while (!nh.connected())
    nh.spinOnce();
    
  bool param_success = false;
  float_msg.data = -1;
  param_success = nh.getParam("~my_param", &float_msg.data);
  
  while (1)
  {
    if (nh.connected())
    {
      // Publish message to be transmitted.
      param_pub.publish(&float_msg);
    }

    // Handle all communications and callbacks.
    nh.spinOnce();

    // Delay for a bit.
    nh.getHardware()->delay(100);
  }
}
