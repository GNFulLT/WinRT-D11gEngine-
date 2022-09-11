#pragma once

namespace GNF::Common::Bindable
{

	class IBindable
	{
		public:
			virtual void Bind() = 0;
			virtual ~IBindable() = default;
	};

}