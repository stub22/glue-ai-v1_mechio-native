#include "stdafx.h"
#include "JNIClass.h"

using namespace org::mechio::jni;

JNIClass::JNIClass(JNIEnv* env, jobject& obj) : m_env(env), m_obj(obj){
  m_cls = env->GetObjectClass(obj);
}
JNIClass::~JNIClass(){}

jfieldID JNIClass::getFieldId(const char* name, const char* type){
  return m_env->GetFieldID(m_cls, name, type);
}

jmethodID JNIClass::getMethodId(const char* name, const char* type){
  return m_env->GetMethodID(m_cls, name, type);
}
