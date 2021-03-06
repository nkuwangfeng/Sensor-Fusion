/*
 * @Description: IMU-GNSS fusion for localization workflow
 * @Author: Ge Yao
 * @Date: 2020-11-12 15:14:07
 */
#ifndef LIDAR_LOCALIZATION_FILTERING_IMU_GNSS_ODO_FILTERING_HPP_
#define LIDAR_LOCALIZATION_FILTERING_IMU_GNSS_ODO_FILTERING_HPP_

#include <deque>
#include <Eigen/Dense>
#include <yaml-cpp/yaml.h>

#include "lidar_localization/ESKFStd.h"

#include "lidar_localization/sensor_data/imu_data.hpp"
#include "lidar_localization/sensor_data/pos_vel_data.hpp"
#include "lidar_localization/sensor_data/pose_data.hpp"

#include "lidar_localization/models/kalman_filter/error_state_kalman_filter.hpp"

namespace lidar_localization {

class IMUGNSSOdoFiltering {
  public:
    IMUGNSSOdoFiltering();

    bool Init(
      const Eigen::Matrix4f& init_pose,
      const Eigen::Vector3f &init_vel,
      const IMUData &init_imu_data
    );

    bool Update(
      const IMUData &imu_data
    );
    bool Correct(
      const IMUData &imu_data,
      const PosVelData &pos_vel_data
    );

    // getters:
    bool HasInited() const { return has_inited_; }

    double GetTime(void) { return kalman_filter_ptr_->GetTime(); }
    Eigen::Matrix4f GetPose(void) { return current_pose_; }
    Eigen::Vector3f GetVel(void) { return current_vel_; }
    void GetOdometry(Eigen::Matrix4f &pose, Eigen::Vector3f &vel);
    void GetStandardDeviation(ESKFStd &eskf_std_msg);
    void SaveObservabilityAnalysis(void);
    
  private:
    bool InitWithConfig(void);
    bool InitFusion(const YAML::Node& config_node);

    // init pose setter:
    bool SetInitGNSS(const Eigen::Matrix4f& init_pose);
    bool SetInitPose(const Eigen::Matrix4f& init_pose);

  private:
    bool has_inited_ = false;

    // IMU-GNSS Kalman filter:
    std::shared_ptr<ErrorStateKalmanFilter> kalman_filter_ptr_;
    ErrorStateKalmanFilter::Measurement current_measurement_;
    
    Eigen::Matrix4f current_gnss_pose_ = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f init_pose_ = Eigen::Matrix4f::Identity(); 
    Eigen::Matrix4f current_pose_ = Eigen::Matrix4f::Identity();
    Eigen::Vector3f current_vel_ = Eigen::Vector3f::Zero();
    ErrorStateKalmanFilter::Cov current_cov_;
};

} // namespace lidar_localization

#endif // LIDAR_LOCALIZATION_FILTERING_IMU_GNSS_ODO_FILTERING_HPP_