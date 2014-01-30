#include "mechio/motion/RobotProxy.h"
#include <boost/thread.hpp>

using namespace mechio;

RobotProxy::RobotProxy(MessagingProvider* stream, std::string sourceId, std::string destinationId, std::string robotId){
    myStream = stream;
	mySourceId = sourceId;
	myDestinationId = destinationId;
	myRobotId = robotId;
	myMotionFrameRecord = new MotionFrameRecord();
}
RobotProxy::RobotProxy(std::string sourceId, std::string destinationId, std::string robotId, const char* brokerAddress, const char* brokerOptions){
	myStream = new MessagingProvider(brokerAddress, brokerOptions);
	mySourceId.assign(sourceId);
	myDestinationId.assign(destinationId);
	myRobotId.assign(robotId);
	myMotionFrameRecord = new MotionFrameRecord();
	myStream->connect();
}
void RobotProxy::setGoalPosition(int jointId, double position){
	JointIdRecord ji;
	//std::vector<JointPositionRecord > myJPs = myMotionFrameRecord->goalPositions.jointPositions;
	ji.robotId = myRobotId;
	ji.jointId = jointId;
	JointPositionRecord jp;
	jp.jointId = ji;
	jp.normalizedPosition = position;
	/*if(!myJPs.empty()){
		int size = (int) myJPs.size();
		JointPositionRecord jpr;
		for(int i = 0; i<size; i++){
			jpr = myJPs[i];
			if(jpr.jointId.jointId == jointId && jpr.jointId.robotId.compare(myRobotId)==0){
				myJPs[i] = jp;
                return;
            }
		}
	}*/
    myMotionFrameRecord->goalPositions.jointPositions.push_back(jp);
}

void RobotProxy::move(int moveDuration){
	myMotionFrameRecord->moveDurationMillisec = moveDuration;
	myMotionFrameRecord->timestampMillisecUTC = 1;
	myMotionFrameSender->send(*myMotionFrameRecord);
}

void RobotProxy::moveAndWait(int moveDuration){
	move(moveDuration);
	boost::this_thread::sleep(boost::posix_time::milliseconds(moveDuration));
}
void RobotProxy::clearGoalPositions(){
	myMotionFrameRecord = new MotionFrameRecord();
}
void RobotProxy::initializeSender(const char* destination){
	MessageSender* sender = myStream->createSender(destination);
	myMotionFrameSender = new MotionFrameSender(sender, mySourceId, myDestinationId);
}
void RobotProxy::setSourceId(std::string sourceId){
	mySourceId = sourceId;
}
void RobotProxy::setDestinationId(std::string destinationId){
	myDestinationId = destinationId;
}
void RobotProxy::setRobotId(std::string robotId){
	myRobotId = robotId;
}
void RobotProxy::disconnect(){
	myStream->disconnect();
}