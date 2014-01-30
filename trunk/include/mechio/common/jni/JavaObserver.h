#ifndef JAVA_OBSERVER_H_
#define JAVA_OBSERVER_H_

#include "jni.h"
#include "JNIClass.h"

namespace org {
  namespace mechio {
	namespace jni {

	class JavaObserver {
		public:
			JavaObserver(JNIEnv* env, jobject obj);
			virtual ~JavaObserver();
			jobject& getObject() { return m_Callback; }
			void attach();
			void detach();
			
		protected:

			JNIEnv* getCurrentEnv() { return m_env; }
			JNIClass getClass() { return JNIClass(getCurrentEnv(), m_Callback); }

		private:
			bool m_NeedsDetached; ///< Whether this thread was attached to JVM
			JNIEnv* m_env;
			jobject m_Callback; // The actual java observer
	};
}}}
#endif
