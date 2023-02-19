#ifndef OBJECT_ID_H
#define OBJECT_ID_H

#include "../typedefs.h"

// RTTI Helper
class ObjectID
{
public:
	_INLINE_ bool is_valid() const noexcept { return m_id != 0; }
	_INLINE_ bool is_null() const noexcept { return m_id == 0; }
	_INLINE_ operator uint64_t() const noexcept { return m_id; }
	_INLINE_ operator int64_t() const noexcept { return m_id; }
	
	_INLINE_ bool operator==(const ObjectID& pId) const { return m_id == pId.m_id; }
	_INLINE_ bool operator!=(const ObjectID& pId) const { return m_id != pId.m_id; }
	_INLINE_ bool operator<(const ObjectID& pId) const { return m_id < pId.m_id; }

	_INLINE_ void operator=(int64_t pInt64) { m_id = pInt64; }
	_INLINE_ void operator=(uint64_t pUint64) { m_id = pUint64; }

	_INLINE_ ObjectID() noexcept {}
	_INLINE_ explicit ObjectID(const uint64_t pId) noexcept { m_id = pId; }
	_INLINE_ explicit ObjectID(const int64_t pId) noexcept { m_id = pId; }

private:
	uint64_t m_id = 0;
};

#endif