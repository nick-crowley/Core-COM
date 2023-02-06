#pragma once
#include "library/core.COM.h"

namespace core::com
{
	class ComExport IsolatedApartment
	{
	public:
		IsolatedApartment();

		~IsolatedApartment() {
			::CoUninitialize();
		}

		satisfies(IsolatedApartment,
			NotCopyable,
			NotMovable,
			NotEqualityComparable,
			NotSortable
		);
	};
	
	class ComExport SharedApartment
	{
	public:
		SharedApartment();

		~SharedApartment() {
			::CoUninitialize();
		}

		satisfies(SharedApartment,
			NotCopyable,
			NotMovable,
			NotEqualityComparable,
			NotSortable
		);
	};
}
