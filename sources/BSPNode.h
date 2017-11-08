#ifndef D_BSP_NODE_H_
#define D_BSP_NODE_H_


/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "nommgr.h"
#include <list>
#include "mmgr.h"


#include "Defs.h"


#include "Line.h"
#include "Vertex.h"
#include "Polygon.h"
#include "MirrorMatrix.h"

namespace dwarf
{

	/*!
		This class represents a bsp-tree (binary space partitioning). A bsp-tree is a binary tree
		where every nod splits the space into two parts, using a plane. This will form a collection
		of convex leaf, each leaf can be solid or non-solid (or anything else, because it's type is
		a template parameter). You can build your bsp-tree in many ways.
		\arg You can add polygon per polygon to the tree (using AddPolygon()), the Node class will
		handle splits od the polygons and build the tree, the downside is that the tree may not be
		balanced.
		\arg You can make a list of all your polygons and give the constructor that list (or the
		BuildFrom() method), this will try to balance the tree before it adds each polygon. This is
		very slow if you have a large number of polygons, but it will be more efficient in the end.
		You should never use this method when your application is loaded, prefer using a precalced
		tree (this class derives from Serializable, so it's very easy to save it to disk).
		\arg The third way you can build you bsp-tree is, if you already have a bsp-tree in some
		other form, to add planes to each child node. Use AddPlane().
		\brief A bsp-tree class

		\sa BuildFrom() AddPolygon() AddPlane()
	*/
	template<class Leaf, class Node = uint8> class BSPNode : public Serializable
	{

	public:


		BSPNode(const Plane<3> &plane, const Leaf &front_leaf, const Leaf &back_leaf);
		BSPNode(const std::list<dwarf::Polygon> &poly_list, const Leaf &front_leaf, const Leaf &back_leaf);
		BSPNode(const Plane<3> &plane, const Leaf &front_leaf, const Leaf &back_leaf, const Node &node = Node());
		BSPNode();
		virtual ~BSPNode();

		void BuildFrom(const std::list<dwarf::Polygon> &poly_list, const Leaf &front_leaf, const Leaf &back_leaf);

		Leaf &LeafForVertex(const Vertex<3> &point);

		template <class P> bool Intersection(const Vertex<3> &start, const Vertex<3> &stop, Vertex<3> &intersection, Vertex<3> &normal, Leaf &leaf, P p) const
		{
			if (start == stop) return false;
			
			int32 from = m_plane.IsInside(start)?0:1;
			int32 to = m_plane.IsInside(stop)?0:1;
			
			if (m_plane.IsOnPlane(start)) from = to;
			if (m_plane.IsOnPlane(stop)) to = from;
			
			// if the two points are on the same side, send the line further down in that sub-tree
			if (from == to)
			{
				if (!m_child[from].is_leaf)
				{
					if (m_child[from].node->Intersection(start, stop, intersection, normal, leaf, p))
					{
						if (intersection == start) normal = m_plane.Normal();
						return true;
					}
				}
				else
				{
					normal = m_plane.Normal();
					intersection = start;
					leaf = *m_child[from].leaf;
					if (p(leaf)) return true;
				}
			}
			else // if not, send the line down the both trees (the nearest first)
			{
				Vertex<3> midpoint = m_plane.Intersects(Line<3>(start, start - stop));
				normal = m_plane.Normal();
				
				if (!m_child[from].is_leaf)
				{
					if (m_child[from].node->Intersection(start, midpoint, intersection, normal, leaf, p))
					{
						if (intersection == start) normal = m_plane.Normal();
						return true;
					}
				}
				else
				{
					normal = m_plane.Normal();
					intersection = start;
					leaf = *m_child[from].leaf;
					if (p(leaf)) return true;
				}
				
				
				if (!m_child[to].is_leaf)
				{
					if (m_child[to].node->Intersection(midpoint, stop, intersection, normal, leaf, p))
					{
						if (intersection == midpoint) normal = m_plane.Normal();
						return true;
					}
				}
				else
				{
					normal = m_plane.Normal();
					intersection = midpoint;
					leaf = *m_child[to].leaf;
					if (p(leaf)) return true;
				}
				
			}
			normal = m_plane.Normal();
			intersection = stop;
			leaf = *m_child[from].leaf;
			return false;
		}
		
	
		template <class P> bool Bounce(const Vertex<3> &start, const Vertex<3> &stop, ListArray<Vertex<3> > &collisions, Vertex<3> &direction, P p) const
		{
			Vertex<3> coll, normal = Vertex<3>(0, 1, 0);
			Leaf l;
			if (Intersection(start, stop, coll, normal, l, p))
			{
				// we are probably inside a solid leaf if we collide instantly
				if (coll == start) return true;
				
				Vertex<3> vector = MirrorMatrix(Plane<3>(coll, normal)) * stop;
				collisions.AddItem(coll);
				
				if (!Bounce(coll, vector, collisions, direction, p)) collisions.AddItem(vector);
				return true;
			}
			direction = stop - start;
			direction.Normalize();
			return false;
		}
		

		void AddPolygon(const dwarf::Polygon &polygon, const Leaf &front_leaf, const Leaf &back_leaf);

		void Print(int32 depth = 0) const;

		// low level access
		const BSPNode *ChildForVertex(const Vertex<3> &v) const;
		BSPNode *ChildForVertex(const Vertex<3> &v);

		const Node &NodeData() const;
		Node &NodeData();

		BSPNode *AddPlane(const Plane<3> &plane, bool front_side, const Leaf &front_leaf, const Leaf &back_leaf, const Node &data = Node());
		

		int32 NodeCount();
		int32 Depth();


		void Serialize(OutStream &stream) const;
		void Restore(InStream &stream);



		const BSPNode &operator=(const BSPNode &node);


	protected:




		void AddChild(int32 side, const dwarf::Polygon &polygon, const Leaf &front_leaf, const Leaf &back_leaf);
		std::list<dwarf::Polygon>::const_iterator find_balanced_root(const std::list<dwarf::Polygon> &list);

		
		struct node_leaf
		{
		// if true the leaf pointer is used, if false the child is another node, and the node pointer is used
			bool is_leaf;
			union
			{
				BSPNode<Leaf, Node> *node;
				Leaf *leaf;
			};
		};

		Node m_node;
		Plane<3> m_plane;
		struct node_leaf m_child[2]; // 0 = front-side 1 = back-side
	};











	template<class Leaf, class Node>
	void BSPNode<Leaf, Node>::BuildFrom(const std::list<dwarf::Polygon> &poly_list, const Leaf &front_leaf, const Leaf &back_leaf)
	{
		// if the tree already contains subtrees, delete them
		for (int32 i = 0; i < 2; i++)
		{
			if (m_child[i].node != 0)
			{
				if (m_child[i].is_leaf) delete m_child[i].leaf;
				else delete m_child[i].node;
			}
		}

		//	printf("list: %ld\n", line_list.Size());
		std::list<Polygon>::const_iterator root = find_balanced_root(poly_list);
		const Polygon &split_poly = *root;
		
		std::list<Polygon> subtree[2];
		
		Plane<3> split_plane(split_poly[0], split_poly[1], split_poly[2]);
		
		for (std::list<dwarf::Polygon>::const_iterator i = poly_list.begin(); i != poly_list.end(); i++)
		{
			if (i == root) continue; // don't add the root
			const Polygon &p = *i;
			
			if (p.IsSplitBy(split_plane))
			{
				// splitting polyon and add the two parts to it's subtree
				Plane<3> planes[] = {split_plane, -split_plane};
				for (int32 i = 0; i < 2; i++)
				{
					logger(TEXT("splitting polygon "));
					logger(TEXT("(%f %f %f), (%f %f %f), (%f %f %f)"), p[0].x(), p[0].y(), p[0].z(), p[1].x(), p[1].y(), p[1].z(), p[2].x(), p[2].y(), p[2].z());
					Polygon split = p.ClipAgainst(planes[i]);
					if (!split.IsValid())
					{
						logger(TEXT("a polygon was split, but one of the parts was too small and is thrown away"));
						if (split.NumVertices() >= 2) logger(TEXT("(%f %f %f), (%f %f %f)"), split[0].x(), split[0].y(), split[0].z(), split[1].x(), split[1].y(), split[1].z());
						continue;
					}
					subtree[i].push_back(split);
				}
			}
			else if (split_plane.IsInside(p[0]))
			{
				subtree[0].push_back(p);
			}
			else
			{
				subtree[1].push_back(p);
			}
		}
		
		m_plane = split_plane;
		
		//	printf("left: %ld, right: %ld\n", subtree[0].Size(), subtree[1].Size());
		
		for (int32 i = 0; i < 2; i++)
		{
			if (subtree[i].size() == 0)
			{
				m_child[i].is_leaf = true;
				m_child[i].leaf = new Leaf( i==0?front_leaf:back_leaf);
			}
			else
			{
				m_child[i].is_leaf = false;
				m_child[i].node = new BSPNode(subtree[i], front_leaf, back_leaf);
			}
		}
		
	}
	

	template<class Leaf, class Node>
	BSPNode<Leaf, Node>::BSPNode(const Plane<3> &plane, const Leaf &front_leaf, const Leaf &back_leaf, const Node &data):
		m_plane(plane),
		m_node(data)
	{
		m_child[0].is_leaf = true;
		m_child[1].is_leaf = true;
		m_child[0].leaf = new Leaf(front_leaf);
		m_child[1].leaf = new Leaf(back_leaf);
	}

	template<class Leaf, class Node>
	BSPNode<Leaf, Node>::BSPNode(const std::list<dwarf::Polygon> &poly_list, const Leaf &front_leaf, const Leaf &back_leaf)
	{
		m_child[0].node = 0;
		m_child[1].node = 0;
		BuildFrom(poly_list, front_leaf, back_leaf);
	}


	template<class Leaf, class Node>
	BSPNode<Leaf, Node>::BSPNode(const Plane<3> &plane, const Leaf &front_leaf, const Leaf &back_leaf):
		m_plane(plane)
	{
		D_ASSERT_MESSAGE(m_plane.Normal() != Vertex<3>(0,0,0), TEXT("Invalid plane added to bsp-tree"));
		m_child[0].is_leaf = true;
		m_child[0].leaf = new Leaf(front_leaf);
		
		m_child[1].is_leaf = true;
		m_child[1].leaf = new Leaf(back_leaf);
	}

		
	template<class Leaf, class Node>
	inline BSPNode<Leaf, Node>::BSPNode()
	{
		m_child[0].node = 0;
		m_child[1].node = 0;
	}
	

	template<class Leaf, class Node>
	BSPNode<Leaf, Node>::~BSPNode()
	{
		for (int32 i = 0; i < 2; i++)
		{
			if (m_child[i].node)
			{
				if (m_child[i].is_leaf) delete m_child[i].leaf;
				else delete m_child[i].node;
			}
		}
	}



	template<class Leaf, class Node>
	const BSPNode<Leaf, Node> &BSPNode<Leaf, Node>::operator=(const BSPNode<Leaf, Node> &node)
	{
		D_ASSERT_MESSAGE((node.m_child[0].node == 0) == (node.m_child[1].node == 0), TEXT("internal error, an uninitialized BSPNode had one initialized child"));


		m_plane = node.m_plane;
		m_node = node.m_node;
		

		// special case if the node we are assigned is uninitialized
		if (node.m_child[0].node == 0)
		{
			for (int32 i = 0; i < 2; i++)
			{
				if (m_child[i].node == 0) continue;
				if (m_child[i].is_leaf) delete m_child[i].leaf;
				else delete m_child[i].node;
				m_child[i].node = 0;
			}
			return *this;
		}
		
		// if the assigned node is initialized, copy the children
		for(int32 i = 0; i < 2; i++)
		{
			if (node.m_child[i].is_leaf)
			{
				if (!m_child[i].is_leaf)
				{
					delete m_child[i].node;
					m_child[i].is_leaf = true;
					m_child[i].leaf = new Leaf(*node.m_child[i].leaf);
				}
				else
				{
					*m_child[i].leaf = *node.m_child[i].leaf;
				}
			}
			else
			{
				if (m_child[i].is_leaf)
				{
					delete m_child[i].leaf;
					m_child[i].is_leaf = false;
					m_child[i].node = new BSPNode<Leaf, Node>(*node.m_child[i].node);
				}
				else
				{
					*m_child[i].node = *node.m_child[i].node;
				}
			}
		}
		return *this;
	}
	
	/*!

	*/
	template<class Leaf, class Node>
	BSPNode<Leaf, Node> *BSPNode<Leaf, Node>::AddPlane(const Plane<3> &plane, bool front_side, const Leaf &front_leaf, const Leaf &back_leaf, const Node &data)
	{
		int32 side = front_side?0:1;

		D_ASSERT_MESSAGE(m_child[side].is_leaf || m_child[side].node == 0, TEXT("You can only use the AddPlane() method on bsp-node that is either uninitialized or that has a leaf in the given subtree."));
		
		if (m_child[side].node == 0)
		{
			m_plane = plane;
			m_node = data;
			m_child[0].is_leaf = true;
			m_child[1].is_leaf = true;
			m_child[0].leaf = new Leaf(front_leaf);
			m_child[1].leaf = new Leaf(back_leaf);
			return this;
		}
		else
		{
			delete m_child[side].leaf;
			m_child[side].is_leaf = false;
			m_child[side].node = new BSPNode(plane, front_leaf, back_leaf, data);
			return m_child[side].node;
		}
	}

	template<class Leaf, class Node>
	Leaf &BSPNode<Leaf, Node>::LeafForVertex(const Vertex<3> &v)
	{
//		logger("BSPNode: (%f %f %f)", v.x(), v.y(), v.z());
		int32 side = m_plane.IsInside(v)?0:1;
		if (m_child[side].is_leaf) return *m_child[side].leaf;
		else
		{
//			char *name[] = {"front", "back"};
//			logger("%s side", name[side]);
			return m_child[side].node->LeafForVertex(v);
		}
	}

	template<class Leaf, class Node>
	inline const BSPNode<Leaf, Node> *BSPNode<Leaf, Node>::ChildForVertex(const Vertex<3> &v) const
	{
		int32 side = m_plane.IsInside(v)?0:1;
		if (m_child[side].is_leaf) return 0;
		return m_child[side].node;
	}

	template<class Leaf, class Node>
	inline BSPNode<Leaf, Node> *BSPNode<Leaf, Node>::ChildForVertex(const Vertex<3> &v)
	{
		int32 side = m_plane.IsInside(v)?0:1;
		if (m_child[side].is_leaf) return 0;
		return m_child[side].node;
	}

	template<class Leaf, class Node>
	inline const Node &BSPNode<Leaf, Node>::NodeData() const
	{
		return m_node;
	}

	template<class Leaf, class Node>
	inline Node &BSPNode<Leaf, Node>::NodeData()
	{
		return m_node;
	}
	

	template<class Leaf, class Node>
	int32 BSPNode<Leaf, Node>::NodeCount()
	{
		if (m_child[0].node == 0) return 0;
		int32 count = 1;
		for (int32 i = 0; i < 2; i++)
			if (!m_child[i].is_leaf) count += m_child[i].node->NodeCount();
		return count;
	}

	template<class Leaf, class Node>
	int32 BSPNode<Leaf, Node>::Depth()
	{
		if (m_child[0].node == 0) return 0;
		return 1 + max(m_child[0].is_leaf?0:m_child[0].node->Depth(), m_child[1].is_leaf?0:m_child[1].node->Depth());
	}
	
	

	/*!
		Remember that it is important which way your polygons are facing.
		\brief adds a plane, that corresponds to the given polygon, to the bsp-tree
	*/
	template<class Leaf, class Node>
	void BSPNode<Leaf, Node>::AddPolygon(const dwarf::Polygon &polygon, const Leaf &front_leaf, const Leaf &back_leaf)
	{
		// a special case, if this node is uninitialized
		// initialize this node to the given parameters
		if (m_child[0].node == 0)
		{
			m_child[0].leaf = new Leaf(front_leaf);
			m_child[0].is_leaf = true;

			m_child[1].leaf = new Leaf(back_leaf);
			m_child[1].is_leaf = true;

			m_plane = Plane<3>(polygon[0], polygon[1], polygon[2]);
		}
		// if the polygon is split by the plane we have to split it and
		// add the respective child recursively
		else if (polygon.IsSplitBy(m_plane))
		{
			// the polygon is split by the plane, add two polygons tp the child-nodes
			Plane<3> planes[] = {m_plane, -m_plane};
			for (int32 i = 0; i < 2; i++)
			{
				logger(TEXT("splitting polygon "));
//				logger(TEXT("(%f %f %f), (%f %f %f), (%f %f %f)"), polygon[0].x, polygon[0].y, polygon[0].z, polygon[1].x, polygon[1].y, polygon[1].z, polygon[2].x, polygon[2].y, polygon[2].z);
				Polygon split = polygon.ClipAgainst(planes[i]);
				if (!split.IsValid())
				{
//					logger(TEXT("a polygon was split, but one of the parts was too small and is thrown away"));
//					if (split.NumVertices() >= 2) logger(TEXT("(%f %f %f), (%f %f %f)"), split[0].x, split[0].y, split[0].z, split[1].x, split[1].y, split[1].z);
					continue;
				}

				AddChild(i, split, front_leaf, back_leaf);
			}
		}
		// if it's not split we just have to add it 
		else
		{
			int32 sub_child = m_plane.IsInside(polygon[0])?0:1;
			AddChild(sub_child, polygon, front_leaf, back_leaf);
		}
	}

	template<class Leaf, class Node>
	void BSPNode<Leaf, Node>::Print(int32 depth) const
	{
		if (m_child[0].node == 0)
		{
			logger(TEXT("uninitialized node."));
			return;
		}
		String message;
		for (int32 i = 0; i< depth; i++)
			message += TEXT("\t");
		message += TEXT("plane: %f %f %f %f");
		logger(message, m_plane.Normal().x(), m_plane.Normal().y(), m_plane.Normal().z(), m_plane.D());
		if (!m_child[0].is_leaf) m_child[0].node->Print(depth + 1);
		if (!m_child[1].is_leaf) m_child[1].node->Print(depth + 1);
	}

	template<class Leaf, class Node>
	inline void BSPNode<Leaf, Node>::AddChild(int32 side, const Polygon &polygon, const Leaf &front_leaf, const Leaf &back_leaf)
	{
		if (m_child[side].is_leaf)
		{
			delete m_child[side].leaf;
			m_child[side].is_leaf = false;
			m_child[side].node = new BSPNode(Plane<3>(polygon[0], polygon[1], polygon[2]), front_leaf, back_leaf);
		}
		else
		{
			m_child[side].node->AddPolygon(polygon, front_leaf, back_leaf);
		}
	}

	template<class Leaf, class Node>
	std::list<dwarf::Polygon>::const_iterator BSPNode<Leaf, Node>::find_balanced_root(const std::list<dwarf::Polygon> &list)
	{
		std::list<dwarf::Polygon>::const_iterator ret;

		int32 grade = 2100000000;

		for (std::list<dwarf::Polygon>::const_iterator i = list.begin(); i != list.end(); i++)
		{
			const dwarf::Polygon &root = *i;
			Plane<3> split_plane(root[0], root[1], root[2]);
			int32 front = 0;
			int32 back = 0;
			int32 splits = 0;
			for (std::list<dwarf::Polygon>::const_iterator j = list.begin(); j != list.end(); j++)
			{
				if (j == i) continue; // don't test against itself
				const dwarf::Polygon &poly = *j;

				if (poly.IsSplitBy(split_plane)) splits++;
				else if (split_plane.IsInside(root[0])) front++;
				else back++;
			}
			int32 c_grade = splits + abs(front - back);
			if (c_grade < grade)
			{
				grade = grade;
				ret = i;
			}
		}
		return ret;
	}

	template<class Leaf, class Node>
	void BSPNode<Leaf, Node>::Serialize(OutStream &stream) const
	{
		stream << m_plane << m_node;

		for (int32 i = 0; i < 2; i++)
		{
			const node_leaf &child = m_child[i];

			stream << child.is_leaf;
			if (child.is_leaf)
			{
				stream << *child.leaf;
			}
			else
			{
				stream << *child.node;
			}
		}
	}

	template<class Leaf, class Node>
	void BSPNode<Leaf, Node>::Restore(InStream &stream)
	{
		stream >> m_plane >> m_node;

		for (int32 i = 0; i < 2; i++)
		{
			node_leaf &child = m_child[i];

			stream >> child.is_leaf;
			if (child.is_leaf)
			{
				m_child[i].leaf = new Leaf();
				stream >> *m_child[i].leaf;
			}
			else
			{
				m_child[i].node = new BSPNode();
				stream >> *child.node;
			}
		}
	}


} // dwarf

#endif // D_BSP_NODE_H_
