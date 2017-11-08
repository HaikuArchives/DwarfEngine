/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"
#include "Object.h"
#include "Vertex.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix4x4.h"
#include "Euler.h"
#include "Sector.h"

using namespace dwarf;

Object::Object() :
	m_scene(0),
	m_sector(0),
	m_position(0.0),
	m_rotation()
{
}

Object::Object(const Vertex<3> &position, const Quaternion &rotation):
	m_scene(0),
	m_sector(0),
	m_position(position),
	m_rotation(rotation)
{
}

Object::Object(const Vertex<3> &position, const Quaternion &rotation, Sector *sector):
	m_scene(&sector->GetScene()),
	m_sector(sector),
	m_position(position),
	m_rotation(rotation)
{
	D_ASSERT_MESSAGE(sector != 0, TEXT("You cannot construct an object without a valid sector (at least not with this constructor)"));
}

Object::~Object()
{
}

void Object::MoveForward(float distance)
{
	Vertex<3> vector = (Rotation()).ToMatrix() * Vector(0.0, 0.0, -1.0) * distance;
	MoveBy(vector);
}

void Object::MoveRight(float distance)
{
	Vertex<3> vector = (Rotation()).ToMatrix() * Vector(1.0, 0.0, 0.0) * distance;
	MoveBy(vector);
}

void Object::MoveUpward(float distance)
{
	Vertex<3> vector = (Rotation()).ToMatrix() * Vector(0.0, 1.0, 0.0) * distance;
	MoveBy(vector);
}

void Object::MoveTo(const Vertex<3> &pos)
{
	m_position = pos;
}

void Object::Restore(InStream &stream)
{
	stream >> m_position >> m_rotation;
}

void Object::Serialize(OutStream &stream) const
{
	stream << m_position << m_rotation;
}

void Object::MoveBy(const Vertex<3> &pos)
{
	m_position += pos;
}

void Object::RotateTo(const Quaternion &rotation)
{
	m_rotation = rotation;
}

void Object::RotateBy(const Quaternion &rotation)
{
	m_rotation *= rotation;
}

void Object::LookAt(const Vertex<3> &pos)
{
	D_ASSERT_MESSAGE(pos != Position(), TEXT("An object cannot look at the same position where it is located"));

	float dist_x = pos.x() - Position().x();
	float dist_y = pos.y() - Position().y();
	float dist_z = pos.z() - Position().z();

	RotateTo(
		Quaternion(
			Euler(
				atan2(dist_y,sqrt(dist_x*dist_x + dist_z*dist_z)),
				atan2(-dist_x,-dist_z),
				0.0
				)
			)
		);
}

void Object::LookAt(const Vertex<3> &lookat, const Vertex<3> &up_vector)
{
	Vector z_axis = lookat - m_position;
	z_axis.Normalize();

	Vector x_axis = up_vector.CrossProduct(z_axis);
	x_axis.Normalize();

	Vector y_axis = z_axis.CrossProduct(x_axis);
	y_axis.Normalize();
	
	Matrix4x4 rotation(x_axis, y_axis, z_axis);
//	rotation.Transpose();
	m_rotation.FromMatrix(rotation);
//	m_rotation = - m_rotation;
}
