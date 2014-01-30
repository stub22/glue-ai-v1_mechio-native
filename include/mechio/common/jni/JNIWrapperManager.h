#ifndef JNI_WRAPPER_MANAGER_H_
#define JNI_WRAPPER_MANAGER_H_

#include "jni.h"

namespace org {
  namespace mechio {
	namespace jni {

	class JNIWrapperManager{
		public:

			JNIWrapperManager(){}
			~JNIWrapperManager(){}

			void startup(JavaVM* vm);
			void shutdown(){};

			JavaVM * getJavaVM() const;

			static JNIWrapperManager* instance();
		private:
			JavaVM* m_vm;
	};

	static JNIWrapperManager* s_JNIWrapperManagerInstance;
}}}

#endif
