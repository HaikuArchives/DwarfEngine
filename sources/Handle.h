#ifndef D_HANDLE_H_
#define D_HANDLE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

namespace dwarf
{

	/*
		\brief A reference counted handle to a class
	*/

	class RefCnt;

	template <class T> class Handle
	{
	public:
		Handle();
		Handle(T* item);
		Handle(Handle<T> const& h);
		virtual ~Handle();

		Handle<T>& operator=(Handle<T> const& h);
		Handle<T>& operator=(T* item);

		bool operator==(Handle<T> const& h) const;
		bool operator!=(Handle<T> const& h) const;

		bool IsValid() const;
		void Invalidate();
		void Invalidate(bool del);

		T const* operator->() const;
		T* operator->();

	private:
		void SetTo(T* item);

		T* m_item;
	};

	/*
		\brief a class all objects using handle must inherit
	*/
	class RefCnt
	{
	friend class Handle<class T>;
	public:
		RefCnt() : m_num_refs(0)
		{
		}

		virtual ~RefCnt()
		{
		}

		int32 GetRefCount() const
		{
			return m_num_refs;
		}

		void AddRef()
		{
			m_num_refs++;
		}
		
		void Release(bool del = true)
		{
			m_num_refs--;
			if (m_num_refs < 1 && del)
				delete this;
		}

	private:

		int32 m_num_refs;
	};


	// inlines

	template <class T> inline
		Handle<T>::Handle() : m_item(0)
	{}
	
	template <class T> inline
		Handle<T>::Handle(T* item) : m_item(0)
	{
		SetTo(item);
	}
	
	template <class T> inline
		Handle<T>::Handle(Handle<T> const& h) : m_item(0)
	{
		SetTo(h.m_item);
	}
	
	template <class T> inline
		Handle<T>::~Handle()
	{
		Invalidate();
	}

	template <class T> inline
		bool Handle<T>::IsValid() const
	{
		return m_item != 0;
	}

	template <class T> inline
		void Handle<T>::Invalidate()
	{
		if (m_item)
		{
			m_item->Release();
			m_item = 0;
		}
	}

	template <class T> inline
		void Handle<T>::Invalidate(bool del)
	{
		if (m_item)
		{
			m_item->Release(del);
			m_item = 0;
		}
	}
	
	template <class T> inline
	Handle<T>& Handle<T>::operator=(Handle<T> const& h)
	{
		SetTo(h.m_item);
		return *this;
	}

	template <class T> inline
		Handle<T>& Handle<T>::operator=(T* item)
	{
//		D_ASSERT_MESSAGE(item, TEXT("NULL given to Handle<T>::operator=()."));
//		D_ASSERT_MESSAGE(item != m_item, TEXT("Item given to Handle<T>::operator=() is same as stored in handle; unsolvable situation."));

		SetTo(item);
		return *this;
	}

	template <class T> inline
		bool Handle<T>::operator==(Handle<T> const& h) const
	{
		return m_item == h.m_item;
	}

	template <class T> inline
		bool Handle<T>::operator!=(Handle<T> const& h) const
	{
		return m_item != h.m_item;
	}


	template <class T> inline
		T const* Handle<T>::operator->() const
	{
		D_ASSERT_MESSAGE(m_item, TEXT("Invalid handle was dereferenced."));
		return m_item;
	}

	template <class T> inline
		T* Handle<T>::operator->()
	{
		D_ASSERT_MESSAGE(m_item, TEXT("Invalid handle was dereferenced."));
		return m_item;
	}

	template <class T> inline
		void Handle<T>::SetTo(T* item)
	{
		if (m_item)
			m_item->Release();
		
		m_item = item;

		if (m_item)
			m_item->AddRef();
	}
}

#endif // D_HANDLE_H_