#pragma once

#include <exception>
#include <Windows.h>
#include <string>
namespace GNF::Exception
{
	class DxException : public std::exception
	{
		public:
			DxException() = default;
			DxException(HRESULT hr,const char* , const char*,int );

			std::string ToString() const;
			char* what();

			HRESULT ErrorCode = S_OK;
			const char* FunctionName;
			const char* Filename;
			int LineNumber = -1;
	};
}