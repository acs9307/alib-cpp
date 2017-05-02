#ifndef ALIB_CPP_FLAGS_IS_DEFINED
#define ALIB_CPP_FLAGS_IS_DEFINED

#include "enum.h"

namespace alib
{
	template<typename T>
	class FlagPole
	{
	private:
		T _pole;

	public:
		/* Constructors */
		FlagPole() :_pole(static_cast<T>(0)) {}
		FlagPole(const T& pole) :_pole(pole) {}
		FlagPole(const FlagPole& pole) : _pole(pole._pole) {}
		/****************/

		/* Functions */
		T raise(const T& flag) { return(_pole = raise(_pole, flag)); }
		T lower(const T& flag) { return(_pole = lower(_pole, flag)); }

		/* Getters */
		T getPole()const { return(_pole); }
		bool isRaised(const T& flag)const { return(isRaised(_pole, flag)); }
		/***********/

			/* Static */
		static constexpr T raise(const T& pole, const T& flag) { return(static_cast<T>(to_integral(pole) | to_integral(flag))); }
		static constexpr T lower(const T& pole, const T& flag) { return(static_cast<T>(to_integral(pole) & ~to_integral(flag))); }

		static constexpr bool isRaised(T pole, T flag) { return((to_integral(pole) & to_integral(flag)) != 0); }
			/**********/
		/*************/

		/* Operator Overrides */
		const FlagPole& operator=(const T& flag)
		{
			_pole = flag;
			return(*this);
		}
		const FlagPole& operator=(const FlagPole& flag) 
		{
			_pole = flag; 
			return(*this);
		}

		T operator|(const T& flag) const { return(static_cast<T>(to_integral(_pole) | to_integral(flag))); }
		T operator|(const FlagPole& flag) const { return(static_cast<T>(to_integral(_pole) | to_integral(flag._pole))); }
		const FlagPole& operator|=(const T& flag)
		{
			_pole = (*this) | flag;
			return(*this);
		}
		const FlagPole& operator|=(const FlagPole& flag)
		{
			_pole = (*this) | flag;
			return(*this);
		}

		T operator&(const T& flag) const { return(static_cast<T>(to_integral(_pole) & to_integral(flag))); }
		T operator&(const FlagPole& flag) const { return(static_cast<T>(to_integral(_pole) & to_integral(flag._pole))); }
		const FlagPole& operator&=(const T& flag)
		{
			_pole = (*this) & flag;
			return(*this);
		}
		const FlagPole& operator&=(const FlagPole& flag)
		{
			_pole = (*this) & flag;
			return(*this);
		}
		/**********************/
	};
}

#endif
