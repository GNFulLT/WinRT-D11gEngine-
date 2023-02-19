#ifndef WINDOW_ID_H
#define WINDOW_ID_H


#include "../core/typedefs.h"

class WindowID
{
public:
	_INLINE_ bool is_valid() const noexcept { return m_id != 0; }
	_INLINE_ bool is_null() const noexcept { return m_id == 0; }
	_INLINE_ operator uint64_t() const noexcept { return m_id; }
	_INLINE_ operator int64_t() const noexcept { return m_id; }

	_INLINE_ bool operator==(const WindowID& pId) const { return m_id == pId.m_id; }
	_INLINE_ bool operator!=(const WindowID& pId) const { return m_id != pId.m_id; }
	_INLINE_ bool operator<(const WindowID& pId) const { return m_id < pId.m_id; }

	_INLINE_ void operator=(int64_t pInt64) { m_id = pInt64; }
	_INLINE_ void operator=(uint64_t pUint64) { m_id = pUint64; }

	_INLINE_ WindowID() noexcept {}
	_INLINE_ explicit WindowID(const uint64_t pId) noexcept { m_id = pId; }
	_INLINE_ explicit WindowID(const int64_t pId) noexcept { m_id = pId; }

private:	



	uint64_t m_id = 0;
};

#endif //WINDOW_ID_H