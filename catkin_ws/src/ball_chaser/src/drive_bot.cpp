#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"

ros::Publisher motor_command_publisher;

bool handle_drive_request(ball_chaser::DriveToTarget::Request& req,
    ball_chaser::DriveToTarget::Response& res){

        // ROS_INFO("DriveToTarget recivido / DriveToTarget received- linear_x:%1.2f, angular_z:%1.2f",
            //  (float)req.linear_x, (float)req.angular_z);
        geometry_msgs::Twist motor_command;
        
        // Poner los comandos enviados a sus respectivos campos
        motor_command.linear.x = req.linear_x;
        motor_command.angular.z = req.angular_z;

        // Publicar la data
        motor_command_publisher.publish(motor_command);
        res.msg_feedback = "Velocidad lineal en x / Linear Velocity x - Linear_x: " + std::to_string(req.linear_x) + "\n, Velocidad angular en z / Angular Velocity z: " + std::to_string(req.angular_z);
        // ROS_INFO_STREAM(res.msg_feedback);
        return true;

    }

int main(int argc, char** argv) {

    // Inicializar el nodo de ros
    ros::init(argc, argv, "drive_bot");

    // Crear el objeto de ros
    ros::NodeHandle n;

    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    ros::ServiceServer service;
    service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);

    // ROS_INFO("Listo para enviar los datos/ Ready for send command");

    ros::spin();

    return 0;


}


