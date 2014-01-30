#ifndef JNI_UTILITIES_H_
#define JNI_UTILITIES_H_

#include <jni.h>
#include <string>
#include <vector>

namespace org {
  namespace mechio {
    namespace jni {

	template<typename Wrapped>
	static Wrapped* getPointer(JNIEnv* env, jobject jobj) {
		JNIClass jcls(env, jobj);
		jfieldID fid = jcls.getFieldId("m_ptr", "J");
		if ( fid == NULL ) {
			return 0;
		}
		jlong addr = env->GetLongField(jobj, fid);
		return (Wrapped*)addr;
	}

	jobject JNU_NewStringNative(JNIEnv *env, const wchar_t *str);
    char* JNU_GetStringNativeChars(JNIEnv *env, jstring jstr);

}}} // close namespaces
#endif // JNI_UTILITIES_H_
