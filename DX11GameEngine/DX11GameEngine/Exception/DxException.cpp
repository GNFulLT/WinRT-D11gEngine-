#include "DxException..h"


namespace GNF::Exception
{
	DxException::DxException(HRESULT hr, const char* functionName, const char* fileName,int lineNum) : ErrorCode(hr),Filename(fileName),FunctionName(functionName),LineNumber(lineNum)
	{

	}

    char* DxException::what()
    {
        return (char*)"DxException\0";
    }

	std::string DxException::ToString() const
	{
        LPTSTR errorText = NULL;

        FormatMessage(
            // use system message tables to retrieve error text
            FORMAT_MESSAGE_FROM_SYSTEM
            // allocate buffer on local heap for error text
            | FORMAT_MESSAGE_ALLOCATE_BUFFER
            // Important! will fail otherwise, since we're not 
            // (and CANNOT) pass insertion parameters
            | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
            ErrorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&errorText,  // output 
            0, // minimum size for output buffer
            NULL);   // arguments - see note 
        
        std::string str = "[FILE]\n" + *Filename;
        str += "\n[FUNC NAME]\n" + *FunctionName;
        str += "\n[LINE]\n" + LineNumber;
        str += "\n[HRESULT TO STR]\n" + *errorText;
       
        return str;
	}
}