#ifndef SIZE_H
#define SIZE_H


struct UVec2
{
	union
	{
		struct {
			unsigned int x;
			unsigned int y;
		};
		unsigned int v[2];
	};
	UVec2() : UVec2(0, 0)
	{

	}
	UVec2(unsigned int x, unsigned int y) : x(x),y(y)
	{

	}
};

#endif
