#include "dynamic_library.h"
	

#include "io_utils.h"
#include "../string/string_utils.h"
#include  <boost/dll/alias.hpp>
/*
DynamicLibrary* DynamicLibrary::load(const String16& fullPath,
	String& errorString)
{
	bool pathExist = is_path_exist(fullPath);

	if (!pathExist)
	{
		errorString = String("There is no file/folder like that");
		return nullptr;
	}

	//X TODO : Logger
	try
	{
		boost::dll::shared_library* dll = new boost::dll::shared_library(string16_to_wstr(fullPath));
		DynamicLibrary* lib = new DynamicLibrary();
		lib->m_handle = dll;
		return lib;
	}
	catch (std::exception&)
	{
		errorString = String("Unknown exception while trying to load dll");
		return nullptr;
	}
}
*/
DynamicLibrary* DynamicLibrary::load_if_exist(const String& fullPath, const String& funcName,String& errorString)
{
	return load_if_exist(string_to_wstr(fullPath), funcName, errorString);
}

DynamicLibrary* DynamicLibrary::load_if_exist(const String16& fullPath, const String& funcName, String& errorString)
{
	return load_if_exist(string16_to_wstr(fullPath),funcName,errorString);
}

DynamicLibrary* DynamicLibrary::load_if_exist(const std::wstring& fullPath, const String& funcName, String& errorString)
{
	bool pathExist = is_path_exist(fullPath);

	if (!pathExist)
	{
		errorString = String("There is no file/folder like that");
		return nullptr;
	}

	try
	{
		boost::dll::library_info inf(fullPath);

		for (const auto& st : inf.symbols())
		{
			const auto str = st;
			int a = 5;
		}

		boost::dll::shared_library* lib = new boost::dll::shared_library(fullPath);
			
		if (!lib->has(funcName))
		{
			errorString = String("Couldn't find func in dll");
			delete lib;
			return nullptr;
		}
		
		DynamicLibrary* dll = new DynamicLibrary();
		dll->m_handle = lib;
		return dll;
		
	}
	catch (std::exception&)
	{
		errorString = String("Unknown exception");
		return nullptr;
	}
}

/*
DynamicLibrary* DynamicLibrary::load(const String& path,String& errorString)
{
	bool pathExist = is_path_exist(path);
	if (!pathExist)
	{
		errorString = String("There is no file/folder like that");
		return nullptr;
	}
	//X TODO : Logger
	try
	{
		boost::dll::shared_library* dll = new boost::dll::shared_library(path);
		DynamicLibrary* lib = new DynamicLibrary();
		lib->m_handle = dll;


		return lib;
	}
	catch (std::exception&)
	{
		errorString = String("Unknown exception while trying to load dll");
		return nullptr;
	}
}
*/
DynamicLibrary::DynamicLibrary()
{

}
DynamicLibrary::~DynamicLibrary()
{
	m_handle->unload();
	delete m_handle;
}