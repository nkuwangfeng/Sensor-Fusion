/*
 * @Description: IMU-GNSS-odom measurement preprocessing workflow
 * @Author: Ge Yao
 * @Date: 2020-11-21 15:39:24
 */
#include <ros/ros.h>
#include "glog/logging.h"

#include "lidar_localization/global_defination/global_defination.h"
#include "lidar_localization/data_pretreat/imu_gnss_odo_preprocess_flow.hpp"

using namespace lidar_localization;

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_log_dir = WORK_SPACE_PATH + "/Log";
    FLAGS_alsologtostderr = 1;

    ros::init(argc, argv, "imu_gnss_odo_preprocess_node");
    ros::NodeHandle nh;

    // subscribe to
    // a. raw IMU measurement
    // b. raw GNSS measurement
    // c. raw odometer measurement
    // d. reference trajectory
    // publish
    // a. synced IMU for ESKF prediction
    // b. synced GNSS-odo for ESKF correction
    // b. synced reference trajectory for evo evaluation
    std::shared_ptr<IMUGNSSOdoPreprocessFlow> imu_gnss_odo_preprocess_flow_ptr = std::make_shared<
        IMUGNSSOdoPreprocessFlow
    >(
        nh
    );

    // pre-process IMU, GNSS & odo measurements at 100Hz:
    ros::Rate rate(100);
    while (ros::ok()) {
        ros::spinOnce();

        imu_gnss_odo_preprocess_flow_ptr->Run();

        rate.sleep();
    }

    return EXIT_SUCCESS;
}