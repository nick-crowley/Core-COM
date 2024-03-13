#pragma once

// NB: Add library-specific Boost dependencies here
#include <boost/mpl/remove_if.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>

namespace mpl = boost::mpl;

namespace boost::mpl
{
	template <typename Sequence>
	using front_t = typename front<Sequence>::type;

	template <typename Sequence, typename UnaryPredicate>
	using remove_if_t = typename remove_if<Sequence,UnaryPredicate>::type;
}