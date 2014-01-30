namespace mechio {
  namespace speech { 
		
	class SAPIObserver{
		public:
			virtual void visitStartInput(long streamNumber) = 0;
			virtual void visitEndInput(long streamNumber) = 0;
			virtual void visitBookmark(long streamNumber, const WCHAR* bookmark) = 0;
			virtual void visitViseme(long streamNumber, int curViseme, int duration, char features, int nextViseme) = 0;
			virtual void visitPhoneme(long streamNumber, int curPhoneme, int duration, char features, int nextPhoneme) = 0;
			virtual void visitSentenceBoundary(long streamNumber, int position, int length) = 0;
			virtual void visitWordBoundary(long streamNumber, int position, int length) = 0;

			virtual void attachObserver() = 0;
			virtual void detachObserver() = 0;
	};
}}