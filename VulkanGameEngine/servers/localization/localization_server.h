#ifndef LOCALIZATION_SERVER_H
#define LOCALIZATION_SERVER_H

#include "../../core/typedefs.h"
#include "../../core/object/object.h"
#include "../../core/string/string_types.h"
#include "../../core/string/locales.h"
#include "config/config.h"

#include <unordered_map>
#include "exception/file_not_found_ex.h"
class LocalizationServer : public Object
{
	OBJECT_DEF(LocalizationServer,Object)
public:
	LocalizationServer()
	{
	}
	_INLINE_ LocalizationServer* get_instance()
	{
		return singleton;
	}
	virtual void init();	
	virtual void load_localization(const String16& path);
	virtual const String32* get_str(const String& id) const noexcept;
private:
	std::unordered_map<size_t, String32> m_localizationMap;
	const char* m_currentLocalization[2] = {nullptr,nullptr};

private:
	_INLINE_ static LocalizationServer* singleton;
	_INLINE_ static String32 g_nullString = U"???????";
};


#endif // LOCALIZATION_SERVER_H