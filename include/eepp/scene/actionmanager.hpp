#ifndef EE_SCENEACTIONMANAGER_HPP
#define EE_SCENEACTIONMANAGER_HPP

#include <atomic>
#include <eepp/config.hpp>
#include <eepp/system/mutex.hpp>
#include <eepp/system/time.hpp>
#include <vector>
using namespace EE::System;

namespace EE { namespace Scene {

class Action;
class Node;

class EE_API ActionManager {
  public:
	static ActionManager* New();

	ActionManager();

	~ActionManager();

	void addAction( Action* action );

	Action* getActionByTag( const Uint32& tag );

	void removeActionByTag( const Uint32& tag );

	void removeAction( Action* action );

	void removeActions( const std::vector<EE::Scene::Action*>& actions );

	void removeAllActionsFromTarget( Node* target );

	void removeActionsByTagFromTarget( Node* target, const String::HashType& tag );

	std::vector<Action*> getActionsFromTarget( Node* target );

	std::vector<Action*> getActionsByTagFromTarget( Node* target, const String::HashType& tag );

	void update( const Time& time );

	std::size_t count() const;

	bool isEmpty() const;

	void clear();

  protected:
	std::vector<Action*> mActions;
	std::vector<Action*> mActionsRemoveList;
	mutable Mutex mMutex;
	std::atomic<bool> mUpdating;
};

}} // namespace EE::Scene

#endif
