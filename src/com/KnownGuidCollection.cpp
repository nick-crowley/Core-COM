#include "com/KnownGuidCollection.h"
using namespace core;

com::KnownGuidCollection::KnownGuidCollection() {
	::GUID constexpr
	static IID_IIdentityUnmarshall = {0x0000001BL, 0, 0, {0xC0,0,0,0,0,0,0,0x46}};

#define _emplaceGuidNamePair(iid)  \
	this->emplace(iid, #iid);

	_emplaceGuidNamePair(IID_AsyncIMultiQI);
	_emplaceGuidNamePair(IID_AsyncIPipeByte);
	_emplaceGuidNamePair(IID_AsyncIPipeDouble);
	_emplaceGuidNamePair(IID_AsyncIPipeLong);
	_emplaceGuidNamePair(IID_IActivationFilter);
	_emplaceGuidNamePair(IID_IAddrExclusionControl);
	_emplaceGuidNamePair(IID_IAddrTrackingControl);
	_emplaceGuidNamePair(IID_IAgileObject);
	_emplaceGuidNamePair(IID_IAgileReference);
	_emplaceGuidNamePair(IID_IAgileReference);
	_emplaceGuidNamePair(IID_IAsyncManager);
	_emplaceGuidNamePair(IID_IAsyncRpcChannelBuffer);
	_emplaceGuidNamePair(IID_IBindCtx);
	_emplaceGuidNamePair(IID_ICallbackWithNoReentrancyToApplicationSTA);
	_emplaceGuidNamePair(IID_ICallFactory);
	_emplaceGuidNamePair(IID_ICancelMethodCalls);
	_emplaceGuidNamePair(IID_IChannelHook);
	_emplaceGuidNamePair(IID_IClassFactory);
	_emplaceGuidNamePair(IID_IClassFactory2);
	_emplaceGuidNamePair(IID_IClientSecurity);
	_emplaceGuidNamePair(IID_IComThreadingInfo);
	_emplaceGuidNamePair(IID_IEnumString);
	_emplaceGuidNamePair(IID_IEnumUnknown);
	_emplaceGuidNamePair(IID_IExternalConnection);
	_emplaceGuidNamePair(IID_IFastRundown);
	_emplaceGuidNamePair(IID_IGlobalInterfaceTable);
	_emplaceGuidNamePair(IID_IGlobalOptions);
	_emplaceGuidNamePair(IID_IDfReserved1);
	_emplaceGuidNamePair(IID_IDfReserved2);
	_emplaceGuidNamePair(IID_IDfReserved3);
	_emplaceGuidNamePair(IID_IInternalMoniker);
	_emplaceGuidNamePair(IID_IInternalUnknown);
	_emplaceGuidNamePair(IID_ILockBytes);
	_emplaceGuidNamePair(IID_IMachineGlobalObjectTable);
	_emplaceGuidNamePair(IID_IMalloc);
	_emplaceGuidNamePair(IID_IMarshal);
	_emplaceGuidNamePair(IID_IMarshal2);
	_emplaceGuidNamePair(IID_IMarshalingStream);
	_emplaceGuidNamePair(IID_IMessageFilter);
	_emplaceGuidNamePair(IID_IMoniker);
	_emplaceGuidNamePair(IID_IMultiQI);
	_emplaceGuidNamePair(IID_INoMarshal);
	_emplaceGuidNamePair(IID_IPipeByte);
	_emplaceGuidNamePair(IID_IPipeDouble);
	_emplaceGuidNamePair(IID_IPipeLong);
	_emplaceGuidNamePair(IID_IProcessInitControl);
	_emplaceGuidNamePair(IID_IPSFactory);
	_emplaceGuidNamePair(IID_IPSFactoryBuffer);
	_emplaceGuidNamePair(IID_IReleaseMarshalBuffers);
	_emplaceGuidNamePair(IID_IRpcChannel);
	_emplaceGuidNamePair(IID_IRpcChannelBuffer);
	_emplaceGuidNamePair(IID_IRpcChannelBuffer2);
	_emplaceGuidNamePair(IID_IRpcChannelBuffer3);
	_emplaceGuidNamePair(IID_IRpcHelper);
	_emplaceGuidNamePair(IID_IRpcOptions);
	_emplaceGuidNamePair(IID_IRpcProxy);
	_emplaceGuidNamePair(IID_IRpcProxyBuffer);
	_emplaceGuidNamePair(IID_IProxyManager);
	_emplaceGuidNamePair(IID_IRootStorage);
	_emplaceGuidNamePair(IID_IRpcStub);
	_emplaceGuidNamePair(IID_IRpcStubBuffer);
	_emplaceGuidNamePair(IID_IRpcSyntaxNegotiate);
	_emplaceGuidNamePair(IID_IRunningObjectTable);
	_emplaceGuidNamePair(IID_ISequentialStream);
	_emplaceGuidNamePair(IID_IServerSecurity);
	_emplaceGuidNamePair(IID_IStdMarshalInfo);
	_emplaceGuidNamePair(IID_IStorage);
	_emplaceGuidNamePair(IID_IStream);
	_emplaceGuidNamePair(IID_IStubManager);
	_emplaceGuidNamePair(IID_ISupportAllowLowerTrustActivation);
	_emplaceGuidNamePair(IID_ISurrogate);
	_emplaceGuidNamePair(IID_ISynchronize);
	_emplaceGuidNamePair(IID_ISynchronizeContainer);
	_emplaceGuidNamePair(IID_ISynchronizeEvent);
	_emplaceGuidNamePair(IID_ISynchronizeHandle);
	_emplaceGuidNamePair(IID_ISynchronizeMutex);
	_emplaceGuidNamePair(IID_IUnknown);
	_emplaceGuidNamePair(IID_IWaitMultiple);
	_emplaceGuidNamePair(IID_IConnectionPoint);
#undef _emplaceGuidNamePair
}

bool
operator<(::GUID const& l, ::GUID const& r) {
	return l.Data1 < r.Data1
		&& l.Data2 < r.Data2
		&& l.Data3 < r.Data3
		&& l.Data4 < r.Data4;
}
