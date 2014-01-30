#include "stdafx.h"
#include "JNIWrapperManager.h"
#include <iostream>
using namespace org::mechio::jni;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved){
	JNIWrapperManager* jniMgr = JNIWrapperManager::instance();
	jniMgr->startup(vm);

	return JNI_VERSION_1_6;
}

namespace org {
  namespace mechio {
    namespace jni {

	void JNIWrapperManager::startup(JavaVM* vm){
	  m_vm = vm;
	}

	JavaVM* JNIWrapperManager::getJavaVM() const {
	  return m_vm;
	}

	JNIWrapperManager* JNIWrapperManager::instance(){
		if(s_JNIWrapperManagerInstance == NULL){
			s_JNIWrapperManagerInstance = new JNIWrapperManager();
		}
		return s_JNIWrapperManagerInstance;
	}
}}}
