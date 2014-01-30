#ifndef JNI_CLASS_H_
#define JNI_CLASS_H_

#include "jni.h"

namespace org {
  namespace mechio {
    namespace jni {

	class JNIClass
	{
		public:
			JNIClass(JNIEnv* env, jobject& m_obj);
			~JNIClass();

			jfieldID   getFieldId(const char* name, const char* type);
			jmethodID getMethodId(const char* name, const char* type);

		private:
			JNIEnv* m_env;
			jobject& m_obj;
			jclass m_cls;
	};
}}}
#endif