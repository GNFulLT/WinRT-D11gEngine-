#pragma once

#define SUPPORTED_KEY_NUM 256
#define SUPPORTED_MOUSE_KEY_NUM 3
#define REGISTER_VC(vc) {vc,#vc}
namespace GNF::Common::Windowing::VC
{
	enum MOUSE_CODE
	{
		MOUSE_LEFT,
		MOUSE_RIGHT,
		MOUSE_WHEEL
	};

	static std::unordered_map<MOUSE_CODE, const char*> mouse_map = {
		REGISTER_VC(MOUSE_LEFT),
		REGISTER_VC(MOUSE_RIGHT),
		REGISTER_VC(MOUSE_WHEEL)
	};

	enum KEY_CODE
	{
		KEY_LEFT = 0x25,
		KEY_UP,
		KEY_RIGHT,
		KEY_DOWN,
		KEY_0 = 0x30,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_A = 0x41,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		KEY_SHIFT = 0x10,
		KEY_ENTER = 0x0D
	};

	static std::unordered_map<KEY_CODE, const char*> map = {
		REGISTER_VC(KEY_ENTER),
		REGISTER_VC(KEY_SHIFT),
		REGISTER_VC(KEY_LEFT),
		REGISTER_VC(KEY_UP),
		REGISTER_VC(KEY_RIGHT),
		REGISTER_VC(KEY_DOWN),
		REGISTER_VC(KEY_0),
		REGISTER_VC(KEY_1),
		REGISTER_VC(KEY_2),
		REGISTER_VC(KEY_3),
		REGISTER_VC(KEY_4),
		REGISTER_VC(KEY_5),
		REGISTER_VC(KEY_6),
		REGISTER_VC(KEY_7),
		REGISTER_VC(KEY_8),
		REGISTER_VC(KEY_9),
		REGISTER_VC(KEY_A),
		REGISTER_VC(KEY_B),
		REGISTER_VC(KEY_C),
		REGISTER_VC(KEY_D),
		REGISTER_VC(KEY_E),
		REGISTER_VC(KEY_F),
		REGISTER_VC(KEY_G),
		REGISTER_VC(KEY_H),
		REGISTER_VC(KEY_I),
		REGISTER_VC(KEY_J),
		REGISTER_VC(KEY_K),
		REGISTER_VC(KEY_L),
		REGISTER_VC(KEY_M),
		REGISTER_VC(KEY_N),
		REGISTER_VC(KEY_O),
		REGISTER_VC(KEY_P),
		REGISTER_VC(KEY_Q),
		REGISTER_VC(KEY_R),
		REGISTER_VC(KEY_S),
		REGISTER_VC(KEY_T),
		REGISTER_VC(KEY_U),
		REGISTER_VC(KEY_V),
		REGISTER_VC(KEY_W),
		REGISTER_VC(KEY_X),
		REGISTER_VC(KEY_Y),
		REGISTER_VC(KEY_Z)
	};

	static const char* VK_TO_STR(KEY_CODE code)
	{
		if (map.find(code) != map.end())
			return map[code];
		else
			return "UNKNOWN_KEY";
	}

	static const char* VK_TO_STR(MOUSE_CODE code)
	{
		if (mouse_map.find(code) != mouse_map.end())
			return mouse_map[code];
		else
			return "UNKNOWN_MOUSE_KEY";
	}
}