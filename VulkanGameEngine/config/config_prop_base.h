#ifndef CONFIG_PROP_BASE
#define CONFIG_PROP_BASE

#include "../core/object/object.h"

class ConfigPropBase : Object
{
	OBJECT_DEF(ConfigPropBase, Object)
public:
	virtual ~ConfigPropBase() = default;
private:
};

#endif // CONFIG_PROP_BASE
