#include "mechio/motion/MotionFrameSender.h"

using namespace mechio;

MotionFrameSender::MotionFrameSender(MessageSender* sender, std::string sourceId, std::string destinationId){
    myMotionFrameSender = sender;
	mySourceId.assign(sourceId);
	myDestinationId.assign(destinationId);
}
void MotionFrameSender::send(MotionFrameRecord &motionFrame){
	MotionFrameEventRecord evnt;
	evnt.currentTimeMillisecUTC = 1;
	evnt.sourceId.assign("source");
	evnt.destinationId.assign("destination");
	evnt.motionFrame = motionFrame;
	evnt.motionFrame.timestampMillisecUTC = 1;
	evnt.motionFrame.startPositions.set_null();
	myMotionFrameSender->send(&evnt);
}