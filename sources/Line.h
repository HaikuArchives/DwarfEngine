#ifndef D_LINE_H_
#define D_LINE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "Serializable.h"
#include "Vertex.h"


namespace dwarf
{


	/*!
	*/
	template<int N> class Line : public Serializable
	{
	public:

		Line(const Vertex<N> &vertex, const Vertex<N> &direction);
	//	virtual ~Line();
		void SetTo(const Vertex<N> &v1, const Vertex<N> &direction);

		void SetDirection(const Vertex<N> &direction);
		const Vertex<N> &Direction() const;

		void MoveTo(const Vertex<N> &position);
		const Vertex<N> &Position() const;

		void Normalize();

		virtual void Restore(InStream &buffer);
		virtual void Serialize(OutStream &buffer) const;

	private:

		Vertex<N> m_direction;
		Vertex<N> m_position;
	};

	// inline definitions

	template<int N> inline void Line<N>::SetDirection(const Vertex<N> &direction)
	{
		m_direction = direction;
	}

	template<int N> inline const Vertex<N> &Line<N>::Direction() const
	{
		return m_direction;
	}

	template<int N> inline const Vertex<N> &Line<N>::Position() const
	{
		return m_position;
	}

	template<int N> inline void Line<N>::MoveTo(const Vertex<N> &position)
	{
		m_position = position;
	}

	template<int N> inline void Line<N>::Normalize()
	{
		m_direction.Normalize();
	}

	template<int N> Line<N>::Line(const Vertex<N> &position, const Vertex<N> &direction):
		m_direction(direction),
		m_position(position)
	{
	}

	/*!
		\brief sets the line
	*/
	template<int N> void Line<N>::SetTo(const Vertex<N> &position, const Vertex<N> &direction)
	{
		m_position = position;
		m_direction = direction;
		Normalize();
	}

	template<int N> void Line<N>::Restore(InStream &stream)
	{
		stream >> m_position >> m_direction;
	}

	template<int N> void Line<N>::Serialize(OutStream &stream) const
	{
		stream << m_position << m_direction;
	}

} // dwarf

#endif // D_LINE_H
