#include "ctrlcodes.h"
#include <stdint.h>

//--------------------------------------------------------------------------------------------------------------------------------//

#define CTRL_INVALID_TAG   UINT32_MAX
#define CTRL_INVALID_INDEX UINT32_MAX

typedef struct CTRLcontrol
{
	uint32_t tag;

	uint8_t inherit;
	union
	{
		CTRLcode code;
		uint32_t parentIdx; //index into the group's controls array where the parent is located, NOT the parent's tag
	};
	
	uint32_t actions;
} CTRLcontrol;

typedef struct CTRLgroup
{
	uint32_t numControls;
	uint32_t arraySize;
	CTRLcontrol* controls;
} CTRLgroup;

typedef struct CTRLinput
{
	CTRLcode code;
	CTRLaction action;
	float dir; //for scrolling / joystick input
} CTRLinput;

//--------------------------------------------------------------------------------------------------------------------------------//

#if !defined(CTRL_MALLOC) || !defined(CTRL_FREE) || !defined(CTRL_REALLOC)
	#include <malloc.h>
#endif

#ifndef CTRL_MALLOC
	#define CTRL_MALLOC(size) malloc(size)
#endif

#ifndef CTRL_FREE
	#define CTRL_FREE(ptr) free(ptr)
#endif

#ifndef CTRL_REALLOC
	#define CTRL_REALLOC(ptr, size) realloc(ptr, size)
#endif

//--------------------------------------------------------------------------------------------------------------------------------//

typedef struct GLFWwindow GLFWwindow;
void ctrl_init_impl_glfw(GLFWwindow* window);

void ctrl_update_impl();

void ctrl_set_callback(void (*callback)(uint32_t, CTRLcode, CTRLaction, float, void*), void* userData);
void ctrl_set_groups(uint32_t numGroups, CTRLgroup** groups);

void ctrl_push_input(CTRLinput input);
uint8_t ctrl_code_held(CTRLcode code);
void ctrl_process_input();

CTRLgroup ctrl_create_group();
void ctrl_free_group(CTRLgroup group);

CTRLcontrol ctrl_control(uint32_t tag, CTRLcode code, uint32_t actions);
CTRLcontrol ctrl_control_inherit(CTRLgroup parentGroup, uint32_t parentTag, uint32_t tag, uint32_t actions);

void ctrl_add_control(CTRLgroup* group, CTRLcontrol control);
CTRLcontrol ctrl_get_control(CTRLgroup* group, uint32_t tag);
void ctrl_remove_control(CTRLgroup* group, uint32_t tag);
void ctrl_set_control(CTRLgroup* group, uint32_t tag, CTRLcode newCode, uint32_t newActions);

const char* ctrl_get_code_name(CTRLcode code);