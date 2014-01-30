#ifndef SAPI_EVENT_LISTENER_H_
#define SAPI_EVENT_LISTENER_H_

#include <sapi.h>
#include <windows.h>
#include "sphelper.h"
#include <string>
#include "mechio/messaging/MessageSender.h"
#include "mechio/speech/protocol/SpeechEventListRecord.h"

namespace mechio {
  namespace speech { 
   
	enum {SP_START=0, SP_STOP=1, SP_TERMINATE=2, SP_MAX_LISTEN_EVENTS=3};
	struct SAPIListenData {
		HANDLE m_eventListen[SP_MAX_LISTEN_EVENTS];
		ISpVoice* m_voice;
		MessageSender* m_sender;
	};

	class SAPIEventListener {
		public:
			SAPIEventListener(SAPIListenData* data);
			~SAPIEventListener();

		private:
			static DWORD runListener(void *params);
			static SpeechEventListRecord fetchEvents(CSpEvent &spEvent, ISpVoice *voice);
			static SpeechEventRecord fetchSpeechEventRecord(CSpEvent &spEvent);
			static SpeechEventRecord speechStartEvent(CSpEvent &spEvent);
			static SpeechEventRecord speechEndEvent(CSpEvent &spEvent);
			static SpeechEventRecord bookmarkEvent(CSpEvent &spEvent);
			static SpeechEventRecord visemeEvent(CSpEvent &spEvent);
			static SpeechEventRecord phonemeEvent(CSpEvent &spEvent);
			static SpeechEventRecord sentenceBoundaryEvent(CSpEvent &spEvent);
			static SpeechEventRecord wordBoundaryEvent(CSpEvent &spEvent);
			static SpeechEventRecord unknownEvent(CSpEvent &spEvent);

			HANDLE m_listenThread;

	};
}}
#endif