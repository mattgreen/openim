#ifndef OPENIM_COMPONENTS_MOCKOBJECTSCOMPONENT_HPP
#define OPENIM_COMPONENTS_MOCKOBJECTSCOMPONENT_HPP

#include "../../ComponentImpl.hpp"

#include "MockAccountFactory.hpp"

class MockObjectsComponent :
	public ComponentImpl<0x66aaca0f>
{
public:
	MockObjectsComponent();
	~MockObjectsComponent();

private:
	MockAccountFactory _factory;
};

#endif