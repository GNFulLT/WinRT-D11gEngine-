#pragma once
#include <Windows.h> 
#include <stdio.h>
#include <exception>
#include <string>
#include "Exception/DxException..h"
namespace GNF::Common
{

   


	#define GThrowIfFailed(hr) ThrowIfFailed(hr,__FUNCTION__,__FILE__,__LINE__)
	
    

	void ThrowIfFailed(HRESULT hr, const char* functionName, const char* fileName, int lineNumber);
    

	
}