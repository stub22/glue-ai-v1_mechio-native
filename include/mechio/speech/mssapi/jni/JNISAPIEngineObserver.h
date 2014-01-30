#include "JNISAPIEngine.h"

#include "../JNIUtilities/JavaObserver.h"
#include "../JNIUtilities/JNIUtilities.h"
#include "../SAPIEngine/SAPIEngine.h"
#include <iostream>

using org::mechio::jni::JNIClass;
using org::mechio::jni::JavaObserver;
using org::mechio::speech::SAPIEngine;
using org::mechio::speech::SAPIObserver;

enum {START_INPUT=0, END_INPUT=1, BOOKMARK=2, VISEME=3, PHONEME=4, SENTENCE_BOUND=5, WORD_BOUND=6, MAX_EVENTS=7};

class JavaTTSEngineObserver : public JavaObserver, public SAPIObserver{

	public:
		JavaTTSEngineObserver(JNIEnv* env, jobject jobj);
		~JavaTTSEngineObserver();
        
        void visitStartInput(long streamNumber);
        void visitEndInput(long streamNumber); 
        void visitBookmark(long streamNumber, const WCHAR* mark);
        void visitViseme(long streamNumber, int curViseme, int duration, char features, int nextViseme);
        void visitPhoneme(long streamNumber, int curPhoneme, int duration, char features, int nextPhoneme);
        void visitSentenceBoundary(long streamNumber, int position, int length);
        void visitWordBoundary(long streamNumber, int position, int length);
        
		void attachObserver();
		void detachObserver();
	private:
	    jmethodID getMethod(int id);
    	jmethodID m_eventHandler[MAX_EVENTS];
};
