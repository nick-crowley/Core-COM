#include "com/Guid.h"
#include "com/Function.h"
#include "core/ToString.h"
using namespace core;

namespace
{
	auto constexpr 
	coCreateGuid = com::function<1>(::CoCreateGuid);

	auto constexpr 
	stringFromCLSID = com::function<1>(::StringFromCLSID);

	auto constexpr 
	clsIdFromString = com::function<1>(::CLSIDFromString);

	auto constexpr 
	clsIdFromProgId = com::function<1>(::CLSIDFromProgID);
}

constexpr 
com::Guid::Guid(::GUID const& g) noexcept 
  : m_value{g}
{
}

//Guid::reference constexpr
//Guid::operator=(::GUID const& g) noexcept
//{
//  FIXME: Consuming modules receive 'LNK2001 unresolved external symbol' unless defined in header
//	this->m_value = g;
//	return *this;
//}

//constexpr
//Guid::operator ::GUID const&() const noexcept
//{
//  FIXME: Consuming modules receive 'LNK2001 unresolved external symbol' unless defined in header
//	return this->m_value;
//}

		
com::Guid 
com::Guid::fromProgId(std::wstring_view str)
{
	return Guid{ clsIdFromProgId(str.data()) };
}

com::Guid 
com::Guid::fromString(std::wstring_view str)
{
	return Guid{ clsIdFromString(str.data()) };
}

com::Guid
com::Guid::generate()
{
	return Guid{ coCreateGuid() };
}

com::wstring
com::Guid::wstr() const
{
    return wstring{adopt, stringFromCLSID(this->m_value)};
}

bool 
com::Guid::operator==(type const& r) const noexcept 
{
	return ::IsEqualGUID(this->m_value, r.m_value);
}

bool 
com::Guid::operator!=(type const& r) const noexcept 
{
	return !(*this == r);
}

std::string
com::to_string(Guid const& g)
{
	return core::to_string(static_cast<std::wstring_view>(g.wstr()));
}

std::wstring
com::to_wstring(Guid const& g)
{
	return std::wstring{static_cast<std::wstring_view>(g.wstr())};
}

com::Guid
com::literals::operator""_guid(gsl::cwzstring guid, size_t count)
{
	return Guid::fromString({guid, guid+count});
}

std::string 
to_string(::GUID const& g)
{
	return core::to_string(::to_wstring(g));
}

std::wstring 
to_wstring(::GUID const& g)
{
#define _DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	GUID constexpr name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

#define _DEFINE_OLEGUID(name, l, w1, w2) _DEFINE_GUID(name, l, w1, w2, 0xC0,0,0,0,0,0,0,0x46)

	static _DEFINE_OLEGUID(IID_IIdentityUnmarshall, 0x0000001BL, 0, 0);

#define _makeIfStatement(iid)  if (g == iid) return TEXT(#iid)
	_makeIfStatement(IID_AsyncIMultiQI);
	_makeIfStatement(IID_AsyncIPipeByte);
	_makeIfStatement(IID_AsyncIPipeDouble);
	_makeIfStatement(IID_AsyncIPipeLong);
	_makeIfStatement(IID_IActivationFilter);
	_makeIfStatement(IID_IAddrExclusionControl);
	_makeIfStatement(IID_IAddrTrackingControl);
	_makeIfStatement(IID_IAgileObject);
	_makeIfStatement(IID_IAgileReference);
	_makeIfStatement(IID_IAgileReference);
	_makeIfStatement(IID_IAsyncManager);
	_makeIfStatement(IID_IAsyncRpcChannelBuffer);
	_makeIfStatement(IID_IBindCtx);
	_makeIfStatement(IID_ICallbackWithNoReentrancyToApplicationSTA);
	_makeIfStatement(IID_ICallFactory);
	_makeIfStatement(IID_ICancelMethodCalls);
	_makeIfStatement(IID_IChannelHook);
	_makeIfStatement(IID_IClassFactory);
	_makeIfStatement(IID_IClassFactory2);
	_makeIfStatement(IID_IClientSecurity);
	_makeIfStatement(IID_IComThreadingInfo);
	_makeIfStatement(IID_IEnumString);
	_makeIfStatement(IID_IEnumUnknown);
	_makeIfStatement(IID_IExternalConnection);
	_makeIfStatement(IID_IFastRundown);
	_makeIfStatement(IID_IGlobalInterfaceTable);
	_makeIfStatement(IID_IGlobalOptions);
	_makeIfStatement(IID_IDfReserved1);
	_makeIfStatement(IID_IDfReserved2);
	_makeIfStatement(IID_IDfReserved3);
	_makeIfStatement(IID_IInternalMoniker);
	_makeIfStatement(IID_IInternalUnknown);
	_makeIfStatement(IID_ILockBytes);
	_makeIfStatement(IID_IMachineGlobalObjectTable);
	_makeIfStatement(IID_IMalloc);
	_makeIfStatement(IID_IMarshal);
	_makeIfStatement(IID_IMarshal2);
	_makeIfStatement(IID_IMarshalingStream);
	_makeIfStatement(IID_IMessageFilter);
	_makeIfStatement(IID_IMoniker);
	_makeIfStatement(IID_IMultiQI);
	_makeIfStatement(IID_INoMarshal);
	_makeIfStatement(IID_IPipeByte);
	_makeIfStatement(IID_IPipeDouble);
	_makeIfStatement(IID_IPipeLong);
	_makeIfStatement(IID_IProcessInitControl);
	_makeIfStatement(IID_IPSFactory);
	_makeIfStatement(IID_IPSFactoryBuffer);
	_makeIfStatement(IID_IReleaseMarshalBuffers);
	_makeIfStatement(IID_IRpcChannel);
	_makeIfStatement(IID_IRpcChannelBuffer);
	_makeIfStatement(IID_IRpcChannelBuffer2);
	_makeIfStatement(IID_IRpcChannelBuffer3);
	_makeIfStatement(IID_IRpcHelper);
	_makeIfStatement(IID_IRpcOptions);
	_makeIfStatement(IID_IRpcProxy);
	_makeIfStatement(IID_IRpcProxyBuffer);
	_makeIfStatement(IID_IProxyManager);
	_makeIfStatement(IID_IRootStorage);
	_makeIfStatement(IID_IRpcStub);
	_makeIfStatement(IID_IRpcStubBuffer);
	_makeIfStatement(IID_IRpcSyntaxNegotiate);
	_makeIfStatement(IID_IRunningObjectTable);
	_makeIfStatement(IID_ISequentialStream);
	_makeIfStatement(IID_IServerSecurity);
	_makeIfStatement(IID_IStdMarshalInfo);
	_makeIfStatement(IID_IStorage);
	_makeIfStatement(IID_IStream);
	_makeIfStatement(IID_IStubManager);
	_makeIfStatement(IID_ISupportAllowLowerTrustActivation);
	_makeIfStatement(IID_ISurrogate);
	_makeIfStatement(IID_ISynchronize);
	_makeIfStatement(IID_ISynchronizeContainer);
	_makeIfStatement(IID_ISynchronizeEvent);
	_makeIfStatement(IID_ISynchronizeHandle);
	_makeIfStatement(IID_ISynchronizeMutex);
	_makeIfStatement(IID_IUnknown);
	_makeIfStatement(IID_IWaitMultiple);
	IID_IConnectionPoint;
#undef _makeIfStatement
    return core::com::to_wstring(core::com::Guid{g});
}

bool 
operator==(::GUID const& l, com::Guid const& r) noexcept
{
	return com::Guid(l) == r;
}

bool 
operator!=(::GUID const& l, com::Guid const& r) noexcept
{
	return com::Guid(l) != r;
}

bool 
operator==(com::Guid const& l, ::GUID const& r) noexcept
{
	return l == com::Guid(r);
}

bool 
operator!=(com::Guid const& l, ::GUID const& r) noexcept
{
	return l != com::Guid(r);
}
