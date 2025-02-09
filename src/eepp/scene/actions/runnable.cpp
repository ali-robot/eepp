#include <eepp/scene/actions/runnable.hpp>

namespace EE { namespace Scene { namespace Actions {

Runnable* Runnable::New( RunnableFunc callback, const Time& time ) {
	return eeNew( Runnable, ( callback, time ) );
}

Runnable::Runnable( Runnable::RunnableFunc callback, const Time& time ) :
	Delay( time ), mCallback( callback ) {}

void Runnable::update( const Time& ) {
	if ( mCallback && Delay::isDone() && !mCalled ) {
		mCallback();
		mCalled = true;
	} else if ( !mCallback ) {
		mCalled = true;
	}
}

bool Runnable::isDone() {
	return Delay::isDone() && mCalled;
}

Action* Runnable::clone() const {
	return New( mCallback, mTime );
}

Action* Runnable::reverse() const {
	return NULL; // or a time machine
}

void Runnable::onStart() {}

}}} // namespace EE::Scene::Actions
