#pragma once

#include <vector>
#include "boost/assert.hpp"
#include "boost/static_assert.hpp"
#include "boost/align/detail/is_aligned.hpp"
#include "Core/VertexBuffer/OnlyVertexBuffer.h"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/functional/hash.hpp>

namespace GNF::Common::Utils
{
	inline std::string DeleteLastSpaces(const char* pStr)
	{
		std::string str(pStr);
		int lastIndex = ((int)str.size()) - 1;
		while (lastIndex >= 0 && str[lastIndex] == ' ')
			lastIndex--;

		return str.substr(0, lastIndex + 1);
	}

	inline std::string DeleteLastSpaces(const std::string& str)
	{
		int last = ((int)str.size()) - 1;
		while (last >= 0 && str[last] == ' ')
			--last;
		return str.substr(0, last + 1);
	}

	inline unsigned long long CreateUniqueID()
	{
		return boost::hash_value(boost::uuids::random_generator()());	
	}

	//!: std::enable_if_t<(boost::alignment::is_aligned(16, structPtrs ) && ...)>* = true,
	/*
	template<typename... Args,typename... Sizes>
	const void* MergeStructs(Args&&... structPtrs, Sizes&&... sizes)
	{
		(static_assert(boost::alignment::is_aligned<16>(structPtrs), "Struct must be alignas(16)"), ...);

		// Allocate memory for the merged struct
		constexpr size_t totalSize = (sizes + ...);
		void* mergedStruct = new char[totalSize];
		char* current = static_cast<char*>(mergedStruct);

		// Use a fold expression to iterate over the struct pointers and sizes
		((memcpy(current, structPtrs, sizes), current += sizes), ...);

		return mergedStruct;
	}
	*/
	inline const void* MergeStructs(const std::vector<const void*>& structPtrs, const std::vector<size_t>& sizes)
	{
		BOOST_ASSERT_MSG(structPtrs.size() != sizes.size(),"Count of Structs and Count of Sizes must be equal");
		
		for (int i = 0; i < structPtrs.size(); i++)
		{
			(BOOST_ASSERT_MSG(boost::alignment::is_aligned(16,structPtrs[0]), "Struct must be alignas(16)"));
		}

		return nullptr;
	}
}