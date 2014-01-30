#ifndef SAPI_ENGINE_H_
#define SAPI_ENGINE_H_

#include <sapi.h>
#include <windows.h>
#include "sphelper.h"
#include <string>
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

			unsigned long speak(const std::string& phrase);
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
	};
	static SAPIEngine* s_SAPIEngineInstance;
}}
#endif
