#include "Entity.h"
#include "Scene.h"

using namespace dwarf;

Entity::Entity():
	Object()
{
}

Entity::Entity(const Object &object):
	Object(object),
	m_timer_event(-1.0f)
{
}

Entity::~Entity()
{
//	D_ASSERT_MESSAGE(m_sector == 0, TEXT("You are not allowed to delete an Entity that still belongs to a sector"));
}

void Entity::Tick(float frame_time)
{
}

void Entity::Draw(RenderingDevice &device, const Camera &frustum/*, List<Light*> *lightsources*/) const
{
}

void Entity::OnTimer()
{
}

void Entity::SetTimer(float time)
{
	m_timer_event = GetScene()->Time() + time;
}

void Entity::OnSectorAdd()
{
}

void Entity::OnSectorRemove()
{
}