#include <stdio.h>
#include <GLFW/glfw3.h>
#include "ctrl.h"

#define CONTROL_PARENT 0
#define CONTROL_1 1
#define CONTROL_2 2
#define CONTROL_3 3
#define CONTROL_4 4

void input_callback(uint32_t control, CTRLcode code, CTRLaction action, float dir, void* userData)
{
	switch(control)
	{
	case CONTROL_PARENT:
		printf("parent\n");
		break;
	case CONTROL_1:
		printf("1\n");
		break;
	case CONTROL_2:
		printf("2\n");
		break;
	case CONTROL_3:
		printf("3\n");
		break;
	case CONTROL_4:
		printf("%s\n", ctrl_get_code_name(code));
		break;
	}
}

void set_callback(CTRLcode code, void* userData)
{
	printf("set\n");
}

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "CTRL", NULL, NULL);
	
	ctrl_init_impl_glfw(window);
	CTRLgroup mainGroup = ctrl_create_group();

	CTRLgroup* groups[] = {&mainGroup};
	ctrl_set_callback(input_callback, NULL);
	ctrl_set_groups(1, groups);

	ctrl_add_control(&mainGroup, ctrl_control(CONTROL_PARENT, CTRL_KEY_Q, CTRL_PRESS | CTRL_RELEASE));
	ctrl_add_control(&mainGroup, ctrl_control(CONTROL_1, CTRL_MOUSE_BUTTON_LEFT, CTRL_HOLD));
	ctrl_add_control(&mainGroup, ctrl_control(CONTROL_2, CTRL_SCROLL_Y, CTRL_PRESS | CTRL_REPEAT));

	ctrl_add_control(&mainGroup, ctrl_control_inherit(mainGroup, CONTROL_PARENT, CONTROL_3, CTRL_REPEAT));
	ctrl_add_control(&mainGroup, ctrl_control_inherit(mainGroup, CONTROL_PARENT, CONTROL_4, CTRL_REPEAT));
	
	ctrl_set_control_to_next_input(&mainGroup, CONTROL_PARENT, set_callback, NULL);

	while(!glfwWindowShouldClose(window))
	{
		ctrl_process_input();
	}
	
	ctrl_free_group(mainGroup);

	glfwTerminate();
	return 0;
}