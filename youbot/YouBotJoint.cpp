/****************************************************************
 *
 * Copyright (c) 2011
 * All rights reserved.
 *
 * Hochschule Bonn-Rhein-Sieg
 * University of Applied Sciences
 * Computer Science Department
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Author:
 * Jan Paulus, Nico Hochgeschwender, Michael Reckhaus, Azamat Shakhimardanov
 * Supervised by:
 * Gerhard K. Kraetzschmar
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * This sofware is published under a dual-license: GNU Lesser General Public 
 * License LGPL 2.1 and BSD license. The dual-license implies that users of this
 * code may choose which terms they prefer.
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Hochschule Bonn-Rhein-Sieg nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License LGPL as
 * published by the Free Software Foundation, either version 2.1 of the
 * License, or (at your option) any later version or the BSD license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License LGPL and the BSD license for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License LGPL and BSD license along with this program.
 *
 ****************************************************************/
#include "youbot/YouBotJoint.hpp"
namespace youbot {

YouBotJoint::YouBotJoint(const unsigned int jointNo, const std::string& configFilePath) {
  // Bouml preserved body begin 000412F1
    this->jointNumber = jointNo;
    timeTillNextMailboxUpdate = 1; //ms
    mailboxMsgRetries = 100;
    this->storage.inverseMovementDirection = false;
    this->storage.lowerLimit = 0;
    this->storage.upperLimit = 0;
    this->storage.areLimitsActive = false;
    std::stringstream jointNameStream;
    jointNameStream << "Joint " << this->jointNumber << " ";
    jointNameString = jointNameStream.str();
    ethercatMaster = &(EthercatMaster::getInstance("youbot-ethercat.cfg", configFilePath));
    
  // Bouml preserved body end 000412F1
}

YouBotJoint::~YouBotJoint() {
  // Bouml preserved body begin 00041371
  // Bouml preserved body end 00041371
}

void YouBotJoint::setConfigurationParameter(const JointParameter& parameter) {
  // Bouml preserved body begin 00074271
  throw std::runtime_error("Please use YouBotJointParameters");
  // Bouml preserved body end 00074271
}

void YouBotJoint::getConfigurationParameter(JointParameter& parameter) {
  // Bouml preserved body begin 0005CE71
  throw std::runtime_error("Please use YouBotJointParameters");
  // Bouml preserved body end 0005CE71
}

void YouBotJoint::getConfigurationParameter(YouBotJointParameterReadOnly& parameter) {
  // Bouml preserved body begin 00071FF1

    if (parameter.getType() == MOTOR_CONTOLLER_PARAMETER) {

      YouBotSlaveMailboxMsg message;
      parameter.getYouBotMailboxMsg(message, GAP, storage);

      message.parameterName = parameter.getName();
      if (retrieveValueFromMotorContoller(message)) {
        parameter.setYouBotMailboxMsg(message, storage);
      } else {
        throw JointParameterException("Unable to get parameter: " + parameter.getName() + " from joint: " + this->jointName);
      }
    }else{
      throw JointParameterException("Parameter " + parameter.getName() + " is not a motor controller parameter of a joint");
    }
  // Bouml preserved body end 00071FF1
}

void YouBotJoint::getConfigurationParameter(YouBotJointParameter& parameter) {
  // Bouml preserved body begin 0005BCF1
    if (parameter.getType() == MOTOR_CONTOLLER_PARAMETER) {

      YouBotSlaveMailboxMsg message;
      parameter.getYouBotMailboxMsg(message, GAP, storage);

      message.parameterName = parameter.getName();
      if (retrieveValueFromMotorContoller(message)) {
        parameter.setYouBotMailboxMsg(message, storage);
      } else {
        throw JointParameterException("Unable to get parameter: " + parameter.getName() + " from joint: " + this->jointName);
      }
    }else{
      throw JointParameterException("Parameter " + parameter.getName() + " is not a motor controller parameter of a joint");
    }
  // Bouml preserved body end 0005BCF1
}

void YouBotJoint::setConfigurationParameter(const YouBotJointParameter& parameter) {
  // Bouml preserved body begin 0005BC71
    if (parameter.getType() == MOTOR_CONTOLLER_PARAMETER) {

      YouBotSlaveMailboxMsg message;
      parameter.getYouBotMailboxMsg(message, SAP, storage);

      message.parameterName = parameter.getName();
      if (!setValueToMotorContoller(message)) {
        throw JointParameterException("Unable to set parameter: " + parameter.getName() + " to joint: " + this->jointName);
      }
    }else{
      throw JointParameterException("Parameter " + parameter.getName() + " is not a motor controller parameter of a joint");
    }
  // Bouml preserved body end 0005BC71
}

void YouBotJoint::getConfigurationParameter(JointName& parameter) {
  // Bouml preserved body begin 000740F1
    parameter.value = this->jointName;
  // Bouml preserved body end 000740F1
}

void YouBotJoint::setConfigurationParameter(const JointName& parameter) {
  // Bouml preserved body begin 0005CDF1
    this->jointName = parameter.value;
  // Bouml preserved body end 0005CDF1
}

void YouBotJoint::getConfigurationParameter(GearRatio& parameter) {
  // Bouml preserved body begin 00074171
    parameter.setParameter(this->storage.gearRatio);
  // Bouml preserved body end 00074171
}

void YouBotJoint::setConfigurationParameter(const GearRatio& parameter) {
  // Bouml preserved body begin 00073FF1
    if (parameter.value == 0) {
      throw std::out_of_range("A Gear Ratio of zero is not allowed");
    }
    this->storage.gearRatio = parameter.value;
  // Bouml preserved body end 00073FF1
}

void YouBotJoint::getConfigurationParameter(EncoderTicksPerRound& parameter) {
  // Bouml preserved body begin 000741F1
    parameter.setParameter(this->storage.encoderTicksPerRound);
  // Bouml preserved body end 000741F1
}

void YouBotJoint::setConfigurationParameter(const EncoderTicksPerRound& parameter) {
  // Bouml preserved body begin 00074071
    if (parameter.value == 0) {
      throw std::out_of_range("Zero Encoder Ticks per Round are not allowed");
    }
    this->storage.encoderTicksPerRound = parameter.value;
  // Bouml preserved body end 00074071
}

void YouBotJoint::setConfigurationParameter(const CalibrateJoint& parameter) {
  // Bouml preserved body begin 000623F1
    if (parameter.doCalibration) {
      LOG(info) << "Calibrate Joint: " << this->jointName;

      int calibrationVel = 0; //rpm
      //YouBotSlaveMsg messageBuffer;
      messageBuffer.stctOutput.controllerMode = VELOCITY_CONTROL;
      if (parameter.calibrationDirection == POSITIV) {
        calibrationVel = 1.0 / storage.gearRatio;
      } else if (parameter.calibrationDirection == NEGATIV) {
        calibrationVel = -1.0 / storage.gearRatio;
      } else {
        throw std::runtime_error("No calibration direction for joint: " + this->jointName);
      }

      if (this->storage.inverseMovementDirection == true) {
        calibrationVel *= -1;
      }

      JointSensedCurrent sensedCurrent;

      messageBuffer.stctOutput.controllerMode = VELOCITY_CONTROL;
      messageBuffer.stctOutput.value = calibrationVel;
      ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);

      sensedCurrent.current = 0;
      //turn till a max current is reached
      while (abs(sensedCurrent.current) < abs(parameter.maxCurrent)) {
        SLEEP_MILLISEC(timeTillNextMailboxUpdate);
        this->getData(sensedCurrent);
      }

      //stop movement
      messageBuffer.stctOutput.controllerMode = VELOCITY_CONTROL;
      messageBuffer.stctOutput.value = 0;
      //   LOG(trace) << "vel [rpm] " << messageBuffer.stctOutput.value << " rad_sec " << data.angularVelocity;
      ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);

      //set encoder reference position
      SLEEP_MILLISEC(500);
      messageBuffer.stctOutput.controllerMode = SET_POSITION_TO_REFERENCE;
      messageBuffer.stctOutput.value = 0;
      ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);

      //switch to position controll
      SLEEP_MILLISEC(100);
      messageBuffer.stctOutput.controllerMode = POSITION_CONTROL;
      messageBuffer.stctOutput.value = 0;
      //   LOG(trace) << "vel [rpm] " << messageBuffer.stctOutput.value << " rad_sec " << data.angularVelocity;
      ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);

      //     LOG(info) << "Calibration finished for joint: " << this->jointName;
    }

  // Bouml preserved body end 000623F1
}

void YouBotJoint::setConfigurationParameter(const InverseMovementDirection& parameter) {
  // Bouml preserved body begin 000624F1
    this->storage.inverseMovementDirection = parameter.value;
  // Bouml preserved body end 000624F1
}

void YouBotJoint::getConfigurationParameter(InverseMovementDirection& parameter) {
  // Bouml preserved body begin 000C9671
    parameter.setParameter(this->storage.inverseMovementDirection);
  // Bouml preserved body end 000C9671
}

void YouBotJoint::setConfigurationParameter(const JointLimits& parameter) {
  // Bouml preserved body begin 000D4371

    this->storage.lowerLimit = parameter.lowerLimit;
    this->storage.upperLimit = parameter.upperLimit;
    this->storage.areLimitsActive = parameter.areLimitsActive;
    ethercatMaster->setJointLimits(parameter.lowerLimit, parameter.upperLimit, storage.inverseMovementDirection, parameter.areLimitsActive, this->jointNumber);

  // Bouml preserved body end 000D4371
}

void YouBotJoint::getConfigurationParameter(JointLimits& parameter) {
  // Bouml preserved body begin 000C95F1
    parameter.setParameter(this->storage.lowerLimit, this->storage.upperLimit, this->storage.areLimitsActive);
  // Bouml preserved body end 000C95F1
}

void YouBotJoint::getConfigurationParameter(JointLimitsRadian& parameter) {
  // Bouml preserved body begin 000D43F1
    quantity<plane_angle> lowlimit = ((double) this->storage.lowerLimit / storage.encoderTicksPerRound) * storage.gearRatio * (2.0 * M_PI) * radian;
    quantity<plane_angle> uplimit = ((double) this->storage.upperLimit / storage.encoderTicksPerRound) * storage.gearRatio * (2.0 * M_PI) * radian;
    parameter.setParameter(lowlimit, uplimit, this->storage.areLimitsActive);
  // Bouml preserved body end 000D43F1
}

void YouBotJoint::setConfigurationParameter(const InitializeJoint& parameter) {
  // Bouml preserved body begin 000973F1
    if (parameter.value) {
      //YouBotSlaveMsg messageBuffer;
      messageBuffer.stctOutput.controllerMode = INITIALIZE;
      messageBuffer.stctOutput.value = 0;

      ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);
    }
  // Bouml preserved body end 000973F1
}

void YouBotJoint::getConfigurationParameter(FirmwareVersion& parameter) {
  // Bouml preserved body begin 0009AA71

    YouBotSlaveMailboxMsg message;
    parameter.getYouBotMailboxMsg(message, GAP, storage);

    bool unvalid = true;
    unsigned int retry = 0;

    ethercatMaster->setMailboxMsgBuffer(message, this->jointNumber);

    SLEEP_MILLISEC(timeTillNextMailboxUpdate);

    do {
      if( ethercatMaster->getMailboxMsgBuffer(message, this->jointNumber) ) {
        unvalid = false;
      } else {
        SLEEP_MILLISEC(timeTillNextMailboxUpdate);
        retry++;
      }
    } while (retry < mailboxMsgRetries && unvalid);

    if (unvalid) {
      this->parseMailboxStatusFlags(message);
      throw std::runtime_error("Unable to get firmware version for joint: " + this->jointName);
      return;
    } 
    
    char versionString[8] = {0};
    versionString[0] = message.stctInput.replyAddress;
    versionString[1] = message.stctInput.moduleAddress;
    versionString[2] = message.stctInput.status;
    versionString[3] = message.stctInput.commandNumber;
    versionString[4] = message.stctInput.value >> 24;
    versionString[5] = message.stctInput.value >> 16;
    versionString[6] = message.stctInput.value >> 8;
    versionString[7] = message.stctInput.value & 0xff;
    
    int controllerType = 0;
    float firmwareVersion = 0;
    sscanf (versionString,"%dV%f",&controllerType,&firmwareVersion);
    
    parameter.setParameter(controllerType, firmwareVersion);

    return;
  // Bouml preserved body end 0009AA71
}

///this method should be only used if you know what you are doing
void YouBotJoint::setConfigurationParameter(const YouBotSlaveMailboxMsg& parameter) {
  // Bouml preserved body begin 000A9D71
   if (!setValueToMotorContoller(parameter)) {
     throw JointParameterException("Unable to set parameter at joint: " + this->jointName);
   }
  // Bouml preserved body end 000A9D71
}

///this method should be only used if you know what you are doing
void YouBotJoint::getConfigurationParameter(YouBotSlaveMailboxMsg& parameter) {
  // Bouml preserved body begin 000A9DF1
   if (!retrieveValueFromMotorContoller(parameter)) {
     throw JointParameterException("Unable to get parameter from joint: " + this->jointName);
   }
   this->parseMailboxStatusFlags(parameter);
  // Bouml preserved body end 000A9DF1
}

void YouBotJoint::getConfigurationParameter(TorqueConstant& parameter) {
  // Bouml preserved body begin 000D9571
   parameter.setParameter(this->storage.torqueConstant);
  // Bouml preserved body end 000D9571
}

void YouBotJoint::setConfigurationParameter(const TorqueConstant& parameter) {
  // Bouml preserved body begin 000C7171
   parameter.getParameter(this->storage.torqueConstant);
  // Bouml preserved body end 000C7171
}

///stores the joint parameter permanent in the EEPROM of the motor contoller
///Attentions: The EEPROM has only a finite number of program-erase cycles
void YouBotJoint::storeConfigurationParameterPermanent(const YouBotJointParameter& parameter) {
  // Bouml preserved body begin 000919F1
    if (parameter.getType() == MOTOR_CONTOLLER_PARAMETER) {

      this->setConfigurationParameter(parameter);

      YouBotSlaveMailboxMsg message;
      parameter.getYouBotMailboxMsg(message, STAP, storage);

      if (!setValueToMotorContoller(message)) {
        throw JointParameterException("Unable to store parameter: " + parameter.getName() + " to joint: " + this->jointName);
      }
    }else{
      throw JointParameterException("Parameter " + parameter.getName() + " is not a motor controller parameter of a joint");
    }
  // Bouml preserved body end 000919F1
}

/// Restores the joint parameter from the EEPROM.
void YouBotJoint::restoreConfigurationParameter(YouBotJointParameter& parameter) {
  // Bouml preserved body begin 00091A71
    if (parameter.getType() == MOTOR_CONTOLLER_PARAMETER) {

      YouBotSlaveMailboxMsg message;
      parameter.getYouBotMailboxMsg(message, RSAP, storage);

      if (!setValueToMotorContoller(message)) {
        throw JointParameterException("Unable to restore parameter: " + parameter.getName() + " at joint: " + this->jointName);
      }

      this->getConfigurationParameter(parameter);
    }else{
      throw JointParameterException("Parameter " + parameter.getName() + " is not a motor controller parameter of a joint");
    }
  // Bouml preserved body end 00091A71
}

void YouBotJoint::setData(const JointDataSetpoint& data, SyncMode communicationMode) {
  // Bouml preserved body begin 000413F1
    LOG(info) << "Nothing to do";
  // Bouml preserved body end 000413F1
}

void YouBotJoint::getData(JointData& data) {
  // Bouml preserved body begin 00041471
    LOG(info) << "Nothing to do";
  // Bouml preserved body end 00041471
}

///commands a position or angle to one joint
///@param data the to command position
///@param communicationMode at the moment only non blocking communication is implemented
void YouBotJoint::setData(const JointAngleSetpoint& data, SyncMode communicationMode) {
  // Bouml preserved body begin 0003C1F1

    //YouBotSlaveMsg messageBuffer;
    ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
    this->parseYouBotErrorFlags(messageBuffer);
    
    if (storage.gearRatio == 0) {
      throw std::out_of_range("A Gear Ratio of zero is not allowed");
    }
    
    if (storage.encoderTicksPerRound == 0) {
      throw std::out_of_range("Zero Encoder Ticks per Round are not allowed");
    }

    if(storage.areLimitsActive){

      quantity<plane_angle> lowLimit = ((double) this->storage.lowerLimit / storage.encoderTicksPerRound) * storage.gearRatio * (2.0 * M_PI) * radian;
      quantity<plane_angle> upLimit = ((double) this->storage.upperLimit / storage.encoderTicksPerRound) * storage.gearRatio * (2.0 * M_PI) * radian;
      
      if (storage.inverseMovementDirection) {
        upLimit = ((double) -(this->storage.lowerLimit) / storage.encoderTicksPerRound) * storage.gearRatio * (2.0 * M_PI) * radian;
        lowLimit = ((double) -(this->storage.upperLimit) / storage.encoderTicksPerRound) * storage.gearRatio * (2.0 * M_PI) * radian;
      }
      

      if (!((data.angle < upLimit) && (data.angle > lowLimit))) {
        std::stringstream errorMessageStream;
        errorMessageStream << "The setpoint angle is out of range. The valid range is between " << lowLimit << " and " << upLimit << " and it is: " << data.angle;
        //    LOG(trace) << "abs_value: " << abs(data.angle) << " abslow " << abs(lowLimit) << " absupper " << abs(upLimit);
        throw std::out_of_range(errorMessageStream.str());
      }
    }

    messageBuffer.stctOutput.controllerMode = POSITION_CONTROL;
    messageBuffer.stctOutput.value = (int32) round((data.angle.value() * ((double) storage.encoderTicksPerRound / (2.0 * M_PI))) / storage.gearRatio);


    if (storage.inverseMovementDirection) {
      messageBuffer.stctOutput.value *= -1;
    }
    //   LOG(trace) << "value: " << data.angle << " gear " << gearRatio << " encoderperRound " << encoderTicksPerRound << " encPos " << messageBuffer.stctOutput.value << " joint " << this->jointNumber;
    ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);
  // Bouml preserved body end 0003C1F1
}

///commands a encoder value (position) to one joint
///@param data the to command encoder value
///@param communicationMode at the moment only non blocking communication is implemented
void YouBotJoint::setData(const JointEncoderSetpoint& data, SyncMode communicationMode) {
  // Bouml preserved body begin 000C2371

    //YouBotSlaveMsg messageBuffer;
    ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
    this->parseYouBotErrorFlags(messageBuffer);
  
    if(storage.areLimitsActive){

      if (!((data.encoderTicks < this->storage.upperLimit) && (data.encoderTicks > this->storage.lowerLimit))) {
        std::stringstream errorMessageStream;
        errorMessageStream << "The setpoint angle is out of range. The valid range is between " << this->storage.lowerLimit << " and " << this->storage.upperLimit << " and it is: " << data.encoderTicks;
        //    LOG(trace) << "abs_value: " << abs(data.angle) << " abslow " << abs(lowLimit) << " absupper " << abs(upLimit);
        throw std::out_of_range(errorMessageStream.str());
      }
    }

    messageBuffer.stctOutput.controllerMode = POSITION_CONTROL;
    messageBuffer.stctOutput.value = data.encoderTicks;
    
    if (storage.inverseMovementDirection) {
      messageBuffer.stctOutput.value *= -1;
    }
    
     ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);
  // Bouml preserved body end 000C2371
}

///gets the position or angle of one joint which have been calculated from the actual encoder value 
///@param data returns the angle by reference
void YouBotJoint::getData(JointSensedAngle& data) {
  // Bouml preserved body begin 0003DCF1
    //YouBotSlaveMsg messageBuffer;
    ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
    this->parseYouBotErrorFlags(messageBuffer);

    if (storage.gearRatio == 0) {
      throw std::out_of_range("A Gear Ratio of zero is not allowed");
    }
    if (storage.encoderTicksPerRound == 0) {
      throw std::out_of_range("Zero Encoder Ticks per Round are not allowed");
    }
    //  LOG(trace) << "enc: " << messageBuffer.stctInput.actualPosition;
    data.angle = ((double) messageBuffer.stctInput.actualPosition / storage.encoderTicksPerRound) * storage.gearRatio * (2.0 * M_PI) * radian;

    if (storage.inverseMovementDirection) {
      data.angle = -data.angle;
    }
  // Bouml preserved body end 0003DCF1
}

///commands a velocity to one joint
///@param data the to command velocity
///@param communicationMode at the moment only non blocking communication is implemented
void YouBotJoint::setData(const JointVelocitySetpoint& data, SyncMode communicationMode) {
  // Bouml preserved body begin 0003C371
  
    //YouBotSlaveMsg messageBuffer;
    ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
    this->parseYouBotErrorFlags(messageBuffer);
  
    messageBuffer.stctOutput.controllerMode = VELOCITY_CONTROL;

    if (storage.gearRatio == 0) {
      throw std::out_of_range("A Gear Ratio of 0 is not allowed");
    }

    messageBuffer.stctOutput.value = (int32) round((data.angularVelocity.value() / (storage.gearRatio * 2.0 * M_PI)) * 60.0);
    if (storage.inverseMovementDirection) {
      messageBuffer.stctOutput.value *= -1;
    }

    //  LOG(trace) << "vel [rpm] " << messageBuffer.stctOutput.value << " rad_sec " << data.angularVelocity;
    ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);
  // Bouml preserved body end 0003C371
}

///gets the velocity of one joint
///@param data returns the velocity by reference
void YouBotJoint::getData(JointSensedVelocity& data) {
  // Bouml preserved body begin 0003DD71
    //YouBotSlaveMsg messageBuffer;
    ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
    this->parseYouBotErrorFlags(messageBuffer);

    if (storage.gearRatio == 0) {
      throw std::out_of_range("A Gear Ratio of 0 is not allowed");
    }
    double motorRPM = messageBuffer.stctInput.actualVelocity;
    //convert RPM of the motor to radian per second of the wheel/joint
    data.angularVelocity = ((motorRPM / 60.0) * storage.gearRatio * 2.0 * M_PI) * radian_per_second;
    
    if (storage.inverseMovementDirection) {
      data.angularVelocity *= -1;
    }
  // Bouml preserved body end 0003DD71
}

///gets the velocity in round per minute of one joint
///@param data returns the velocity by reference
void YouBotJoint::getData(JointSensedRoundsPerMinute& data) {
  // Bouml preserved body begin 000AEC71
    //YouBotSlaveMsg messageBuffer;
    ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
    this->parseYouBotErrorFlags(messageBuffer);

    data.rpm = messageBuffer.stctInput.actualVelocity;
    
    if (storage.inverseMovementDirection) {
      data.rpm *= -1;
    }
  // Bouml preserved body end 000AEC71
}

///sets the velocity in round per minute to one joint
///@param data the setpoint velocity
///@param communicationMode at the moment only non blocking communication is implemented
void YouBotJoint::setData(const JointRoundsPerMinuteSetpoint& data, SyncMode communicationMode) {
  // Bouml preserved body begin 000AECF1
    //YouBotSlaveMsg messageBuffer;
    ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
    this->parseYouBotErrorFlags(messageBuffer);
    
    messageBuffer.stctOutput.controllerMode = VELOCITY_CONTROL;
    messageBuffer.stctOutput.value = data.rpm;
    
    if (storage.inverseMovementDirection) {
      messageBuffer.stctOutput.value *= -1;
    }

    ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);
  // Bouml preserved body end 000AECF1
}

///gets the motor current of one joint which have been measured by a hal sensor
///@param data returns the actual motor current by reference
void YouBotJoint::getData(JointSensedCurrent& data) {
  // Bouml preserved body begin 0003DDF1
    //YouBotSlaveMsg messageBuffer;
    ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
    this->parseYouBotErrorFlags(messageBuffer);
    //convert mili ampere to ampere
    double current = messageBuffer.stctInput.actualCurrent;
    data.current =  current / 1000.0 * ampere;
    
    if (storage.inverseMovementDirection) {
      data.current *= -1;
    }
  // Bouml preserved body end 0003DDF1
}

///commands a current to one joint
///@param data the to command current
///@param communicationMode at the moment only non blocking communication is implemented
void YouBotJoint::setData(const JointCurrentSetpoint& data, SyncMode communicationMode) {
  // Bouml preserved body begin 000955F1
    //YouBotSlaveMsg messageBuffer;
    ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
    this->parseYouBotErrorFlags(messageBuffer);
    
    messageBuffer.stctOutput.controllerMode = CURRENT_MODE;
    messageBuffer.stctOutput.value = (int32) (data.current.value() * 1000.0);  //convert from Ampere to milli Ampere
    
    if (storage.inverseMovementDirection) {
      messageBuffer.stctOutput.value *= -1;
    }
    ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);
  // Bouml preserved body end 000955F1
}

///gets the actual PWM value of one joint
///@param data returns the PWM value by reference
void YouBotJoint::getData(JointSensedPWM& data) {
  // Bouml preserved body begin 000CAFF1
    //YouBotSlaveMsg messageBuffer;
    ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
    this->parseYouBotErrorFlags(messageBuffer);

    data.pwm = messageBuffer.stctInput.actualPWM;
    
    if (storage.inverseMovementDirection) {
      data.pwm *= -1;
    }
    
  // Bouml preserved body end 000CAFF1
}

///commands a pulse-width modulation to one joint
///@param data the to command pulse-width modulation
///@param communicationMode at the moment only non blocking communication is implemented
void YouBotJoint::setData(const JointPWMSetpoint& data, SyncMode communicationMode) {
  // Bouml preserved body begin 00095671
    //YouBotSlaveMsg messageBuffer;
    ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
    this->parseYouBotErrorFlags(messageBuffer);
    
    messageBuffer.stctOutput.controllerMode = PWM_MODE;
    messageBuffer.stctOutput.value = data.pwm;
    
    if (storage.inverseMovementDirection) {
      messageBuffer.stctOutput.value *= -1;
    }
    ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);
  // Bouml preserved body end 00095671
}

///gets the encoder ticks of one joint
///@param data returns the ticks by reference
void YouBotJoint::getData(JointSensedEncoderTicks& data) {
  // Bouml preserved body begin 000AB7F1
    //YouBotSlaveMsg messageBuffer;
    ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
    this->parseYouBotErrorFlags(messageBuffer);

    //  LOG(trace) << "enc: " << messageBuffer.stctInput.actualPosition;
    data.encoderTicks = messageBuffer.stctInput.actualPosition ;
    
    if (storage.inverseMovementDirection) {
      data.encoderTicks *= -1;
    }

  // Bouml preserved body end 000AB7F1
}

///sets the output part of a EtherCAT slave message
///this methode should be only used if you know what you are doing
///@param data output part of a EtherCAT slave message
///@param communicationMode at the moment only non blocking communication is implemented
void YouBotJoint::setData(const SlaveMessageOutput& data, SyncMode communicationMode) {
  // Bouml preserved body begin 000C5671
    //YouBotSlaveMsg messageBuffer;
    ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
    this->parseYouBotErrorFlags(messageBuffer);
    
    messageBuffer.stctOutput = data;
  
    ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);
  // Bouml preserved body end 000C5671
}

///gets the input and ouput part of a EtherCAT slave message
///this methode should be only used if you know what you are doing
///@param data returns the sensor values by reference
void YouBotJoint::getData(YouBotSlaveMsg& data) {
  // Bouml preserved body begin 000C56F1
    //YouBotSlaveMsg messageBuffer;
    ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
    this->parseYouBotErrorFlags(messageBuffer);

    data = messageBuffer;
  // Bouml preserved body end 000C56F1
}

///commands a torque to one joint
///@param data the to command torque
///@param communicationMode at the moment only non blocking communication is implemented
void YouBotJoint::setData(const JointTorqueSetpoint& data, SyncMode communicationMode) {
  // Bouml preserved body begin 000C7071
    JointCurrentSetpoint currentSetpoint;
    
    if (this->storage.torqueConstant == 0) {
      throw std::out_of_range("A torque constant of 0 is not allowed");
    }
   
    currentSetpoint.current = ((data.torque.value()*this->storage.gearRatio)/this->storage.torqueConstant) * ampere;
    this->setData(currentSetpoint);
  // Bouml preserved body end 000C7071
}

///gets the motor torque of one joint which have been calculated from the current
///@param data returns the actual motor torque by reference
void YouBotJoint::getData(JointSensedTorque& data) {
  // Bouml preserved body begin 000C70F1
  JointSensedCurrent sensedCurrent;
  this->getData(sensedCurrent);
  
  if (this->storage.gearRatio == 0) {
    throw std::out_of_range("A Gear Ratio of 0 is not allowed");
  }
  data.torque = ((sensedCurrent.current.value() * this->storage.torqueConstant) / this->storage.gearRatio) * newton_meter;
  // Bouml preserved body end 000C70F1
}

void YouBotJoint::getUserVariable(const unsigned int index, int& data) {
  // Bouml preserved body begin 000AD171
  
  if(index == 0 || index > 55){
    throw JointParameterException("User variable index is out of range use 1-55 at: " + this->jointName);
  }
  //56 is the last userdata at bank 2
    YouBotSlaveMailboxMsg message;
    message.stctOutput.moduleAddress = DRIVE;
    message.stctOutput.commandNumber = GGP;
    message.stctOutput.typeNumber = index;
    message.stctOutput.motorNumber = USER_VARIABLE_BANK;
    message.stctOutput.value = 0;
    
   if (!retrieveValueFromMotorContoller(message)) {
     throw JointParameterException("Unable to get parameter from joint: " + this->jointName);
   }
   this->parseMailboxStatusFlags(message);
   
   data = message.stctInput.value;
  // Bouml preserved body end 000AD171
}

void YouBotJoint::setUserVariable(const unsigned int index, const int data) {
  // Bouml preserved body begin 000AD1F1
  
  if(index == 0 || index > 55){
    throw JointParameterException("User variable index is out of range use 1-55 at: " + this->jointName);
  }
  //56 is the last userdata at bank 2
    YouBotSlaveMailboxMsg message;
    message.stctOutput.moduleAddress = DRIVE;
    message.stctOutput.commandNumber = GGP;
    message.stctOutput.typeNumber = index;
    message.stctOutput.motorNumber = USER_VARIABLE_BANK;
    message.stctOutput.value = data;
    
  if (!setValueToMotorContoller(message)) {
     throw JointParameterException("Unable to set parameter at joint: " + this->jointName);
   }
    this->parseMailboxStatusFlags(message);
  // Bouml preserved body end 000AD1F1
}

/// Returns the status messages for the motor controller. 
void YouBotJoint::getStatus(std::vector<std::string>& statusMessages) {
  // Bouml preserved body begin 000AD271
  //YouBotSlaveMsg messageBuffer;
  ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
  
  
  

    if (messageBuffer.stctInput.errorFlags & OVER_CURRENT) {
      statusMessages.push_back(jointNameString + "got over current");
    }

    if (messageBuffer.stctInput.errorFlags & UNDER_VOLTAGE) {
      statusMessages.push_back(jointNameString + "got under voltage");
    }

    if (messageBuffer.stctInput.errorFlags & OVER_VOLTAGE) {
      statusMessages.push_back(jointNameString + "got over voltage");
    }

    if (messageBuffer.stctInput.errorFlags & OVER_TEMPERATURE) {
      statusMessages.push_back(jointNameString + "got over temperature");
    }

    if (messageBuffer.stctInput.errorFlags & MOTOR_HALTED) {
      statusMessages.push_back(jointNameString + "is halted");
    }

    if (messageBuffer.stctInput.errorFlags & HALL_SENSOR_ERROR) {
      statusMessages.push_back(jointNameString + "got hall sensor problem");
    }

//    if (messageBuffer.stctInput.errorFlags & ENCODER_ERROR) {
//      statusMessages.push_back(jointNameString + "got encoder problem");
//    }
//
//     if (messageBuffer.stctInput.errorFlags & INITIALIZATION_ERROR) {
//      statusMessages.push_back(jointNameString + "got inizialization problem");
//    }

    if (messageBuffer.stctInput.errorFlags & PWM_MODE_ACTIVE) {
      statusMessages.push_back(jointNameString + "has PWM mode active");
    }

    if (messageBuffer.stctInput.errorFlags & VELOCITY_MODE) {
      statusMessages.push_back(jointNameString + "has velocity mode active");
    }

    if (messageBuffer.stctInput.errorFlags & POSITION_MODE) {
      statusMessages.push_back(jointNameString + "has position mode active");
    }

    if (messageBuffer.stctInput.errorFlags & TORQUE_MODE) {
      statusMessages.push_back(jointNameString + "has torque mode active");
    }

//    if (messageBuffer.stctInput.errorFlags & EMERGENCY_STOP) {
//      statusMessages.push_back(jointNameString + "has emergency stop active");
//    }
//
//    if (messageBuffer.stctInput.errorFlags & FREERUNNING) {
//      statusMessages.push_back(jointNameString + "has freerunning active");
//    }

    if (messageBuffer.stctInput.errorFlags & POSITION_REACHED) {
      statusMessages.push_back(jointNameString + "has position reached");
    }

    if (messageBuffer.stctInput.errorFlags & INITIALIZED) {
      statusMessages.push_back(jointNameString + "is initialized");
    }

    if (messageBuffer.stctInput.errorFlags & TIMEOUT) {
      statusMessages.push_back(jointNameString + "has a timeout");
    }

    if (messageBuffer.stctInput.errorFlags & I2T_EXCEEDED) {
      statusMessages.push_back(jointNameString + "exceeded I2t");
    }
  
  
  // Bouml preserved body end 000AD271
}

/// Returns the status messages as status flags for the motor controller. The status flag bits are assigned like this:
/// 0:  Overcurrent
/// 1:  Undervoltage
/// 2:  Overvoltage
/// 3:  Overtemperature
/// 4:  Motor halted
/// 5:  Hall error flag
/// 6:  ---
/// 7:  ---
/// 8:  PWM mode active
/// 9:  Velocity mode active
/// 10: Position mode active
/// 11: Torque mode active
/// 12: ---
/// 13: ---
/// 14: Position end flag
/// 15: Module initialized
/// 16: EtherCAT timeout flag
/// 17: I2t exceeded flag
void YouBotJoint::getStatus(unsigned int& statusFlags) {
  // Bouml preserved body begin 000AD2F1
  //YouBotSlaveMsg messageBuffer;
  ethercatMaster->getMsgBuffer(this->jointNumber, messageBuffer);
  
  statusFlags = messageBuffer.stctInput.errorFlags;
  // Bouml preserved body end 000AD2F1
}

/// set the encoder values of the joint to zero. This postion will be the new reference.
void YouBotJoint::setEncoderToZero() {
  // Bouml preserved body begin 000AED71

    //YouBotSlaveMsg messageBuffer;
    messageBuffer.stctOutput.controllerMode = SET_POSITION_TO_REFERENCE;
    messageBuffer.stctOutput.value = 0;

    ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);

  // Bouml preserved body end 000AED71
}

void YouBotJoint::noMoreAction() {
  // Bouml preserved body begin 000664F1
    //YouBotSlaveMsg messageBuffer;
    messageBuffer.stctOutput.controllerMode = NO_MORE_ACTION;
    messageBuffer.stctOutput.value = 0;

    ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);
  // Bouml preserved body end 000664F1
}

void YouBotJoint::stopJoint() {
  // Bouml preserved body begin 00066471
    //YouBotSlaveMsg messageBuffer;
    messageBuffer.stctOutput.controllerMode = MOTOR_STOP;
    messageBuffer.stctOutput.value = 0;

    ethercatMaster->setMsgBuffer(messageBuffer, this->jointNumber);
  // Bouml preserved body end 00066471
}

unsigned int YouBotJoint::getJointNumber() {
  // Bouml preserved body begin 000EA2F1
  return this->jointNumber;
  // Bouml preserved body end 000EA2F1
}

/// calculates all trajectory values for the future and sets all the the ethercat master
/// if the trajectory is still active the the values will be set in the next buffer
void YouBotJoint::setTrajectory(const std::vector< quantity<plane_angle> >& positions, const std::vector< quantity<angular_velocity> >& velocities, const std::vector< quantity<angular_acceleration> >& accelerations) {
  // Bouml preserved body begin 000E84F1
  std::list<int32> targetPositions;
  if(!this->trajectoryController.isTrajectoryControllerActive()){
    JointSensedAngle currentpose;
    JointSensedVelocity currentvel;
    this->getData(currentpose);
    this->getData(currentvel);
    lastVelocity = currentvel.angularVelocity;
    lastPosition = currentpose.angle;
  }
  
  for(unsigned int i = 0; i< positions.size(); i++) {
    this->calculatePositions(positions[i], lastPosition, velocities[i], lastVelocity,  accelerations[i], targetPositions);
    lastVelocity = velocities[i];
    lastPosition = positions[i];
  }
  
  this->trajectoryController.setTrajectoryPositions(targetPositions);
  
  // Bouml preserved body end 000E84F1
}

/// Stops just the trajectory controller but not the joint movement
void YouBotJoint::cancelTrajectory() {
  // Bouml preserved body begin 000E8571
  this->trajectoryController.cancelCurrentTrajectory();
  // Bouml preserved body end 000E8571
}

void YouBotJoint::calculatePositions(const quantity<plane_angle>& position, const quantity<plane_angle>& position_current, const quantity<angular_velocity>& velocity, const quantity<angular_velocity>& velocity_current, const quantity<angular_acceleration>& acceleration, std::list<int32>& targetPositions) {
  // Bouml preserved body begin 000EA071
  
  quantity<plane_angle> targetposition(0 *radian);
  quantity<boost::units::si::time> timestep(0*second);
  quantity<boost::units::si::time> timestep2(0*second);
  quantity<plane_angle> targetpositionAfterAcc(0 *radian);
  quantity<plane_angle> positionThreshold = 0.01*radian;
  int u = 0;
	int steps_per_second = 1000;
	steps_per_second = this->trajectoryController.getControllerUpdatesPerSecond();
  
  quantity<angular_velocity> velocity_calc;
	quantity<angular_velocity> zeroVel(0 * radian_per_second);
  quantity<angular_acceleration> zeroAcc(0 * radian_per_second/second);
	
	if(steps_per_second == 0)
		 throw JointErrorException("Invalid controller updates per second");
	
	if(position < position_current && (velocity >=zeroVel || acceleration >=zeroAcc))
		 throw JointErrorException("Invalid trajectory");
	
	if(position > position_current && (velocity <=zeroVel || acceleration <=zeroAcc))
		 throw JointErrorException("Invalid trajectory");
  
  
  for(int i= 1; targetposition <= (position-positionThreshold) || targetposition >= (position+positionThreshold) ;i++){
      timestep = ((double)i/steps_per_second)* second;
      targetposition = position_current + (velocity_current * timestep) + ((acceleration * timestep * timestep)/2.0);
      velocity_calc = (acceleration * timestep) +velocity_current;
      
      if(acceleration >= (0 * radian_per_second/second)){
        if(velocity_calc >= velocity){
          if(u == 0){
            u = i;
            targetpositionAfterAcc = targetposition;
          }
          
          timestep2 = ((double)(i-u)/steps_per_second)* second;
          targetposition = targetpositionAfterAcc + (velocity  * timestep2);
        }
      }else{
        if(velocity_calc <= velocity){
          if(u == 0){
            u = i;
            targetpositionAfterAcc = targetposition;
          }
          
          timestep2 = ((double)(i-u)/steps_per_second)* second;
          targetposition = targetpositionAfterAcc + (velocity  * timestep2);
        }
      }

    
    //////////////////////////////////////////////////
    if (storage.gearRatio == 0) {
      throw std::out_of_range("A Gear Ratio of zero is not allowed");
    }
    
    if (storage.encoderTicksPerRound == 0) {
      throw std::out_of_range("Zero Encoder Ticks per Round are not allowed");
    }

    if(storage.areLimitsActive){

      quantity<plane_angle> lowLimit = ((double) this->storage.lowerLimit / storage.encoderTicksPerRound) * storage.gearRatio * (2.0 * M_PI) * radian;
      quantity<plane_angle> upLimit = ((double) this->storage.upperLimit / storage.encoderTicksPerRound) * storage.gearRatio * (2.0 * M_PI) * radian;
      
      if (storage.inverseMovementDirection) {
        upLimit = ((double) -(this->storage.lowerLimit) / storage.encoderTicksPerRound) * storage.gearRatio * (2.0 * M_PI) * radian;
        lowLimit = ((double) -(this->storage.upperLimit) / storage.encoderTicksPerRound) * storage.gearRatio * (2.0 * M_PI) * radian;
      }
      

      if (!((targetposition < upLimit) && (targetposition > lowLimit))) {
        std::stringstream errorMessageStream;
        errorMessageStream << "The setpoint angle is out of range. The valid range is between " << lowLimit << " and " << upLimit << " and it is: " << targetposition;
        //    LOG(trace) << "abs_value: " << abs(data.angle) << " abslow " << abs(lowLimit) << " absupper " << abs(upLimit);
        throw std::out_of_range(errorMessageStream.str());
      }
    }
    int32 targetposition_enc;

    targetposition_enc = (int32) round((targetposition.value() * ((double) storage.encoderTicksPerRound / (2.0 * M_PI))) / storage.gearRatio);


    if (storage.inverseMovementDirection) {
      targetposition_enc *= -1;
    }
    
    targetPositions.push_back(targetposition_enc); 
  }
  
  // Bouml preserved body end 000EA071
}

void YouBotJoint::parseYouBotErrorFlags(const YouBotSlaveMsg& messageBuffer) {
  // Bouml preserved body begin 00044AF1

    if (messageBuffer.stctInput.errorFlags & OVER_CURRENT) {
      LOG(warning) << jointNameString << "over current";
      //    throw JointErrorException(jointNameString + "got over current");
    }

    if (messageBuffer.stctInput.errorFlags & UNDER_VOLTAGE) {
      LOG(warning) << jointNameString << "under voltage";
      //    throw JointErrorException(jointNameString + "got under voltage");
    }

    if (messageBuffer.stctInput.errorFlags & OVER_VOLTAGE) {
      LOG(warning) << jointNameString << "over voltage";
      //   throw JointErrorException(jointNameString + "got over voltage");
    }

    if (messageBuffer.stctInput.errorFlags & OVER_TEMPERATURE) {
      LOG(warning) << jointNameString << "over temperature";
      //   throw JointErrorException(jointNameString + "got over temperature");
    }

    if (messageBuffer.stctInput.errorFlags & MOTOR_HALTED) {
      //   LOG(info) << jointNameString << "is halted";
      //   throw JointErrorException(jointNameString + "is halted");
    }

    if (messageBuffer.stctInput.errorFlags & HALL_SENSOR_ERROR) {
      LOG(warning) << jointNameString << "hall sensor problem";
      //   throw JointErrorException(jointNameString + "got hall sensor problem");
    }

//    if (messageBuffer.stctInput.errorFlags & ENCODER_ERROR) {
//      LOG(warning) << jointNameString << "encoder problem";
//      //   throw JointErrorException(jointNameString + "got encoder problem");
//    }
//
//     if (messageBuffer.stctInput.errorFlags & INITIALIZATION_ERROR) {
//      LOG(warning) << jointNameString << "initialization problem";
//      //   throw JointErrorException(jointNameString + "got motor winding problem");
//    }

    if (messageBuffer.stctInput.errorFlags & PWM_MODE_ACTIVE) {
   //   LOG(info) << jointNameString << "has PWM mode active";
      //   throw JointErrorException(jointNameString + "the cycle time is violated");
    }

    if (messageBuffer.stctInput.errorFlags & VELOCITY_MODE) {
   //   LOG(info) << jointNameString << "has velocity mode active";
      //   throw JointErrorException(jointNameString + "need to initialize the sinus commutation");
    }

    if (messageBuffer.stctInput.errorFlags & POSITION_MODE) {
   //   LOG(info) << jointNameString << "has position mode active";
      //   throw JointErrorException(jointNameString + "need to initialize the sinus commutation");
    }

    if (messageBuffer.stctInput.errorFlags & TORQUE_MODE) {
   //   LOG(info) << jointNameString << "has torque mode active";
      //   throw JointErrorException(jointNameString + "need to initialize the sinus commutation");
    }

//    if (messageBuffer.stctInput.errorFlags & EMERGENCY_STOP) {
//      LOG(info) << jointNameString << "emergency stop active";
//      //   throw JointErrorException(jointNameString + "need to initialize the sinus commutation");
//    }
//
//    if (messageBuffer.stctInput.errorFlags & FREERUNNING) {
//   //   LOG(info) << jointNameString << "has freerunning active";
//      //   throw JointErrorException(jointNameString + "need to initialize the sinus commutation");
//    }

    if (messageBuffer.stctInput.errorFlags & POSITION_REACHED) {
  //    LOG(info) << jointNameString << "has position reached";
      //   throw JointErrorException(jointNameString + "need to initialize the sinus commutation");
    }

    if (!(messageBuffer.stctInput.errorFlags & INITIALIZED)) {
      LOG(warning) << jointNameString << "not initialized";
      //   throw JointErrorException(jointNameString + "need to initialize the sinus commutation");
    }

    if (messageBuffer.stctInput.errorFlags & TIMEOUT) {
      LOG(warning) << jointNameString << "exceeded timeout";
      //   throw JointErrorException(jointNameString + "need to initialize the sinus commutation");
    }

    if (messageBuffer.stctInput.errorFlags & I2T_EXCEEDED) {
      LOG(warning) << jointNameString << "exceeded I2t";
      //   throw JointErrorException(jointNameString + "need to initialize the sinus commutation");
    }

  // Bouml preserved body end 00044AF1
}

void YouBotJoint::parseMailboxStatusFlags(const YouBotSlaveMailboxMsg& mailboxMsg) {
  // Bouml preserved body begin 00075BF1

    switch(mailboxMsg.stctInput.status){
      case NO_ERROR:
        break;
      case INVALID_COMMAND:
        LOG(error) << jointNameString << "Parameter name: " << mailboxMsg.parameterName << "; Command no: " << mailboxMsg.stctOutput.commandNumber << " is an invalid command!" ;
      //    throw JointParameterException(jointNameString + "invalid command");
        break;
      case WRONG_TYPE:
        LOG(error) << jointNameString << "Parameter name: " << mailboxMsg.parameterName << " has a wrong type!";
      //    throw JointParameterException(jointNameString + "wrong type");
        break;
      case INVALID_VALUE:
        LOG(error) << jointNameString << "Parameter name: " << mailboxMsg.parameterName << " Value: " << mailboxMsg.stctOutput.value << " is a invalid value!";
      //    throw JointParameterException(jointNameString + "invalid value");
        break;
      case CONFIGURATION_EEPROM_LOCKED:
        LOG(error) << jointNameString << "Parameter name: " << mailboxMsg.parameterName << " - Configuration EEPROM locked";
      //    throw JointParameterException(jointNameString + "configuration EEPROM locked");
        break;
      case COMMAND_NOT_AVAILABLE:
        LOG(error) << jointNameString << "Parameter name: " << mailboxMsg.parameterName << " - Command is not available!";
      //    throw JointParameterException(jointNameString + "command not available");
        break;
      case REPLY_WRITE_PROTECTED:
        LOG(error) << jointNameString << "Parameter name: " << mailboxMsg.parameterName << " - Permissions denied!";
      //    throw JointParameterException(jointNameString + "command not available");
        break;
    }
   

  // Bouml preserved body end 00075BF1
}

bool YouBotJoint::retrieveValueFromMotorContoller(YouBotSlaveMailboxMsg& message) {
  // Bouml preserved body begin 0005BD71

    bool unvalid = true;
    unsigned int retry = 0;

    ethercatMaster->setMailboxMsgBuffer(message, this->jointNumber);

    SLEEP_MILLISEC(timeTillNextMailboxUpdate);

    do {
      ethercatMaster->getMailboxMsgBuffer(message, this->jointNumber);
    /*     LOG(trace) << "CommandNumber " << (int) message.stctInput.commandNumber
                 << " moduleAddress " << (int) message.stctInput.moduleAddress
                 << " replyAddress " << (int) message.stctInput.replyAddress
                 << " status " << (int) message.stctInput.status
                 << " value " << message.stctInput.value; */
       
      if (message.stctOutput.commandNumber == message.stctInput.commandNumber &&
              message.stctInput.status == NO_ERROR) {
        unvalid = false;
      } else {
        SLEEP_MILLISEC(timeTillNextMailboxUpdate);
        retry++;
      }
    } while (retry < mailboxMsgRetries && unvalid);

    if (unvalid) {
      this->parseMailboxStatusFlags(message);
      return false;
    } else {
      return true;
    }

  // Bouml preserved body end 0005BD71
}

bool YouBotJoint::setValueToMotorContoller(const YouBotSlaveMailboxMsg& mailboxMsg) {
  // Bouml preserved body begin 00054AF1

    YouBotSlaveMailboxMsg mailboxMsgBuffer;
    mailboxMsgBuffer = mailboxMsg;
    bool unvalid = true;
    unsigned int retry = 0;

    ethercatMaster->setMailboxMsgBuffer(mailboxMsgBuffer, this->jointNumber);

    SLEEP_MILLISEC(timeTillNextMailboxUpdate);

    do {
      ethercatMaster->getMailboxMsgBuffer(mailboxMsgBuffer, this->jointNumber);
      /*    LOG(trace) << "CommandNumber " << (int) mailboxMsgBuffer.stctInput.commandNumber
                  << " moduleAddress " << (int) mailboxMsgBuffer.stctInput.moduleAddress
                  << " replyAddress " << (int) mailboxMsgBuffer.stctInput.replyAddress
                  << " status " << (int) mailboxMsgBuffer.stctInput.status
                  << " value " << mailboxMsgBuffer.stctInput.value;
       */
      if (mailboxMsgBuffer.stctOutput.commandNumber == mailboxMsgBuffer.stctInput.commandNumber &&
              mailboxMsgBuffer.stctOutput.value == mailboxMsgBuffer.stctInput.value &&
              mailboxMsgBuffer.stctInput.status == NO_ERROR) {
        unvalid = false;
      } else {
        SLEEP_MILLISEC(timeTillNextMailboxUpdate);
        retry++;
      }
    } while (retry < mailboxMsgRetries && unvalid);

    if (unvalid) {
      this->parseMailboxStatusFlags(mailboxMsgBuffer);
      return false;
    } else {
      return true;
    }

  // Bouml preserved body end 00054AF1
}


} // namespace youbot
