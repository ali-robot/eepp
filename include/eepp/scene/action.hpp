#ifndef EE_SCENEACTION_HPP
#define EE_SCENEACTION_HPP

#include <cstdlib>
#include <eepp/core.hpp>
#include <eepp/system/time.hpp>
using namespace EE::System;

namespace EE { namespace Scene {

class Node;

class EE_API Action {
  public:
	enum ActionType { OnStart, OnStop, OnDone, OnStep, OnDelete };

	typedef std::function<void( Action*, const ActionType& )> ActionCallback;

	Action();

	virtual ~Action();

	/** Starts the action. */
	virtual void start() = 0;

	/** Stops the actions (pause it, it will not reset the animation state). */
	virtual void stop() = 0;

	/** Update the action state (shouldn't be called manually unless you actually now what you are
	 * doing) */
	virtual void update( const Time& time ) = 0;

	/** @return If the action is completed. */
	virtual bool isDone() = 0;

	/** @return The current progress percentage. Normalized between 0 and 1. */
	virtual Float getCurrentProgress() = 0;

	/** The total action time. */
	virtual Time getTotalTime() = 0;

	/** Clones the action. */
	virtual Action* clone() const;

	/** Clones and reverse the action sequence. Note: not all actions can be reversed. */
	virtual Action* reverse() const;

	/** @return The action custom flags. */
	Uint32 getFlags() const;

	/** Sets the action custom flags. */
	void setFlags( const Uint32& flags );

	/** @return The action tag. */
	Uint32 getTag() const;

	/** Sets a tag to identify and filter actions. */
	void setTag( const Uint32& tag );

	/** The target node that the action is being applied. */
	Node* getTarget() const;

	/** Adds an event listener for a specific Action::ActionType.
	 * @param actionType The action type to receive the event notification.
	 * @param callback The callback to receive the event notification.
	 * @return An unique event callback ID. This can be use to remove the event listener with
	 * Action::removeEventListener.
	 */
	Uint32 addEventListener( const ActionType& actionType, const ActionCallback& callback );

	/** Same as Action::addEventListener */
	Action* on( const ActionType& actionType, const ActionCallback& callback );

	/** Removes an event listener previusly added.
	 * @param callbackId The ID of the event listener returned by the Action::addEventListener call.
	 */
	void removeEventListener( const Uint32& callbackId );

	/** Manually emmit an event of type Action::ActionType. */
	void sendEvent( const ActionType& actionType );

	/** Sets the action EE::Scene::Node target. */
	void setTarget( Node* target );

	/** Sets a unique ID to identify the action. */
	void setId( const Uint32& id );

	/** @return The unique action ID. */
	const Uint32& getId();

  protected:
	friend class Node;
	typedef std::map<ActionType, std::map<Uint32, ActionCallback>> ActionCallbackMap;

	Node* mNode;
	Uint32 mFlags;
	Uint32 mTag;
	Uint32 mNumCallBacks;
	Uint32 mId;
	ActionCallbackMap mCallbacks;

	virtual void onStart();

	virtual void onStop();

	virtual void onUpdate( const Time& time );
};

}} // namespace EE::Scene

#endif
