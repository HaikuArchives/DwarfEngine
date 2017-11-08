#ifndef D_PAIR_H_
#define D_PAIR_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


namespace dwarf
{


		/*!
			\brief A pair class, can be used if you want to return more than one value, for example.

			Used like this:
			\code
			Pair<int, float> mypair;
			mypair.First = 3;
			mypair.Second = 4.0f;
			cout << mypair.First;
			Could be used to make a map class.. when we make a tree class.. like this:
			template<class Key, class Val> class Map : public Tree<Pair<Key, Val> >
			\endcode
			Would be cool =)
		*/
		template<class F, class S> class Pair
		{
		public:
			Pair();
			Pair(const F &f, const S &s);
			Pair(const Pair &p);
			F &First;
			S &Second;
		};



		template<class F, class S> inline Pair<F,S>::Pair() : First(F()), Second(S()) {}
		template<class F, class S> inline Pair<F,S>::Pair(const F &f, const S &s) : First(f), Second(s) {}
		template<class F, class S> inline Pair<F,S>::Pair(const Pair<F,S> &p) : First(p.First), Second(s.Second) {}

} // dwarf
#endif // D_PAIR_H_
