/* o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o */ /*!
* @copyright	Copyright (c) 2023, Nick Crowley. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions
*    and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other materials provided
*    with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
* WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* The views and conclusions contained in the software and documentation are those of the author 
* and should not be interpreted as representing official policies, either expressed or implied, of
* the projects which contain it.
*/
// o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o Header Files o~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~-~=~o
#include "com/KnownGuidCollection.h"
#include "com/Guid.h"
using namespace core;

com::KnownGuidCollection
com::KnownGuids{};

com::KnownGuidCollection::KnownGuidCollection() {
	using namespace com::literals;

	::GUID constexpr
	static IID_IIdentityUnmarshall = {0x0000001BL, 0, 0, {0xC0,0,0,0,0,0,0,0x46}};
	
	Guid constexpr
	static IID_IApplicationFrame = "{143715D9-A015-40EA-B695-D5CC267E36EE}"_guid;

	Guid constexpr
	static IID_IApplicationFrameManager = "{D6DEFAB3-DBB9-4413-8AF9-554586FDFF94}"_guid;
	
	Guid constexpr
	static IID_IApplicationFrameEventHandler = "{EA5D0DE4-770D-4DA0-A9F8-D7F9A140FF79}"_guid;
	
	Guid constexpr
	static IID_IAudioDeviceGraph = "{3C169FF7-37B2-484C-B199-C3155590F316}"_guid;

	Guid constexpr
	static IID_IInspectable = "{AF86E2E0-B12D-4C6A-9C5A-D7AA65101E90}"_guid;
	
	Guid constexpr
	static IID_IPimcContext2 = "{1868091E-AB5A-415F-A02F-5C4DD0CF901D}"_guid;
	
	Guid constexpr
	static IID_IEUserBroker = "{1AC7516E-E6BB-4A69-B63F-E841904DC5A6}"_guid;

	Guid constexpr
	static IID_IFrameTaskManager = "{35BD3360-1B35-4927-BAE4-B10E70D99EFF}"_guid;
	
	Guid constexpr
	static IID_IStreamGroup = "{816E5B3E-5523-4EFC-9223-98EC4214C3A0}"_guid;
	
	Guid constexpr
	static IID_IVerbStateTaskCallBack = "{F2153260-232E-4474-9D0A-9F2AB153441D}"_guid;
	
#define _emplaceGuidNamePair(iid)  \
	this->emplace(iid, #iid);
	
	_emplaceGuidNamePair(IID_IApplicationFrame);
	_emplaceGuidNamePair(IID_IApplicationFrameManager);
	_emplaceGuidNamePair(IID_IApplicationFrameEventHandler);
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
	_emplaceGuidNamePair(IID_IAudioDeviceGraph);
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
	_emplaceGuidNamePair(IID_IFrameTaskManager);
	_emplaceGuidNamePair(IID_IGlobalInterfaceTable);
	_emplaceGuidNamePair(IID_IGlobalOptions);
	_emplaceGuidNamePair(IID_IIdentityUnmarshall);
	_emplaceGuidNamePair(IID_IInspectable);
	_emplaceGuidNamePair(IID_IDfReserved1);
	_emplaceGuidNamePair(IID_IDfReserved2);
	_emplaceGuidNamePair(IID_IDfReserved3);
	_emplaceGuidNamePair(IID_IEUserBroker);
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
	_emplaceGuidNamePair(IID_IPimcContext2);
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
	_emplaceGuidNamePair(IID_IStreamGroup);
	_emplaceGuidNamePair(IID_IStubManager);
	_emplaceGuidNamePair(IID_ISupportAllowLowerTrustActivation);
	_emplaceGuidNamePair(IID_ISurrogate);
	_emplaceGuidNamePair(IID_ISynchronize);
	_emplaceGuidNamePair(IID_ISynchronizeContainer);
	_emplaceGuidNamePair(IID_ISynchronizeEvent);
	_emplaceGuidNamePair(IID_ISynchronizeHandle);
	_emplaceGuidNamePair(IID_ISynchronizeMutex);
	_emplaceGuidNamePair(IID_IUnknown);
	_emplaceGuidNamePair(IID_IVerbStateTaskCallBack);
	_emplaceGuidNamePair(IID_IWaitMultiple);
	_emplaceGuidNamePair(IID_IConnectionPoint);
#undef _emplaceGuidNamePair
}
