

#pragma once
#include "../C++/Functional/includes.h"
#include "../C++/Functional/functional.h"



namespace saints {
	
	void Kill(const std::string& name);

	void Controls(sur::Object* woods, sur::Object* sky, f32 speed);

	void Jump(sur::Object* woods, sur::Object* sky, f32 speed, bool up);

	void Gravity(sur::Master* obj, f32* m, bool allowed);

	void WallpaperScroll(sur::Object& obj, bool horizontalonly);

	void Portals();
}