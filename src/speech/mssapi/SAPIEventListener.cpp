#include <iostream>
#include <vector>
#include <exception>
#include "mechio/speech/mssapi/SAPIEventListener.h"

using namespace mechio;
using namespace mechio::speech;

SAPIEventListener::SAPIEventListener(SAPIListenData* data){
	m_listenThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)runListener,(LPVOID)data,0,0);
}

DWORD SAPIEventListener::runListener(void *params){
	ISpVoice* voice = ((SAPIListenData*)params)->m_voice;
	if(FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED))){
		throw std::exception("CoInitializeEx() failed");
	}
	if(FAILED(voice->SetNotifyWin32Event())){
		throw std::exception("Could not configure sapi to use win32 event notification");
	}

	ULONGLONG interestingEvents = SPFEI(SPEI_START_INPUT_STREAM) | SPFEI(SPEI_END_INPUT_STREAM) 
		| SPFEI(SPEI_TTS_BOOKMARK) | SPFEI(SPEI_VISEME) | SPFEI(SPEI_WORD_BOUNDARY)
		| SPFEI(SPEI_PHONEME) | SPFEI(SPEI_SENTENCE_BOUNDARY);
	if(FAILED(voice->SetInterest(interestingEvents, interestingEvents))){
		throw std::exception("Could not set interest in sapi events");
	}
	
	MessageSender* sender = ((SAPIListenData*)params)->m_sender;
	HANDLE eventStart = ((SAPIListenData*)params)->m_eventListen[SP_START];
	HANDLE eventStop = ((SAPIListenData*)params)->m_eventListen[SP_STOP];
	HANDLE eventTerminate = ((SAPIListenData*)params)->m_eventListen[SP_TERMINATE];
	HANDLE sapiEvent = voice->GetNotifyEventHandle();
	HANDLE doneEvent = voice->SpeakCompleteEvent();
	HANDLE events[5] = {eventStart, eventStop, eventTerminate, sapiEvent, doneEvent};
	CSpEvent spEvent;
	bool silent = false;
	while(1){
		DWORD hr = WaitForMultipleObjectsEx(5, events, FALSE, INFINITE, FALSE);
		if(hr == WAIT_OBJECT_0){//START
			silent = false;
			ResetEvent(eventStart);
		}else if(hr == (WAIT_OBJECT_0 + 1)){//STOP
			silent = true;
			ResetEvent(eventStop);
		}else if(hr == (WAIT_OBJECT_0 + 2)){//TERMINATE
			ResetEvent(eventTerminate);
			break;
		}else if(hr == (WAIT_OBJECT_0 + 3)){//SAPI EVENT
			if(!silent){
				SpeechEventListRecord eventList = fetchEvents(spEvent, voice);
				sender->send<SpeechEventListRecord>(&eventList);
			}else{
				//flush out events
				while(spEvent.GetFrom(voice) == S_OK);
			}
			ResetEvent(sapiEvent);
		}else if(hr == (WAIT_OBJECT_0 + 4)){//DONE SPEAKING
			ResetEvent(doneEvent);
		}
	}
	CoUninitialize();
	return S_OK;
}

SpeechEventListRecord SAPIEventListener::fetchEvents(CSpEvent &spEvent, ISpVoice *voice){
	SpeechEventListRecord list;
	list.speechServiceId ="MSSAPI";
	list.timestampMillisecUTC = static_cast<long>(GetTickCount());
	HRESULT eventFetchResult = S_OK;
	while(spEvent.GetFrom(voice) == S_OK){
		SpeechEventRecord eventRec = fetchSpeechEventRecord(spEvent);
		list.speechEvents.push_back(eventRec);
	}
	return list;
}

SpeechEventRecord SAPIEventListener::fetchSpeechEventRecord(CSpEvent &spEvent){
	switch(spEvent.eEventId){
		case SPEI_START_INPUT_STREAM: return speechStartEvent(spEvent);
		case SPEI_END_INPUT_STREAM:return speechEndEvent(spEvent);
		case SPEI_TTS_BOOKMARK: return bookmarkEvent(spEvent);
		case SPEI_VISEME: return visemeEvent(spEvent);
		case SPEI_PHONEME: return phonemeEvent(spEvent);
		case SPEI_SENTENCE_BOUNDARY: return sentenceBoundaryEvent(spEvent);
		case SPEI_WORD_BOUNDARY: return wordBoundaryEvent(spEvent);
		default: return unknownEvent(spEvent);
	}
}

SpeechEventRecord SAPIEventListener::speechStartEvent(CSpEvent &spEvent){
	SpeechEventRecord eventRec;
	eventRec.eventType = "SPEECH_START";
	eventRec.streamNumber = spEvent.ulStreamNum;
	return eventRec;
}

SpeechEventRecord SAPIEventListener::speechEndEvent(CSpEvent &spEvent){
	SpeechEventRecord eventRec;
	eventRec.eventType = "SPEECH_END";
	eventRec.streamNumber = spEvent.ulStreamNum;
	return eventRec;
}

static std::string toString(const wchar_t* str){
	int len = wcslen(str);
	char* chars = (char*)new char[len];
	for(int i=0; i<len; i++){
		chars[i] = str[i];
	}
	std::string nstr(chars);
	return nstr;
}

SpeechEventRecord SAPIEventListener::bookmarkEvent(CSpEvent &spEvent){
	SpeechEventRecord eventRec;
	eventRec.eventType = "BOOKMARK";
	eventRec.streamNumber = spEvent.ulStreamNum;
	eventRec.stringData = toString(spEvent.BookmarkName());
	return eventRec;
}

SpeechEventRecord SAPIEventListener::visemeEvent(CSpEvent &spEvent){
	SpeechEventRecord eventRec;
	eventRec.eventType = "VISEME";
	eventRec.streamNumber = spEvent.ulStreamNum;
	eventRec.currentData = spEvent.Viseme();
	eventRec.nextData = (SPVISEMES)(spEvent.wParam & 0x00FF);
	eventRec.duration = static_cast<unsigned int>(spEvent.wParam >> 16);
	return eventRec;
}

SpeechEventRecord SAPIEventListener::phonemeEvent(CSpEvent &spEvent){
	SpeechEventRecord eventRec;
	eventRec.eventType = "PHONEME";
	eventRec.streamNumber = spEvent.ulStreamNum;
	eventRec.currentData = spEvent.Phoneme();
	eventRec.nextData = (SPPHONEID)(spEvent.wParam & 0x00FF);
	eventRec.duration = static_cast<unsigned int>(spEvent.wParam >> 16);
	return eventRec;
}

SpeechEventRecord SAPIEventListener::sentenceBoundaryEvent(CSpEvent &spEvent){
	SpeechEventRecord eventRec;
	eventRec.eventType = "SENTENCE_BOUNDARY";
	eventRec.streamNumber = spEvent.ulStreamNum;
	eventRec.textPosition = spEvent.InputSentPos();
	eventRec.textLength = spEvent.InputSentLen();
	return eventRec;
}

SpeechEventRecord SAPIEventListener::wordBoundaryEvent(CSpEvent &spEvent){
	SpeechEventRecord eventRec;
	eventRec.eventType = "WORD_BOUNDARY";
	eventRec.streamNumber = spEvent.ulStreamNum;
	eventRec.textPosition = spEvent.InputWordPos();
	eventRec.textLength = spEvent.InputWordLen();
	return eventRec;
}

SpeechEventRecord SAPIEventListener::unknownEvent(CSpEvent &spEvent){
	SpeechEventRecord eventRec;
	eventRec.eventType = "UNKNOWN";
	eventRec.streamNumber = spEvent.ulStreamNum;
	return eventRec;
}