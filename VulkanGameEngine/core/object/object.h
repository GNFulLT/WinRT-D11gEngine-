#ifndef OBJECT_H
#define OBJECT_h

#include "object_id.h"
#include "../string/string_types.h"
#include "../typedefs.h"

#define OBJECT_DEF(m_class,m_inherited) \
public : \
virtual const String32& get_class_name() {return g_className;}		 \
private : \
_INLINE_ static String32 g_className = _STR(m_class) \

class Object
{
public:



	virtual const String32& get_class_name() = 0;
	virtual const String32& get_inherited_class_name() = 0;
	virtual const String32& get_object_name() = 0;


	_INLINE_ bool operator==(const Object& pObj) const { return m_instance_id == pObj.m_instance_id; }
	_INLINE_ bool operator!=(const Object& pObj) const { return m_instance_id != pObj.m_instance_id; }
	_INLINE_ bool operator<(const Object& pObj) const { return m_instance_id < pObj.m_instance_id; }

private:
	ObjectID m_instance_id;
};

#endif