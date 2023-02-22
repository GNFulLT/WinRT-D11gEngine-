#ifndef DYNAMIC_LIBRARY
#define DYNAMIC_LIBRARY

#include "../string/string_types.h"
#include "../typedefs.h"
#include <any>
#include <boost/dll.hpp>
#include <boost/dll/import.hpp>

class DynamicLibrary
{
public:
	/*
	static DynamicLibrary* load(const String& fullPath,
		String& errorString);
	static DynamicLibrary* load(const String16& fullPath,
		String& errorString);
	*/
	static DynamicLibrary* load_if_exist(const String& fullPath, const String& funcName,
		String& errorString);
	static DynamicLibrary* load_if_exist(const String16& fullPath, const String& funcName,
		String& errorString);
	static DynamicLibrary* load_if_exist(const std::wstring& fullPath, const String& funcName,
		String& errorString);
	~DynamicLibrary();
	
	_INLINE_ bool has_function(const String& name)
	{
		return m_handle->has(name);
	}

	_INLINE_ bool is_loaded()
	{
		return m_handle->is_loaded();
	}

	template<typename T>
	_INLINE_ T get_function(const String& name)
	{
		try {
			void* ptr = m_handle->get<void()>(name);
			return (T)ptr;
		}
		catch (_UNUSED_ const std::exception& ex)
		{
			return nullptr;
		}
	}
	

private:
	DynamicLibrary();
private:
	boost::dll::shared_library* m_handle;
};

#endif //DYNAMIC_LIBRARY