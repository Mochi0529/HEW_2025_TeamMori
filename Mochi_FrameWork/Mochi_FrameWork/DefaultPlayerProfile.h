#pragma once
#include "IInputProfile.h"


namespace MochiFramework::InputSystem {


	class DefaultPlayerProfile : public IInputProfile {
	public:
		void Apply(InputMap& map) const override;
	};


} // namespace MochiFramework::InputSystem