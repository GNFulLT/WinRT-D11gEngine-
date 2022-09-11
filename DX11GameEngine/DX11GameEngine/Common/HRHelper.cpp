#include "HRHelper.h"

namespace GNF::Common
{

	
    

	void ThrowIfFailed(HRESULT hr,const char* functionName,const char* fileName,int lineNumber)
	{
		if (!FAILED(hr))
			return;
		throw Exception::DxException(hr,functionName,fileName,lineNumber);
    }
    

	
}