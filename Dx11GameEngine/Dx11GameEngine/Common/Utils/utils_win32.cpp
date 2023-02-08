#include "pch.h"


#include "utils_win32.h"

#include <format>

namespace GNF::Common::Utils
{
	void ThrowIfFailed(HRESULT hr, const char* functionName, const char* fileName, int lineNumber)
	{
		if (!FAILED(hr))
			return;
		Common::Logger::LogCritical(std::format("There is an error occured on \n FILE : {} \n LINE : {} \n FUNC : {}", fileName,lineNumber,functionName).c_str());
		LPSTR messageBuffer = nullptr; 
			size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
				NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL); 
		std::string message(messageBuffer, size); 
		LocalFree(messageBuffer); 
		Common::Logger::LogError(message.c_str());
		throw std::runtime_error("There is an unknown error occurred");
	}
	void CheckAnyError(const char* functionName, const char* fileName, int lineNumber)
	{
		auto err = GetLastError(); 
		if (err != 0) 
		{ 
			LPSTR messageBuffer = nullptr; 
			size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
					NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL); 
			std::string message(messageBuffer, size); 
			LocalFree(messageBuffer); 
			Common::Logger::LogCritical(std::format("There is an error before on \n FILE : {} \n LINE : {} \n FUNC : {}", fileName, lineNumber, functionName).c_str());
			Common::Logger::LogError(message.c_str());
			SetLastError(0); 
		}
	} 
}
