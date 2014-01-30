/* 
 * File:   RobotProxy.h
 * Author: matt
 *
 * Created on July 15, 2011, 4:17 AM
 */

#ifndef ROBOTPROXY_H
#define	ROBOTPROXY_H

#include <string.h>
#include "mechio/messaging/MessageSender.h"
#include "mechio/messaging/MessagingProvider.h"
#include "mechio/motion/protocol/MotionFrameEventRecord.h"
#include "mechio/motion/MotionFrameSender.h"

namespace mechio{
    class RobotProxy  {
        private:
            MessagingProvider* myStream;
			std::string mySourceId;
			std::string myDestinationId;
			std::string myRobotId;
			MotionFrameSender* myMotionFrameSender;
			MotionFrameRecord* myMotionFrameRecord;
        public:
			RobotProxy(std::string sourceId, std::string destinationId, std::string robotId, const char* brokerAddress="localhost:5672", const char* brokerOptions="{username:admin,password:admin}");
			RobotProxy(MessagingProvider* stream, std::string sourceId, std::string destinationId, std::string robotId);
			void setGoalPosition(int jointId, double position);
			void move(int moveDuration);
			void moveAndWait(int moveDuration);
			void clearGoalPositions();
			void initializeSender(const char* destination);
			void setSourceId(std::string sourceId);
			void setDestinationId(std::string destinationId);
			void setRobotId(std::string robotId);
			void disconnect();
    };
}

#endif	/* ROBOTPROXY_H */

