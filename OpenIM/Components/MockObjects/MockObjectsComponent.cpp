#include "MockObjectsComponent.hpp"

#include "../../ComponentManager.hpp"
#include "../../Interfaces/IAccountManager.hpp"

MockObjectsComponent::MockObjectsComponent()
{
	IAccountManager* mgr = ComponentManager::Get<IAccountManager>();
	mgr->Register(&_factory);
}

MockObjectsComponent::~MockObjectsComponent()
{
	IAccountManager* mgr = ComponentManager::Get<IAccountManager>();
	mgr->Unregister(&_factory);
}