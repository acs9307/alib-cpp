#ifndef ALIB_ENUM_IS_DEFINED
#define ALIB_ENUM_IS_DEFINED

namespace alib
{
	template<typename E>
	constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type
	{
		return static_cast<typename std::underlying_type<E>::type>(e);
	}
}

#endif
