/*
 * robot_interface.hpp
 *
 * Created on: Jul 08, 2021 11:48
 * Description:
 *
 * Copyright (c) 2021 Weston Robot Pte. Ltd.
 */

#ifndef ROBOT_INTERFACE_HPP
#define ROBOT_INTERFACE_HPP

#include <string>

#include "ugv_sdk/details/interface/agilex_message.h"
#include "ugv_sdk/details/interface/parser_interface.hpp"

#define AGX_MAX_ACTUATOR_NUM 8

namespace westonrobot {
struct CoreStateMsgGroup {
  SystemStateMessage system_state;
  MotionStateMessage motion_state;
  LightStateMessage light_state;
  MotionModeStateMessage motion_mode_state;
  RcStateMessage rc_state;
};

struct ActuatorStateMsgGroup {
  ActuatorHSStateMessage actuator_hs_state[AGX_MAX_ACTUATOR_NUM];  // v2 only
  ActuatorLSStateMessage actuator_ls_state[AGX_MAX_ACTUATOR_NUM];  // v2 only
  ActuatorStateMessageV1 actuator_state[AGX_MAX_ACTUATOR_NUM];     // v1 only
};

struct CommonSensorStateMsgGroup {};

class RobotCommonInterface {
 public:
  ~RobotCommonInterface() = default;

  // functions to be implemented by class AgilexBase
  virtual void EnableCommandedMode() = 0;

  // functions to be implemented by each robot class
  virtual void Connect(std::string can_name) = 0;

  virtual void ResetRobotState() = 0;

  virtual void DisableLightControl() {
    // do nothing if no light on robot
  }

  virtual ProtocolVersion GetParserProtocolVersion() = 0;

 protected:
  /****** functions not available/valid to all robots ******/
  // functions to be implemented by class AgilexBase
  virtual void SetMotionMode(uint8_t mode){};
  virtual CoreStateMsgGroup GetRobotCoreStateMsgGroup() {
    return CoreStateMsgGroup{};
  };
  virtual ActuatorStateMsgGroup GetActuatorStateMsgGroup() {
    return ActuatorStateMsgGroup{};
  };

  // any specific robot will use a specialized version of the two functions
  virtual void SendMotionCommand(double linear_vel, double angular_vel,
                                 double lateral_velocity,
                                 double steering_angle){
      // use derived version
  };

  virtual void SendLightCommand(LightMode front_mode,
                                uint8_t front_custom_value, LightMode rear_mode,
                                uint8_t rear_custom_value){};
};
}  // namespace westonrobot

#endif /* ROBOT_INTERFACE_HPP */
