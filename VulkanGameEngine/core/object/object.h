#ifndef OBJECT_H
#define OBJECT_H

#include "object_id.h"
#include "../string/string_types.h"
#include "../typedefs.h"

#define OBJECT_DEF(m_class,m_inherited)  \
public : \
virtual const String32& get_class_name() const noexcept override { static String32 g_className = _USTR(m_class); return g_className;}		 \
virtual const String32& get_inherited_class_name() const noexcept override {static String32 g_inheritedClassName = _USTR(m_inherited); return g_inheritedClassName;}	 \
private:   

class Object
{
public:
	virtual ~Object() = default;

	virtual const String32& get_class_name() const noexcept = 0;
	virtual const String32& get_inherited_class_name() const noexcept = 0;

	virtual const String32& get_object_name() const noexcept
	{
		static String32 g_nullObjectName(U"UNKNOWN"); 
		return g_nullObjectName;
	}


	_INLINE_ bool operator==(const Object& pObj) const { return m_instance_id == pObj.m_instance_id; }
	_INLINE_ bool operator!=(const Object& pObj) const { return m_instance_id != pObj.m_instance_id; }
	_INLINE_ bool operator<(const Object& pObj) const { return m_instance_id < pObj.m_instance_id; }

private:
	ObjectID m_instance_id;
};

#endif