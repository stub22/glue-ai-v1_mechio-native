#ifndef SPEECH_EVENT_LISTENER_H_
#define SPEECH_EVENT_LISTENER_H_

 
namespace mechio {
    namespace speech { 
   
	enum {START=0, STOP=1, TERMINATE=2, MAX_LISTEN_EVENTS=3};
	struct SAPIListenData {
		HANDLE m_eventListen[MAX_LISTEN_EVENTS];
		ISpVoice* m_voice;
		SAPIObserver* m_observer;
	};

	class SAPIEventListener {
		public:
			SAPIEventListener(SAPIListenData* data);
			~SAPIEventListener();

		private:
			static DWORD runListener(void *params);
			static void fetchEvents(SAPIObserver* observer, CSpEvent &spEvent, ISpVoice *voice);
			HANDLE m_listenThread;
	};
}}
#endif