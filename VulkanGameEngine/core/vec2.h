#ifndef VEC2_H
#define VEC2_H

namespace core
{
	struct UVec2
	{
		unsigned int x;
		unsigned int y;
		
		UVec2() : UVec2(0,0)
		{}

		UVec2(unsigned int x,unsigned int y) : x(x),y(y)
		{}
	};
}

#endif