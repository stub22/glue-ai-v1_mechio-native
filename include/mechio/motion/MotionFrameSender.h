/* 
 * File:   MotionFrameSender.h
 * Author: matt
 *
 * Created on July 15, 2011, 4:17 AM
 */

#ifndef MOTIONFRAMESENDER_H
#define	MOTIONFRAMESENDER_H

#include <string.h>
#include "mechio/messaging/MessageSender.h"
#include "mechio/motion/protocol/MotionFrameEventRecord.h"

namespace mechio{
    class MotionFrameSender  {
        private:
            MessageSender* myMotionFrameSender;
			std::string mySourceId;
			std::string myDestinationId;
        public:
			MotionFrameSender(MessageSender* sender, std::string sourceId, std::string destinationId);
			void send(MotionFrameRecord &motionFrame);
    };
}

#endif	/* MOTIONFRAMESENDER_H */

