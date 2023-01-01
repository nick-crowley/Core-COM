#pragma once
#include "comFramework.h"

namespace meta
{
	// identity
	template <typename T> 
	struct identity { using type = T; };

	template <typename T> 
	using identity_t = typename identity<T>::type;


	// tuple_first_n
	template <size_t N, typename Tuple>
	struct tuple_first_n : identity<void> {};

	template <size_t N, typename Tuple>
	using tuple_first_n_t = typename tuple_first_n<N,Tuple>::type;

	// template <typename T0, ..., typename Tn, typename... Rest> 
	// struct tuple_first_n<1..N, std::tuple<T0,..,Tn,Rest...>> : identity<std::tuple<T0,..,Tn>> {};
#define tuple_first_n__definition(obj, n, unused)											\
	template <BOOST_PP_ENUM_PARAMS(n,typename T), typename... Rest> 						\
	struct tuple_first_n<n,std::tuple<BOOST_PP_ENUM_PARAMS(n,T),Rest...>> 					\
		: identity<std::tuple<BOOST_PP_ENUM_PARAMS(n,T)>> {};
	// Define operations for 1 <= N <= 15
	// template <typename T0, typename T1, typename... Rest> struct tuple_first_n<2,std::tuple<T0, T1, Rest...>> : identity<std::tuple<T0, T1>> {};
	BOOST_PP_REPEAT_FROM_TO(1, 16, tuple_first_n__definition, ~);
#undef tuple_first_n__definition


	// tuple_reverse
	template <typename Tuple>
	struct tuple_reverse : identity<void> {};

	template <typename Tuple>
	using tuple_reverse_t = typename tuple_reverse<Tuple>::type;

	// template <typename T0, ..., typename Tn>
	// struct tuple_reverse<std::tuple<T0,..,Tn>> : identity<std::tuple<Tn,...,T0>> {};
#define tuple_reverse__definition(dummy, n, unused)														\
	template <BOOST_PP_ENUM_PARAMS(n,typename T)>														\
	struct tuple_reverse<std::tuple<BOOST_PP_ENUM_PARAMS(n,T)>>											\
	  : identity<std::tuple<BOOST_PP_REPEAT(n,tuple_reverse__decrementing_type,BOOST_PP_DEC(n))>> {};
	// [,] T(idx-n)
#define tuple_reverse__decrementing_type(dummy, n, count)												\
	BOOST_PP_COMMA_IF(n) BOOST_PP_CAT(T,BOOST_PP_SUB(count,n))
	// Define operations for 1 <= N <= 15
	// eg. template <typename T0, typename T1> struct tuple_reverse<std::tuple<T0, T1>> : identity<std::tuple<T1, T0>> {};
	BOOST_PP_REPEAT_FROM_TO(1, 16, tuple_reverse__definition, ~);
#undef tuple_reverse__definition
#undef tuple_reverse__decrementing_type


	// tuple_last_n
	template <size_t N, typename Tuple>
	using tuple_last_n = tuple_reverse<tuple_first_n_t<N,tuple_reverse_t<Tuple>>>;

	template <size_t N, typename Tuple>
	using tuple_last_n_t = typename tuple_last_n<N,Tuple>::type;


	// tuple_transform
	template <typename Tuple, template <typename> typename UnaryFunction>
	struct tuple_transform : identity<void> {};

	template <typename Tuple, template <typename> typename UnaryFunction>
	using tuple_transform_t = typename tuple_transform<Tuple,UnaryFunction>::type;

	// template <typename T0, ... , typename Tn, template <typename> typename UnaryMetaFunction>
	// struct tuple_transform<std::tuple<T0,...,Tn>, UnaryMetaFunction> 
	//   : identity<std::tuple<UnaryMetaFunction<T0>,...,UnaryMetaFunction<Tn>>> {};
#define tuple_transform__definition(dummy, n, unused)												\
	template <BOOST_PP_ENUM_PARAMS(n,typename T), template <typename> typename UnaryMetaFunction>	\
	struct tuple_transform<std::tuple<BOOST_PP_ENUM_PARAMS(n,T)>,UnaryMetaFunction>					\
		: identity<std::tuple<BOOST_PP_REPEAT(n,tuple_transform__functor,unused)>> {};
	// [, ] UnaryMetaFunction<Tn>
#define tuple_transform__functor(dummy, n, unused)													\
	BOOST_PP_COMMA_IF(n) UnaryMetaFunction<BOOST_PP_CAT(T,n)>
	// Define operations for 1 <= N <= 15
	// eg. template <typename T0, template <typename> typename UnaryMetaFunction> 
	//     struct tuple_transform<std::tuple<T0>,UnaryMetaFunction> : identity<std::tuple<UnaryMetaFunction<T0>>> {};
	BOOST_PP_REPEAT_FROM_TO(1, 16, tuple_transform__definition, ~);
#undef tuple_transform__functor
#undef tuple_transform__definition

	
	// tuple_front
	template <typename Tuple>
	struct tuple_front : identity<std::tuple_element_t<0,Tuple>> {};
	
	template <typename Tuple>
	using tuple_front_t = typename tuple_front<Tuple>::type;

	// tuple_back
	template <typename Tuple>
	struct tuple_back : identity<std::tuple_element_t<std::tuple_size_v<Tuple>-1,Tuple>> {};
	
	template <typename Tuple>
	using tuple_back_t = typename tuple_back<Tuple>::type;


	// tuple_push_front
	template <typename Tuple, typename Item>
	struct tuple_push_front;

	template <typename... Items, typename Item>
	struct tuple_push_front<std::tuple<Items...>,Item> : meta::identity<std::tuple<Item,Items...>>
	{};

	template <typename Tuple, typename Item>
	using tuple_push_front_t = typename tuple_push_front<Tuple,Item>::type;
}

namespace meta::testing 
{
	// struct UT0 {}, ..., struct UTn {};
#define make_tuple_test_fakes(dummy, n, unused)														\
	struct BOOST_PP_CAT(UT,n) {};
	// Generate 15 fake types
	BOOST_PP_REPEAT(16, make_tuple_test_fakes, ~);
#undef make_tuple_test_fakes

	// tuple_first_n_t
	
	// static_assert(std::is_same_v<void, tuple_first_n_t<n,std::tuple<>>>);
#define tuple_first_n_t__returns_void_for_empty_tuple(dummy, n, unused)								\
	static_assert(std::is_same_v<void, tuple_first_n_t<n,std::tuple<>>>);
	// Test operations for 1 <= N <= 15
	// eg. static_assert(std::is_same_v<void, tuple_first_n_t<n,std::tuple<>>>);
	BOOST_PP_REPEAT(16, tuple_first_n_t__returns_void_for_empty_tuple, ~);
#undef tuple_first_n_t__returns_void_for_empty_tuple
	
	// static_assert(std::is_same_v<std::tuple<T0,..,Tn>, tuple_first_n_t<1..N,std::tuple<T0,..,Tn,Tn+1>>>);
#define tuple_first_n_t__returns_first_n_types(dummy, n, unused)									\
	static_assert(std::is_same_v<std::tuple<BOOST_PP_ENUM_PARAMS(n,UT)>,							\
		tuple_first_n_t<n,std::tuple<BOOST_PP_ENUM_PARAMS(n,UT), BOOST_PP_CAT(UT,n)>>>);
	// Test operations for 1 <= N <= 15
	// eg. static_assert(std::is_same_v<std::tuple<UT0>, tuple_first_n_t<1,std::tuple<UT0, UT1>>>);
	BOOST_PP_REPEAT_FROM_TO(1, 16, tuple_first_n_t__returns_first_n_types, ~);
#undef tuple_first_n_t__returns_first_n_types

	// tuple_last_n_t

	// static_assert(std::is_same_v<void, tuple_last_n_t<n,std::tuple<>>>);
#define tuple_last_n_t__returns_void_for_empty_tuple(dummy, n, unused)								\
	static_assert(std::is_same_v<void, tuple_last_n_t<n,std::tuple<>>>);
	// Test operations for 1 <= N <= 15
	// eg. static_assert(std::is_same_v<void, tuple_last_n_t<n,std::tuple<>>>);
	BOOST_PP_REPEAT(16, tuple_last_n_t__returns_void_for_empty_tuple, ~);
#undef tuple_last_n_t__returns_void_for_empty_tuple
	
	// static_assert(std::is_same_v<std::tuple<T1,..,Tn,Tn+1>, tuple_last_n_t<1..N,std::tuple<T0,..,Tn,Tn+1>>>);
#define tuple_last_n_t__returns_last_n_types(dummy, n, unused)										\
	static_assert(std::is_same_v<std::tuple<BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(n),UT)>,		\
		tuple_last_n_t<n,std::tuple<BOOST_PP_ENUM_PARAMS(n,UT), BOOST_PP_CAT(UT,n)>>>);
	// Test operations for 1 <= N <= 15
	// eg. static_assert(std::is_same_v<std::tuple<UT1>, tuple_last_n_t<1,std::tuple<UT0, UT1>>>);
	BOOST_PP_REPEAT_FROM_TO(1, 15, tuple_last_n_t__returns_last_n_types, ~);
#undef tuple_last_n_t__returns_last_n_types
}
