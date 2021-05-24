#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

ros::ServiceClient client;

void drive_robot(float lin_X, float ang_z){
    
    
    ROS_INFO_STREAM("Reciviendo data" + std::to_string(lin_X) + "," + std::to_string(ang_z));

    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_X;
    srv.request.angular_z = ang_z;

    if (!client.call(srv))
        ROS_ERROR("Error llamando el servicio // Error calling the service");
        
}

void process_image_callback(const sensor_msgs::Image img)
{
    // int matrix[img.height][img.width];
    int position;
    int white_pixel = 255;
    int pixel_encontado = 2;
    float left = img.step/3.0;
    float center = 2*img.step/3.0;
    
     for (int i=0; i< img.height * img.step; i++){
        if (img.data[i] == white_pixel){
            ROS_INFO_STREAM("Pixel encontrado");
            position = i;
            pixel_encontado = 1;
            break;
        }
        else{
            pixel_encontado = 0;
        }
     }
     int pos_pixel = position % img.step;
    ROS_INFO_STREAM("Posicion pixel" + std::to_string(pos_pixel));
    
         if (pixel_encontado == 1)
        {
            if (pos_pixel < left)
            {   ROS_INFO("Pixel encontrado en la izquierda");
                drive_robot(0.0,0.25);
                }
            else if(pos_pixel < center && pos_pixel > left)
            {   ROS_INFO("Pixel encontrado en el centro");
                drive_robot(0.25,0);
                }
            else if(pos_pixel > center)
            {   ROS_INFO("Pixel encontrado en la derecha");
                drive_robot(0.0, -0.25);
                }
        }
        else if (pixel_encontado == 0)
            {
                drive_robot(0,0);
            }
        pixel_encontado = 2 ;
    ros::Duration(0.5).sleep();

}


int main(int argc, char** argv){

    // Inicializa el nodo
    ros::init(argc, argv, "process_image");

    // 
    ros::NodeHandle n;

    // Define un cliente capaz de requerir los comandos del robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
    // Subscribirse a /camera/rgb/image_raw
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    ros::spin();

    return 0;
        
}