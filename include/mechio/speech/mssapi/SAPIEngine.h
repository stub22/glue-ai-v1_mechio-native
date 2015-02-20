#ifndef SAPI_ENGINE_H_
#define SAPI_ENGINE_H_

#include <sapi.h>
#include <windows.h>
#include "sphelper.h"
#include <string>
#include <queue>
#include <boost/regex.hpp>
#include "mechio/speech/SpeechEngine.h"
#include "SAPIEventListener.h"

namespace mechio {
  namespace speech { 

		class SAPIEngine : public SpeechEngine {
		public:
			SAPIEngine();
			~SAPIEngine();

			static SAPIEngine* instance();
			
			bool initialize(std::string voiceName, double sampleRate);
			bool isInitialized();
			void stop();

			unsigned long speak(SpeechRequestRecord* request);
			unsigned long cancelSpeech();

            void setEventSender(MessageSender* eventSender);
			void startEventListener();
			bool isListening();
			void stopEventListener();

			bool changeVoice(std::string voiceName);
			bool changeSampleRate(int sampleRate);

		private:
			HANDLE m_eventListen[3];
			ISpVoice* m_voice;
			SAPIEventListener* m_listener;
			bool m_initialized;
			bool m_listening;
			std::queue<int64_t> m_reqQueue;
			boost::regex* m_reg;
	};
	static SAPIEngine* s_SAPIEngineInstance;
}}
#endif
