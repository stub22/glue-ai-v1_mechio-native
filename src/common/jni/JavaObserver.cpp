#include "stdafx.h"
#include "JNIWrapperManager.h"
#include "JavaObserver.h"
#include <iostream>

using namespace org::mechio::jni;

JavaObserver::JavaObserver(JNIEnv* env, jobject obj) : m_NeedsDetached(false), m_env(env){
	// Tell the JVM that we are holding a durable ref to this JVM object in native code.
	m_Callback = env->NewGlobalRef(obj);
}

JavaObserver::~JavaObserver(){
	// Let the JVM know we are done with the ref.
	m_env->DeleteGlobalRef(m_Callback);
}

// Must attach C++ thread to JVM before we can call the JVM.
void JavaObserver::attach(){
  m_env = 0;
  JNIWrapperManager * jniMgr = JNIWrapperManager::instance();
  jint ret = jniMgr->getJavaVM()->GetEnv((void**)&m_env, JNI_VERSION_1_6);
 
  if(ret == JNI_EDETACHED){
    m_NeedsDetached = true;
    jniMgr->getJavaVM()->AttachCurrentThread((void**)&m_env, 0);
  }
}
  
void JavaObserver::detach(){
	JNIWrapperManager *jniMgr = JNIWrapperManager::instance();

	if(m_NeedsDetached){
		jniMgr->getJavaVM()->DetachCurrentThread();
	}
	m_env = 0;
}
