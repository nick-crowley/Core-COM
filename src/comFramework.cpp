#include "ComFramework.h"
#include "win/DWord.h"
#include "com/Exception.h"
#include "nt/Exception.h"
#include "win/Exception.h"
#include <ntsecapi.h>

std::string 
static formatMessage(int err)
{
	std::string msg(128,'\0');
	msg.resize(
		::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, err, NULL, msg.data(), win::DWord{msg.capacity()}, nullptr)
	);
	msg.erase(msg.find_last_not_of("\r\n\t "));
	return msg;
}

void 
com::throw_exception(::HRESULT hr, std::source_location loc)
{
	throw std::runtime_error{formatMessage(hr)};
}

void 
nt::throw_exception(::NTSTATUS r)
{
	throw std::runtime_error{formatMessage(::LsaNtStatusToWinError(r))};
}

void 
win::throw_exception(::LRESULT r)
{
	throw std::runtime_error{formatMessage(r)};
}

void 
win::throw_exception(::LRESULT r, std::string message)
{
	throw std::runtime_error{message + "." + formatMessage(r)};
}
