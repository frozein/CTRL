#include <ctrl.h>
#include <GLFW/glfw3.h>

//--------------------------------------------------------------------------------------------------------------------------------//

CTRLaction _ctrl_glfw_action_to_action(int32_t action);
uint32_t _ctrl_glfw_mods_to_mods(int32_t mods);
CTRLcode _ctrl_glfw_key_to_code(int32_t key);
CTRLcode _ctrl_glfw_mouse_button_to_code(int32_t button);

int32_t _ctrl_code_to_glfw_key(CTRLcode code);
int32_t _ctrl_code_to_glfw_mouse_button(CTRLcode code);

void _ctrl_key_callback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);
void _ctrl_mouse_button_callback(GLFWwindow* window, int32_t button, int32_t action, int32_t mods);
void _ctrl_scroll_callback(GLFWwindow* window, double offsetX, double offsetY);

GLFWwindow* g_window;

//--------------------------------------------------------------------------------------------------------------------------------//

void ctrl_init_impl_glfw(GLFWwindow* window)
{
	glfwSetKeyCallback(window, _ctrl_key_callback);
	glfwSetMouseButtonCallback(window, _ctrl_mouse_button_callback);
	glfwSetScrollCallback(window, _ctrl_scroll_callback);
	
	g_window = window;
}

void ctrl_update_impl()
{
	glfwPollEvents();
}

//--------------------------------------------------------------------------------------------------------------------------------//

uint8_t ctrl_code_held(CTRLcode code)
{
	if(code >= CTRL_SCROLL_X)
		return 0;
	else if(code >= CTRL_MOUSE_BUTTON_LEFT)
		return glfwGetMouseButton(g_window, _ctrl_code_to_glfw_mouse_button(code)) == GLFW_PRESS;
	else if(code >= CTRL_KEY_SPACE)
		return glfwGetKey(g_window, _ctrl_code_to_glfw_key(code)) == GLFW_PRESS;
	else
		return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------//

CTRLaction _ctrl_glfw_action_to_action(int32_t action)
{
	switch(action)
	{
	case GLFW_PRESS:
		return CTRL_PRESS;
	case GLFW_RELEASE:
		return CTRL_RELEASE;
	case GLFW_REPEAT:
		return CTRL_REPEAT;
	}

	return 0;
}

uint32_t _ctrl_glfw_mods_to_mods(int32_t mods)
{
	uint32_t ctrlMods = 0;

	if(mods & GLFW_MOD_SHIFT)
		ctrlMods |= CTRL_MOD_SHIFT;
	if(mods & GLFW_MOD_CONTROL)
		ctrlMods |= CTRL_MOD_CONTROL;
	if(mods & GLFW_MOD_ALT)
		ctrlMods || CTRL_MOD_ALT;

	return ctrlMods;
}

CTRLcode _ctrl_glfw_key_to_code(int32_t key)
{
	switch(key)
	{
	case GLFW_KEY_SPACE:
		return CTRL_KEY_SPACE;
	case GLFW_KEY_APOSTROPHE:
		return CTRL_KEY_APOSTROPHE;
	case GLFW_KEY_COMMA:
		return CTRL_KEY_COMMA;
	case GLFW_KEY_MINUS:
		return CTRL_KEY_MINUS;
	case GLFW_KEY_PERIOD:
		return CTRL_KEY_PERIOD;
	case GLFW_KEY_SLASH:
		return CTRL_KEY_SLASH;
	case GLFW_KEY_0:
		return CTRL_KEY_0;
	case GLFW_KEY_1:
		return CTRL_KEY_1;
	case GLFW_KEY_2:
		return CTRL_KEY_2;
	case GLFW_KEY_3:
		return CTRL_KEY_3;
	case GLFW_KEY_4:
		return CTRL_KEY_4;
	case GLFW_KEY_5:
		return CTRL_KEY_5;
	case GLFW_KEY_6:
		return CTRL_KEY_6;
	case GLFW_KEY_7:
		return CTRL_KEY_7;
	case GLFW_KEY_8:
		return CTRL_KEY_8;
	case GLFW_KEY_9:
		return CTRL_KEY_9;
	case GLFW_KEY_SEMICOLON:
		return CTRL_KEY_SEMICOLON;
	case GLFW_KEY_EQUAL:
		return CTRL_KEY_EQUAL;
	case GLFW_KEY_A:
		return CTRL_KEY_A;
	case GLFW_KEY_B:
		return CTRL_KEY_B;
	case GLFW_KEY_C:
		return CTRL_KEY_C;
	case GLFW_KEY_D:
		return CTRL_KEY_D;
	case GLFW_KEY_E:
		return CTRL_KEY_E;
	case GLFW_KEY_F:
		return CTRL_KEY_F;
	case GLFW_KEY_G:
		return CTRL_KEY_G;
	case GLFW_KEY_H:
		return CTRL_KEY_H;
	case GLFW_KEY_I:
		return CTRL_KEY_I;
	case GLFW_KEY_J:
		return CTRL_KEY_J;
	case GLFW_KEY_K:
		return CTRL_KEY_K;
	case GLFW_KEY_L:
		return CTRL_KEY_L;
	case GLFW_KEY_M:
		return CTRL_KEY_M;
	case GLFW_KEY_N:
		return CTRL_KEY_N;
	case GLFW_KEY_O:
		return CTRL_KEY_O;
	case GLFW_KEY_P:
		return CTRL_KEY_P;
	case GLFW_KEY_Q:
		return CTRL_KEY_Q;
	case GLFW_KEY_R:
		return CTRL_KEY_R;
	case GLFW_KEY_S:
		return CTRL_KEY_S;
	case GLFW_KEY_T:
		return CTRL_KEY_T;
	case GLFW_KEY_U:
		return CTRL_KEY_U;
	case GLFW_KEY_V:
		return CTRL_KEY_V;
	case GLFW_KEY_W:
		return CTRL_KEY_W;
	case GLFW_KEY_X:
		return CTRL_KEY_X;
	case GLFW_KEY_Y:
		return CTRL_KEY_Y;
	case GLFW_KEY_Z:
		return CTRL_KEY_Z;
	case GLFW_KEY_LEFT_BRACKET:
		return CTRL_KEY_LEFT_BRACKET;
	case GLFW_KEY_BACKSLASH:
		return CTRL_KEY_BACKSLASH;
	case GLFW_KEY_RIGHT_BRACKET:
		return CTRL_KEY_RIGHT_BRACKET;
	case GLFW_KEY_GRAVE_ACCENT:
		return CTRL_KEY_GRAVE_ACCENT;
	case GLFW_KEY_ESCAPE:
		return CTRL_KEY_ESCAPE;
	case GLFW_KEY_ENTER:
		return CTRL_KEY_ENTER;
	case GLFW_KEY_TAB:
		return CTRL_KEY_TAB;
	case GLFW_KEY_BACKSPACE:
		return CTRL_KEY_BACKSPACE;
	case GLFW_KEY_INSERT:
		return CTRL_KEY_INSERT;
	case GLFW_KEY_DELETE:
		return CTRL_KEY_DELETE;
	case GLFW_KEY_RIGHT:
		return CTRL_KEY_RIGHT;
	case GLFW_KEY_LEFT:
		return CTRL_KEY_LEFT;
	case GLFW_KEY_DOWN:
		return CTRL_KEY_DOWN;
	case GLFW_KEY_UP:
		return CTRL_KEY_UP;
	case GLFW_KEY_PAGE_UP:
		return CTRL_KEY_PAGE_UP;
	case GLFW_KEY_PAGE_DOWN:
		return CTRL_KEY_PAGE_DOWN;
	case GLFW_KEY_HOME:
		return CTRL_KEY_HOME;
	case GLFW_KEY_END:
		return CTRL_KEY_END;
	case GLFW_KEY_CAPS_LOCK:
		return CTRL_KEY_CAPS_LOCK;
	case GLFW_KEY_SCROLL_LOCK:
		return CTRL_KEY_SCROLL_LOCK;
	case GLFW_KEY_NUM_LOCK:
		return CTRL_KEY_NUM_LOCK;
	case GLFW_KEY_PRINT_SCREEN:
		return CTRL_KEY_PRINT_SCREEN;
	case GLFW_KEY_PAUSE:
		return CTRL_KEY_PAUSE;
	case GLFW_KEY_F1:
		return CTRL_KEY_F1;
	case GLFW_KEY_F2:
		return CTRL_KEY_F2;
	case GLFW_KEY_F3:
		return CTRL_KEY_F3;
	case GLFW_KEY_F4:
		return CTRL_KEY_F4;
	case GLFW_KEY_F5:
		return CTRL_KEY_F5;
	case GLFW_KEY_F6:
		return CTRL_KEY_F6;
	case GLFW_KEY_F7:
		return CTRL_KEY_F7;
	case GLFW_KEY_F8:
		return CTRL_KEY_F8;
	case GLFW_KEY_F9:
		return CTRL_KEY_F9;
	case GLFW_KEY_F10:
		return CTRL_KEY_F10;
	case GLFW_KEY_F11:
		return CTRL_KEY_F11;
	case GLFW_KEY_F12:
		return CTRL_KEY_F12;
	case GLFW_KEY_F13:
		return CTRL_KEY_F13;
	case GLFW_KEY_F14:
		return CTRL_KEY_F14;
	case GLFW_KEY_F15:
		return CTRL_KEY_F15;
	case GLFW_KEY_F16:
		return CTRL_KEY_F16;
	case GLFW_KEY_F17:
		return CTRL_KEY_F17;
	case GLFW_KEY_F18:
		return CTRL_KEY_F18;
	case GLFW_KEY_F19:
		return CTRL_KEY_F19;
	case GLFW_KEY_F20:
		return CTRL_KEY_F20;
	case GLFW_KEY_F21:
		return CTRL_KEY_F21;
	case GLFW_KEY_F22:
		return CTRL_KEY_F22;
	case GLFW_KEY_F23:
		return CTRL_KEY_F23;
	case GLFW_KEY_F24:
		return CTRL_KEY_F24;
	case GLFW_KEY_F25:
		return CTRL_KEY_F25;
	case GLFW_KEY_KP_0:
		return CTRL_KEY_NUMPAD_0;
	case GLFW_KEY_KP_1:
		return CTRL_KEY_NUMPAD_1;
	case GLFW_KEY_KP_2:
		return CTRL_KEY_NUMPAD_2;
	case GLFW_KEY_KP_3:
		return CTRL_KEY_NUMPAD_3;
	case GLFW_KEY_KP_4:
		return CTRL_KEY_NUMPAD_4;
	case GLFW_KEY_KP_5:
		return CTRL_KEY_NUMPAD_5;
	case GLFW_KEY_KP_6:
		return CTRL_KEY_NUMPAD_6;
	case GLFW_KEY_KP_7:
		return CTRL_KEY_NUMPAD_7;
	case GLFW_KEY_KP_8:
		return CTRL_KEY_NUMPAD_8;
	case GLFW_KEY_KP_9:
		return CTRL_KEY_NUMPAD_9;
	case GLFW_KEY_KP_DECIMAL:
		return CTRL_KEY_NUMPAD_DECIMAL;
	case GLFW_KEY_KP_DIVIDE:
		return CTRL_KEY_NUMPAD_DIVIDE;
	case GLFW_KEY_KP_MULTIPLY:
		return CTRL_KEY_NUMPAD_MULTIPLY;
	case GLFW_KEY_KP_SUBTRACT:
		return CTRL_KEY_NUMPAD_SUBTRACT;
	case GLFW_KEY_KP_ADD:
		return CTRL_KEY_NUMPAD_ADD;
	case GLFW_KEY_KP_ENTER:
		return CTRL_KEY_NUMPAD_ENTER;
	case GLFW_KEY_KP_EQUAL:
		return CTRL_KEY_NUMPAD_EQUAL;
	case GLFW_KEY_LEFT_SHIFT:
		return CTRL_KEY_LEFT_SHIFT;
	case GLFW_KEY_LEFT_CONTROL:
		return CTRL_KEY_LEFT_CONTROL;
	case GLFW_KEY_LEFT_ALT:
		return CTRL_KEY_LEFT_ALT;
	case GLFW_KEY_RIGHT_SHIFT:
		return CTRL_KEY_RIGHT_SHIFT;
	case GLFW_KEY_RIGHT_CONTROL:
		return CTRL_KEY_RIGHT_CONTROL;
	case GLFW_KEY_RIGHT_ALT:
		return CTRL_KEY_RIGHT_ALT;
	default:
		return CTRL_CODE_NONE;
	}
}

CTRLcode _ctrl_glfw_mouse_button_to_code(int32_t button)
{
	return button + CTRL_MOUSE_BUTTON_LEFT;
}

//--------------------------------------------------------------------------------------------------------------------------------//

int32_t _ctrl_code_to_glfw_key(CTRLcode code)
{
	switch(code)
	{
	case CTRL_KEY_SPACE:
		return GLFW_KEY_SPACE;
	case CTRL_KEY_APOSTROPHE:
		return GLFW_KEY_APOSTROPHE;
	case CTRL_KEY_COMMA:
		return GLFW_KEY_COMMA;
	case CTRL_KEY_MINUS:
		return GLFW_KEY_MINUS;
	case CTRL_KEY_PERIOD:
		return GLFW_KEY_PERIOD;
	case CTRL_KEY_SLASH:
		return GLFW_KEY_SLASH;
	case CTRL_KEY_0:
		return GLFW_KEY_0;
	case CTRL_KEY_1:
		return GLFW_KEY_1;
	case CTRL_KEY_2:
		return GLFW_KEY_2;
	case CTRL_KEY_3:
		return GLFW_KEY_3;
	case CTRL_KEY_4:
		return GLFW_KEY_4;
	case CTRL_KEY_5:
		return GLFW_KEY_5;
	case CTRL_KEY_6:
		return GLFW_KEY_6;
	case CTRL_KEY_7:
		return GLFW_KEY_7;
	case CTRL_KEY_8:
		return GLFW_KEY_8;
	case CTRL_KEY_9:
		return GLFW_KEY_9;
	case CTRL_KEY_SEMICOLON:
		return GLFW_KEY_SEMICOLON;
	case CTRL_KEY_EQUAL:
		return GLFW_KEY_EQUAL;
	case CTRL_KEY_A:
		return GLFW_KEY_A;
	case CTRL_KEY_B:
		return GLFW_KEY_B;
	case CTRL_KEY_C:
		return GLFW_KEY_C;
	case CTRL_KEY_D:
		return GLFW_KEY_D;
	case CTRL_KEY_E:
		return GLFW_KEY_E;
	case CTRL_KEY_F:
		return GLFW_KEY_F;
	case CTRL_KEY_G:
		return GLFW_KEY_G;
	case CTRL_KEY_H:
		return GLFW_KEY_H;
	case CTRL_KEY_I:
		return GLFW_KEY_I;
	case CTRL_KEY_J:
		return GLFW_KEY_J;
	case CTRL_KEY_K:
		return GLFW_KEY_K;
	case CTRL_KEY_L:
		return GLFW_KEY_L;
	case CTRL_KEY_M:
		return GLFW_KEY_M;
	case CTRL_KEY_N:
		return GLFW_KEY_N;
	case CTRL_KEY_O:
		return GLFW_KEY_O;
	case CTRL_KEY_P:
		return GLFW_KEY_P;
	case CTRL_KEY_Q:
		return GLFW_KEY_Q;
	case CTRL_KEY_R:
		return GLFW_KEY_R;
	case CTRL_KEY_S:
		return GLFW_KEY_S;
	case CTRL_KEY_T:
		return GLFW_KEY_T;
	case CTRL_KEY_U:
		return GLFW_KEY_U;
	case CTRL_KEY_V:
		return GLFW_KEY_V;
	case CTRL_KEY_W:
		return GLFW_KEY_W;
	case CTRL_KEY_X:
		return GLFW_KEY_X;
	case CTRL_KEY_Y:
		return GLFW_KEY_Y;
	case CTRL_KEY_Z:
		return GLFW_KEY_Z;
	case CTRL_KEY_LEFT_BRACKET:
		return GLFW_KEY_LEFT_BRACKET;
	case CTRL_KEY_BACKSLASH:
		return GLFW_KEY_BACKSLASH;
	case CTRL_KEY_RIGHT_BRACKET:
		return GLFW_KEY_RIGHT_BRACKET;
	case CTRL_KEY_GRAVE_ACCENT:
		return GLFW_KEY_GRAVE_ACCENT;
	case CTRL_KEY_ESCAPE:
		return GLFW_KEY_ESCAPE;
	case CTRL_KEY_ENTER:
		return GLFW_KEY_ENTER;
	case CTRL_KEY_TAB:
		return GLFW_KEY_TAB;
	case CTRL_KEY_BACKSPACE:
		return GLFW_KEY_BACKSPACE;
	case CTRL_KEY_INSERT:
		return GLFW_KEY_INSERT;
	case CTRL_KEY_DELETE:
		return GLFW_KEY_DELETE;
	case CTRL_KEY_RIGHT:
		return GLFW_KEY_RIGHT;
	case CTRL_KEY_LEFT:
		return GLFW_KEY_LEFT;
	case CTRL_KEY_DOWN:
		return GLFW_KEY_DOWN;
	case CTRL_KEY_UP:
		return GLFW_KEY_UP;
	case CTRL_KEY_PAGE_UP:
		return GLFW_KEY_PAGE_UP;
	case CTRL_KEY_PAGE_DOWN:
		return GLFW_KEY_PAGE_DOWN;
	case CTRL_KEY_HOME:
		return GLFW_KEY_HOME;
	case CTRL_KEY_END:
		return GLFW_KEY_END;
	case CTRL_KEY_CAPS_LOCK:
		return GLFW_KEY_CAPS_LOCK;
	case CTRL_KEY_SCROLL_LOCK:
		return GLFW_KEY_SCROLL_LOCK;
	case CTRL_KEY_NUM_LOCK:
		return GLFW_KEY_NUM_LOCK;
	case CTRL_KEY_PRINT_SCREEN:
		return GLFW_KEY_PRINT_SCREEN;
	case CTRL_KEY_PAUSE:
		return GLFW_KEY_PAUSE;
	case CTRL_KEY_F1:
		return GLFW_KEY_F1;
	case CTRL_KEY_F2:
		return GLFW_KEY_F2;
	case CTRL_KEY_F3:
		return GLFW_KEY_F3;
	case CTRL_KEY_F4:
		return GLFW_KEY_F4;
	case CTRL_KEY_F5:
		return GLFW_KEY_F5;
	case CTRL_KEY_F6:
		return GLFW_KEY_F6;
	case CTRL_KEY_F7:
		return GLFW_KEY_F7;
	case CTRL_KEY_F8:
		return GLFW_KEY_F8;
	case CTRL_KEY_F9:
		return GLFW_KEY_F9;
	case CTRL_KEY_F10:
		return GLFW_KEY_F10;
	case CTRL_KEY_F11:
		return GLFW_KEY_F11;
	case CTRL_KEY_F12:
		return GLFW_KEY_F12;
	case CTRL_KEY_F13:
		return GLFW_KEY_F13;
	case CTRL_KEY_F14:
		return GLFW_KEY_F14;
	case CTRL_KEY_F15:
		return GLFW_KEY_F15;
	case CTRL_KEY_F16:
		return GLFW_KEY_F16;
	case CTRL_KEY_F17:
		return GLFW_KEY_F17;
	case CTRL_KEY_F18:
		return GLFW_KEY_F18;
	case CTRL_KEY_F19:
		return GLFW_KEY_F19;
	case CTRL_KEY_F20:
		return GLFW_KEY_F20;
	case CTRL_KEY_F21:
		return GLFW_KEY_F21;
	case CTRL_KEY_F22:
		return GLFW_KEY_F22;
	case CTRL_KEY_F23:
		return GLFW_KEY_F23;
	case CTRL_KEY_F24:
		return GLFW_KEY_F24;
	case CTRL_KEY_F25:
		return GLFW_KEY_F25;
	case CTRL_KEY_NUMPAD_0:
		return GLFW_KEY_KP_0;
	case CTRL_KEY_NUMPAD_1:
		return GLFW_KEY_KP_1;
	case CTRL_KEY_NUMPAD_2:
		return GLFW_KEY_KP_2;
	case CTRL_KEY_NUMPAD_3:
		return GLFW_KEY_KP_3;
	case CTRL_KEY_NUMPAD_4:
		return GLFW_KEY_KP_4;
	case CTRL_KEY_NUMPAD_5:
		return GLFW_KEY_KP_5;
	case CTRL_KEY_NUMPAD_6:
		return GLFW_KEY_KP_6;
	case CTRL_KEY_NUMPAD_7:
		return GLFW_KEY_KP_7;
	case CTRL_KEY_NUMPAD_8:
		return GLFW_KEY_KP_8;
	case CTRL_KEY_NUMPAD_9:
		return GLFW_KEY_KP_9;
	case CTRL_KEY_NUMPAD_DECIMAL:
		return GLFW_KEY_KP_DECIMAL;
	case CTRL_KEY_NUMPAD_DIVIDE:
		return GLFW_KEY_KP_DIVIDE;
	case CTRL_KEY_NUMPAD_MULTIPLY:
		return GLFW_KEY_KP_MULTIPLY;
	case CTRL_KEY_NUMPAD_SUBTRACT:
		return GLFW_KEY_KP_SUBTRACT;
	case CTRL_KEY_NUMPAD_ADD:
		return GLFW_KEY_KP_ADD;
	case CTRL_KEY_NUMPAD_ENTER:
		return GLFW_KEY_KP_ENTER;
	case CTRL_KEY_NUMPAD_EQUAL:
		return GLFW_KEY_KP_EQUAL;
	case CTRL_KEY_LEFT_SHIFT:
		return GLFW_KEY_LEFT_SHIFT;
	case CTRL_KEY_LEFT_CONTROL:
		return GLFW_KEY_LEFT_CONTROL;
	case CTRL_KEY_LEFT_ALT:
		return GLFW_KEY_LEFT_ALT;
	case CTRL_KEY_RIGHT_SHIFT:
		return GLFW_KEY_RIGHT_SHIFT;
	case CTRL_KEY_RIGHT_CONTROL:
		return GLFW_KEY_RIGHT_CONTROL;
	case CTRL_KEY_RIGHT_ALT:
		return GLFW_KEY_RIGHT_ALT;
	default:
		return GLFW_KEY_UNKNOWN;
	}
}

int32_t _ctrl_code_to_glfw_mouse_button(CTRLcode code)
{
	return code - CTRL_MOUSE_BUTTON_LEFT;
}

//--------------------------------------------------------------------------------------------------------------------------------//

void _ctrl_key_callback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
	CTRLinput input;
	input.code = _ctrl_glfw_key_to_code(key);
	input.action = _ctrl_glfw_action_to_action(action);
	input.dir = 0.0f;
	input.mods = _ctrl_glfw_mods_to_mods(mods);

	ctrl_push_input(input);
}

void _ctrl_mouse_button_callback(GLFWwindow* window, int32_t button, int32_t action, int32_t mods)
{
	CTRLinput input;
	input.code = _ctrl_glfw_mouse_button_to_code(button);
	input.action = _ctrl_glfw_action_to_action(action);
	input.dir = 0.0f;
	input.mods = _ctrl_glfw_mods_to_mods(mods);

	ctrl_push_input(input);
}

void _ctrl_scroll_callback(GLFWwindow* window, double offsetX, double offsetY)
{
	uint32_t mods = 0;
	mods |= (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT  ) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT  )) * CTRL_MOD_SHIFT;
	mods |= (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) * CTRL_MOD_CONTROL;
	mods |= (glfwGetKey(window, GLFW_KEY_LEFT_ALT    ) || glfwGetKey(window, GLFW_KEY_RIGHT_ALT    )) * CTRL_MOD_ALT;

	if(offsetX != 0.0f)
	{
		CTRLinput input;
		input.code = CTRL_SCROLL_X;
		input.action = CTRL_PRESS | CTRL_REPEAT | CTRL_HOLD | CTRL_RELEASE;
		input.dir = (float)offsetX;

		ctrl_push_input(input);
	}

	if(offsetY != 0.0f)
	{
		CTRLinput input;
		input.code = CTRL_SCROLL_Y;
		input.action = CTRL_PRESS | CTRL_REPEAT | CTRL_HOLD | CTRL_RELEASE;
		input.dir = (float)offsetY;

		ctrl_push_input(input);
	}
}