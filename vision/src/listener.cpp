#include "ros/ros.h"
#include <pcl/point_types.h>
#include "std_msgs/String.h"
#include "pcl_ros/point_cloud.h"
#include <pcl/point_types_conversion.h>
#include <iostream>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/common/centroid.h>

using namespace pcl;

ros::Publisher pub;

void pclCallback(const sensor_msgs::PointCloud2ConstPtr& input)
{
  PointCloud<pcl::PointXYZRGB> cloud_raw;
  fromROSMsg(*input, cloud_raw); 

  PointCloud<pcl::PointXYZHSV>::Ptr cloud_ptr(new PointCloud<pcl::PointXYZHSV>());
  PointCloudXYZRGBtoXYZHSV(cloud_raw, *cloud_ptr);

  PassThrough<PointXYZHSV> color_filter;
  color_filter.setInputCloud(cloud_ptr);
  color_filter.setFilterFieldName("h");
  color_filter.setFilterLimits(82, 162);
  //color_filter.setFilterLimits(31, 44);
  color_filter.filter(*cloud_ptr);

  color_filter.setFilterFieldName("s");
  color_filter.setFilterLimits(0.2, 0.7);
  //color_filter.setFilterLimits(0.34, 0.6);
  color_filter.filter(*cloud_ptr);

  color_filter.setFilterFieldName("v");
  color_filter.setFilterLimits(254, 255); //range from 0 to 255
 // color_filter.setFilterLimits(201, 360);
  color_filter.filter(*cloud_ptr);
  //cout << cloud_ptr->points.size () << endl;
  ROS_INFO("cloud size: %d", cloud_ptr->points.size());
  PointCloud<pcl::PointXYZHSV>::Ptr cloud_outlier_filtered (new pcl::PointCloud<pcl::PointXYZHSV>);

  // Create the filtering object
  StatisticalOutlierRemoval<pcl::PointXYZHSV> sor;
  sor.setInputCloud (cloud_ptr);
  sor.setMeanK (50);
  sor.setStddevMulThresh (1.0);
  sor.filter (*cloud_outlier_filtered);


  Eigen::Vector4f centroid(0,0,0,0);
  int num;
  num = compute3DCentroid(*cloud_outlier_filtered, centroid); 	
  //ROS_INFO("centroid x: %lf", *centroid[0]);
  //ROS_INFO("centroid x: %lf", *centroid[1]);
  //ROS_INFO("centroid x: %lf", *centroid[2]);
  ROS_INFO("num used for centroid: %d", num);
  std::cout << centroid << std::endl;
  ROS_INFO("cloud size: %d", cloud_outlier_filtered->points.size());
  sensor_msgs::PointCloud2 output;
  toROSMsg(*cloud_outlier_filtered, output);
  output.header.frame_id = "camera_link";
  pub.publish(output);

  ROS_INFO("Yes i'm actually alive");
  //ROS_INFO("I heard: [%s]", msg->width.c_str());
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "pcl_listener");
  ROS_INFO("Hello world!");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe< sensor_msgs::PointCloud2> ("camera/depth_registered/points", 1000, pclCallback);
  pub = nh.advertise<sensor_msgs::PointCloud2> ("output", 1);
  ros::spin();
  return 0;
}
