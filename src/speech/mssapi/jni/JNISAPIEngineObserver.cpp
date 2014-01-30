#include "stdafx.h"
#include "JNISAPIEngineObserver.h"

std::string s_methodId[MAX_EVENTS][2] = {
    {"startInputStreamEvent", "(J)V"},
    {"endInputStreamEvent", "(J)V"},
    {"bookMarkEvent", "(JLjava/lang/String;)V"},
    {"visemeEvent", "(JIIBI)V"},
    {"phonemeEvent", "(JIIBI)V"},
    {"sentenceBoundaryEvent", "(JII)V"},
    {"wordBoundaryEvent", "(JII)V"}
};

JavaTTSEngineObserver::JavaTTSEngineObserver(JNIEnv* env, jobject jobj) : JavaObserver(env, jobj) {
    for(int i=0; i<MAX_EVENTS; i++){
        m_eventHandler[i] = NULL;
    }
}
JavaTTSEngineObserver::~JavaTTSEngineObserver(){
    for(int i=0; i<MAX_EVENTS; i++){
        m_eventHandler[i] = NULL;
    }
}

jmethodID JavaTTSEngineObserver::getMethod(int id){
    if(m_eventHandler[id] == NULL){
		m_eventHandler[id] = getClass().getMethodId(s_methodId[id][0].c_str(), s_methodId[id][1].c_str());
	}
	return m_eventHandler[id];
}

void JavaTTSEngineObserver::visitStartInput(long streamNumber) {
	//this->attach();
    if(getMethod(START_INPUT) != NULL){
        getCurrentEnv()->CallVoidMethod(getObject(), m_eventHandler[START_INPUT], streamNumber);
    }
	//this->detach();
}
void JavaTTSEngineObserver::visitEndInput(long streamNumber) {
	//this->attach();
    if(getMethod(END_INPUT) != NULL){
        getCurrentEnv()->CallVoidMethod(getObject(), m_eventHandler[END_INPUT], streamNumber);
    }
	//this->detach();
}
void JavaTTSEngineObserver::visitBookmark(long streamNumber, const WCHAR* mark){
	//this->attach();
    if(getMethod(BOOKMARK) == NULL){
	    jobject jname = org::mechio::jni::JNU_NewStringNative(getCurrentEnv(), mark);
	    if(jname != NULL){
            getCurrentEnv()->CallVoidMethod(getObject(), m_eventHandler[BOOKMARK], (jlong)streamNumber, jname);
	    }
    }
	//this->detach();
}
void JavaTTSEngineObserver::visitViseme(long streamNumber, int curViseme, int duration, char features, int nextViseme){
	//this->attach();
    if(getMethod(VISEME) != NULL){
        getCurrentEnv()->CallVoidMethod(getObject(), m_eventHandler[VISEME], (jlong)streamNumber,
	        (jint)curViseme, (jint)duration, (jbyte)features, (jint)nextViseme);
    }
	//this->detach();
}
void JavaTTSEngineObserver::visitPhoneme(long streamNumber, int curPhoneme, int duration, char features, int nextPhoneme){
	//this->attach();
    if(getMethod(PHONEME) != NULL){
        getCurrentEnv()->CallVoidMethod(getObject(), m_eventHandler[PHONEME], (jlong)streamNumber,
	        (jint)curPhoneme, (jint)duration, (jbyte)features, (jint)nextPhoneme);
    }
	//this->detach();
}

void JavaTTSEngineObserver::visitSentenceBoundary(long streamNumber, int position, int length){
	//this->attach();
    if(getMethod(SENTENCE_BOUND) != NULL){
        getCurrentEnv()->CallVoidMethod(getObject(), m_eventHandler[SENTENCE_BOUND], 
            (jlong)streamNumber, (jint)position, (jint)length);
    }
	//this->detach();
}
void JavaTTSEngineObserver::visitWordBoundary(long streamNumber, int position, int length){
	//this->attach();
    if(getMethod(WORD_BOUND) != NULL){
        getCurrentEnv()->CallVoidMethod(getObject(), m_eventHandler[WORD_BOUND], 
            (jlong)streamNumber, (jint)position, (jint)length);
    }
	//this->detach();
}

void JavaTTSEngineObserver::attachObserver(){
	this->attach();
}

void JavaTTSEngineObserver::detachObserver(){
	this->detach();
}

JNIEXPORT void JNICALL Java_org_mechio_speech_SAPI51_NativeSAPIEngine_addObserverNative(JNIEnv * env, jobject jobj, jobject obs){
	SAPIEngine* self = org::mechio::jni::getPointer<SAPIEngine>(env, jobj);
	JavaTTSEngineObserver *observer = new JavaTTSEngineObserver(env, obs);
	self->registerCallback(observer);
}
