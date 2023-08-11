# CTRL
CTRL is a dead simple library for managing input and controls for video games. It allows you to simply define a list of controls and their associated inputs on startup, and it will call a callback function whenever one of them is triggered. It is nothing more than a wrapper on top of a context creation library (only GLFW is supported for now) designed for improved ease of game programming.

## How to Use

A simple use case may look like this:
```cpp
#include <ctrl.h>
#include "my_game.h"

enum MyGameControls
{
	JUMP,
	SPRINT,
	SHOOT,
	COMBO
};

void input_callback(uint32_t tag, CTRLcode code, CTRLaction action, uint32_t mods, float dir, void* userData)
{
	switch(tag)
	{
	case JUMP:
		my_game_jump();
		break;
	case SPRINT:
		my_game_set_sprinting();
		break;
	case SHOOT:
		my_game_shoot();
		break;
	case COMBO:
		my_game_super_combo();
		break;
	}
}

int main()
{
	ctrl_init_impl_glfw(myWindow);
	ctrl_set_callback(input_callback, &myGameState);

	CTRLgroup mainGroup;

	//JUMP control will be triggered with the spacebar is pressed
	ctrl_add_control(&mainGroup, ctrl_control(JUMP, CTRL_KEY_SPACE, CTRL_PRESS, CTRL_MOD_ANY));
	//SPRINT control will be triggered with the left shift key is held
	ctrl_add_control(&mainGroup, ctrl_control(SPRINT, CTRL_KEY_LEFT_SHIFT, CTRL_HOLD, CTRL_MOD_ANY));
	//SHOOT control will be triggered with the left mouse key is pressed
	ctrl_add_control(&mainGroup, ctrl_control(JUMP, CTRL_MOUSE_BUTTON_LEFT, CTRL_PRESS, CTRL_MOD_ANY));
	//COMBO control will be triggered when ctrl + q is pressed OR repeated
	ctrl_add_control(&mainGroup, ctrl_control(COMBO, CTRL_KEY_Q, CTRL_PRESS | CTRL_REPEAT, CTRL_MOD_CONTROL));

	CTRLgroup activeGroups[1] = {&mainGroup};
	ctrl_set_groups(1, activeGroups);

	while(1)
	{
		ctrl_process_input();

		my_game_update();
		my_game_render();
	}

	ctrl_free_group(mainGroup);
	return 0;
}
```

More detailed documentaion can be found in `ctrl.h`

## Building
The included `CMakeLists.txt` can be used to generate project files. It currently is set to build to a static library, but simply change `add_library(${PROJECT_NAME} STATIC ${ctrl_src})` to `add_executable(${PROJECT_NAME} ${ctrl_src})` if you wish to build an executable.
