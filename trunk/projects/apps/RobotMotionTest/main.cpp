/* 
 * File:   main.cpp
 * Author: matt
 *
 * Created on December 9, 2011, 10:48 AM
 */

#include <cstdlib>
#include "mechio/motion/RobotProxy.h"

using namespace std;
using namespace mechio;

const int WAIST = 100;
    
const int NECK_YAW = 200;
const int NECK_ROLL = 201;
const int NECK_PITCH = 202;

const int BROWS = 300;
const int EYELIDS = 301;
const int EYES_PITCH = 310;
const int LEFT_EYE_YAW = 311;
const int RIGHT_EYE_YAW = 312;
const int LEFT_SMILE = 320;
const int RIGHT_SMILE = 321;
const int JAW = 322;

const int LEFT_SHOULDER_PITCH = 400;
const int LEFT_SHOULDER_ROLL = 401;
const int LEFT_ELBOW_YAW = 410;
const int LEFT_ELBOW_PITCH = 411;

const int RIGHT_SHOULDER_PITCH = 500;
const int RIGHT_SHOULDER_ROLL = 501;
const int RIGHT_ELBOW_YAW = 510;
const int RIGHT_ELBOW_PITCH = 511;

const int LEFT_HIP_ROLL = 600;
const int LEFT_HIP_YAW = 601;
const int LEFT_HIP_PITCH = 602;
const int LEFT_KNEE_PITCH = 610;
const int LEFT_ANKLE_PITCH = 620;
const int LEFT_ANKLE_ROLL = 621;

const int RIGHT_HIP_ROLL = 700;
const int RIGHT_HIP_YAW = 701;
const int RIGHT_HIP_PITCH = 702;
const int RIGHT_KNEE_PITCH = 710;
const int RIGHT_ANKLE_PITCH = 720;
const int RIGHT_ANKLE_ROLL = 721;

int main(int argc, char** argv) {
    RobotProxy robot("source", "destination", "Avatar_ZenoR50", "localhost:5672");
    robot.initializeSender("robotAvatarZenoR50hostmotionFrame; {create: always, node: {type: topic}}");

    robot.setGoalPosition(NECK_YAW, 0.5);
    robot.setGoalPosition(NECK_PITCH, 0.5);
    robot.moveAndWait(1000);

    robot.setGoalPosition(NECK_YAW, 0.2);
    robot.setGoalPosition(NECK_PITCH, 0.1);
    robot.moveAndWait(1000);

    robot.setGoalPosition(NECK_YAW, 0.7);
    robot.setGoalPosition(NECK_PITCH, 0.3);
    robot.moveAndWait(1000);
    
    robot.setGoalPosition(NECK_YAW, 0.5);
    robot.setGoalPosition(NECK_PITCH, 0.5);
    robot.moveAndWait(1000);

    robot.disconnect();
    return 0;
}

