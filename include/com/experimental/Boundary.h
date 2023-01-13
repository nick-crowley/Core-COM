#pragma once
#include "comFramework.h"
#include "com/SetLastError.h"

namespace core::com
{
	template <typename CoClass>
	class Boundary
	{
		std::function<::HRESULT ()> m_action;

	public:
		template <std::invocable<> Callable>
		Boundary&
		operator=(Callable&& fx) {
			this->m_action = fx;
			return *this;
		}

		satisfies(Boundary,
			IsDefaultConstructible,
			NotCopyable,
			NotMovable
		);

	public:
		implicit operator 
		::HRESULT() const 
		try {
			if (!this->m_action)
				return E_NOTIMPL;

			return this->m_action();
		}
		catch (std::invalid_argument const& e) {
			return SetLastError<CoClass>{E_INVALIDARG, e.what()};
		}
		catch (std::system_error const& e) {
			return SetLastError<CoClass>{e.code().value(), e.what()};
		}
		catch (std::bad_alloc const& e) {
			return SetLastError<CoClass>{E_OUTOFMEMORY, e.what()};
		}
		catch (std::exception const& e) {
			return SetLastError<CoClass>{E_FAIL, e.what()};
		}
	};
	
	//! @brief	Code-block within a COM boundary (non-standard)
#	define co_impl	return ::core::com::Boundary<std::remove_cvref_t<decltype(*this)>>{} = [&]
}
