
#include "stdafx.h"
#include "JNISAPIEngine.h"
#include "../JNIUtilities/JNIClass.h"
#include "../JNIUtilities/JNIUtilities.h"
#include <iostream>
#include "../SAPIEngine/SAPIEngine.h"

using org::mechio::jni::JNIClass;
using org::mechio::speech::SAPIEngine;

JNIEXPORT void JNICALL Java_org_mechio_speech_SAPI51_NativeSAPIEngine_createEngine(JNIEnv * env, jobject jobj){
	JNIClass jcls(env, jobj);
	jfieldID fid = jcls.getFieldId("m_ptr", "J");

	if ( fid == NULL )
	{
		return;
	}
	SAPIEngine* addr = SAPIEngine::instance();
	env->SetLongField(jobj, fid, reinterpret_cast<jlong>(addr));
	addr->Listen();
}

JNIEXPORT void JNICALL Java_org_mechio_speech_SAPI51_NativeSAPIEngine_shutdownEngine(JNIEnv * env, jobject jobj){
	SAPIEngine* self = org::mechio::jni::getPointer<SAPIEngine>(env, jobj);
	self->ShutDown();
	// Now a singleton so don't do this here
	//delete self;
}

JNIEXPORT jlong JNICALL Java_org_mechio_speech_SAPI51_NativeSAPIEngine_speakNative(JNIEnv * env, jobject jobj, jstring toSpeak){
	SAPIEngine* self = org::mechio::jni::getPointer<SAPIEngine>(env, jobj);
	char* phrase = org::mechio::jni::JNU_GetStringNativeChars(env, toSpeak);
	jlong result = (jlong)self->Speak(phrase);

	delete[] phrase;
	return result;
}

JNIEXPORT jlong JNICALL Java_org_mechio_speech_SAPI51_NativeSAPIEngine_cancelSpeechNative(JNIEnv *env, jobject jobj) {
	SAPIEngine* self = org::mechio::jni::getPointer<SAPIEngine>(env, jobj);

	return (jlong)self->CancelSpeech();
}
