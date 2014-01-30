
#include <windows.h>
#include "sphelper.h"
#include <iostream>
#include <exception>
#include "mechio/speech/mssapi/SAPIEngine.h"

using namespace mechio;
using namespace mechio::speech;

SAPIEngine::SAPIEngine() : m_voice(0), m_listener(0), m_initialized(false), m_listening(false){
	if(FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED))){
		std::cout << "Failed initializing SapiEngine: Failed to initialize COM." << std::endl;
		return;
	}
	// Create SAPI Voice
	if(FAILED(CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&m_voice))){
		std::cout << "Failed initializing SapiEngine: CoCreateInstance() failed." << std::endl;
		return;
	}
}
SAPIEngine::~SAPIEngine(){
	// Stu notes that this Release() call is throwing an unhandled exception when
	// called during program-exit destructors from Tester.exe.
	std::cout << "About to call Release() on m_voice instance: " << m_voice << "\n";
	if (m_voice != NULL) {
		m_voice->Release();
		m_voice = NULL;
	}
	CoUninitialize();
}

bool SAPIEngine::initialize(std::string voice, double sampleRate){
	if(m_initialized)
		return true;

	
	for(int i=0; i<SP_MAX_LISTEN_EVENTS; i++){
		m_eventListen[i] = CreateEvent(NULL, TRUE, TRUE, NULL);
		ResetEvent(m_eventListen[i]);
	}


	m_initialized = true;
	return true;
}

bool SAPIEngine::isInitialized(){
	return m_initialized;
}

void SAPIEngine::stop(){
	if(!m_initialized)
		return;

	stopEventListener();
}

unsigned long SAPIEngine::speak(const std::string& phrase){
	bool xml = ((phrase.find("<sapi>") != std::string::npos) 
		|| (phrase.find("<SAPI>") != std::string::npos));
	unsigned long xmlFlag = xml ? SPF_IS_XML : SPF_IS_NOT_XML;

	WCHAR* txt = new WCHAR[phrase.size() + 2];
	std::copy(phrase.begin(), phrase.end(), txt);
	txt[phrase.size()] = 0;

	ULONG stream = 0;
	if(m_voice->Speak(txt, SPF_ASYNC | xmlFlag, &stream) != S_OK){
		std::cout << "ISpVoice->Speak() failed!";
	}
	delete[] txt;
	return stream;
}

ULONG SAPIEngine::cancelSpeech(){
	ULONG streamNumber = 0;
	if (m_voice->Speak(L"\0", SPF_ASYNC | SPF_PURGEBEFORESPEAK, &streamNumber) != S_OK){
		std::cout << "ISpVoice->Speak() failed!";
	}
	return streamNumber;
}

void SAPIEngine::setEventSender(MessageSender* eventSender){
	SAPIListenData *data = new SAPIListenData();
	data->m_voice = m_voice;
	data->m_sender = eventSender;
	for(int i=0; i<3; i++){
		data->m_eventListen[i] = m_eventListen[i];
	}
	m_listener = new SAPIEventListener(data);
}

void SAPIEngine::startEventListener(){
	if(m_listening)
		return;

	SetEvent(m_eventListen[SP_START]);
	m_listening = true;
}

void SAPIEngine::stopEventListener(){
	if(!m_listening)
		return;

	SetEvent(m_eventListen[SP_STOP]);
	m_listening = false;
}

bool SAPIEngine::isListening(){
	return m_listening;
}

SAPIEngine* SAPIEngine::instance(){
	if(s_SAPIEngineInstance == NULL){
		s_SAPIEngineInstance = new SAPIEngine();
	}
	return s_SAPIEngineInstance;
}

bool SAPIEngine::changeVoice(std::string voiceName){
	return false;
}

bool SAPIEngine::changeSampleRate(int sampleRate){
	return false;
}
