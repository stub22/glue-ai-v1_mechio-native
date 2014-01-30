#include "stdafx.h"
#include <iostream>
#include <string>

#include "JNIUtilities.h"

jobject org::mechio::jni::JNU_NewStringNative(JNIEnv *env, const wchar_t *str){
	jclass Class_java_lang_String = env->FindClass("java/lang/String");
	jmethodID MID_String_init = env->GetMethodID(Class_java_lang_String, "<init>", "([B)V");
	jobject result = NULL;
	jbyteArray bytes = 0;
	if (env->EnsureLocalCapacity(2) < 0) {
		return NULL; /* out of memory error */
	}
	jsize len = static_cast<jsize>(wcslen(str) * sizeof(wchar_t));
	bytes = env->NewByteArray(len);
	if (bytes != NULL) {
		env->SetByteArrayRegion(bytes, 0, len, (jbyte *)str);
		result = env->NewObject(Class_java_lang_String, MID_String_init, bytes);
		env->DeleteLocalRef(bytes);
	} /* else fall through */
	return result;
}

char* org::mechio::jni::JNU_GetStringNativeChars(JNIEnv *env, jstring jstr){
	jbyteArray bytes = 0;
	jthrowable exc;
	char* result = 0;
	if (env->EnsureLocalCapacity(2) < 0) {
		return 0; /* out of memory error */
	}
	jclass Class_java_lang_String = env->FindClass("java/lang/String");
	jmethodID MID_String_getBytes = env->GetMethodID(Class_java_lang_String, "getBytes", "()[B");

	bytes = (jbyteArray)env->CallObjectMethod(jstr, MID_String_getBytes);
	exc = env->ExceptionOccurred();
	if (!exc) {
		jint len = env->GetArrayLength(bytes);
		result = new char[len + 1];
		if (result == 0) {
			env->DeleteLocalRef(bytes);
			return 0;
		}
		env->GetByteArrayRegion(bytes, 0, len, (jbyte *)result);
		result[len] = 0; /* NULL-terminate */
	} else {
		env->DeleteLocalRef(exc);
	}
	env->DeleteLocalRef(bytes);
	return result;
}

