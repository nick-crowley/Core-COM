#pragma once
#include "library/core.COM.h"
#include "com/CoClassTraits.h"
#include "win/RegistryKey.h"
#include "com/Boolean.h"
#include "com/Guid.h"
#include "com/Function.h"
#include "com/SharedPtr.h"

namespace core::com
{
	template <meta::CoreCoClass CoClass>
	class ClassObject
	{
		auto constexpr
		inline static coRegisterClassObject = function<1>(::CoRegisterClassObject);

	private:
		DWORD m_cookie;

	public:
		ClassObject(::IUnknown& instance, ::CLSCTX context, ::REGCLS usage)
			: m_cookie{coRegisterClassObject(guid_v<CoClass>, &instance, context, usage)}
		{}

		~ClassObject() {
			::CoRevokeClassObject(this->m_cookie);
		}

		satisfies(ClassObject,
			NotDefaultConstructible,
			NotCopyable,
			NotMovable,
			NotEqualityComparable,
			NotSortable
		);
	};

	void
	inline resumeClassObjects() 
	{
		com::ThrowingHResult hr = ::CoResumeClassObjects();
	}
}