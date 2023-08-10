#include <ctrl.h>

//--------------------------------------------------------------------------------------------------------------------------------//

#define CTRL_MAX_GROUPS 32
#define CTRL_MAX_INPUTS 64

static uint32_t g_numGroups = 0;
static CTRLgroup* g_groups[CTRL_MAX_GROUPS];

static uint32_t g_numInputs = 0;
static CTRLinput g_inputs[CTRL_MAX_INPUTS];

static void (*g_input_callback)(uint32_t, CTRLcode, CTRLaction, float, void*);
static void* g_callbackUserData;

//--------------------------------------------------------------------------------------------------------------------------------//

void ctrl_set_callback(void (*callback)(uint32_t, CTRLcode, CTRLaction, float, void*), void* userData)
{
	g_input_callback = callback;
	g_callbackUserData = userData;
}

void ctrl_set_groups(uint32_t numGroups, CTRLgroup** groups)
{
	if(numGroups > CTRL_MAX_GROUPS)
		numGroups = CTRL_MAX_GROUPS;
	
	g_numGroups = numGroups;
	for(uint32_t i = 0; i < numGroups; i++)
		g_groups[i] = groups[i];
}

//--------------------------------------------------------------------------------------------------------------------------------//

void ctrl_push_input(CTRLinput input)
{
	if(g_numInputs >= CTRL_MAX_INPUTS)
		return;
	
	g_inputs[g_numInputs++] = input;
}

void ctrl_process_input()
{
	ctrl_update_impl();
	if(!g_input_callback)
	{
		g_numInputs = 0;
		return;
	}

	for(uint32_t g = 0; g < g_numGroups; g++)
	{
		for(uint32_t i = 0; i < g_groups[g]->arraySize; i++)
		{
			if(g_groups[g]->controls[i].tag == CTRL_INVALID_TAG)
				continue;

			uint32_t parent = i;
			while(g_groups[g]->controls[parent].inherit && g_groups[g]->controls[parent].parentIdx != CTRL_INVALID_INDEX)
				parent = g_groups[g]->controls[parent].parentIdx;

			CTRLaction actions = g_groups[g]->controls[i].actions;
			CTRLcode code = g_groups[g]->controls[parent].code;

			if(actions & CTRL_HOLD)
			{
				if(ctrl_code_held(code))
					g_input_callback(g_groups[g]->controls[i].tag, code, CTRL_HOLD, 0.0f, g_callbackUserData);

				if(actions == CTRL_HOLD)
					continue;
			}

			for(uint32_t j = 0; j < g_numInputs; j++)
			{
				if(code == g_inputs[j].code && (actions & g_inputs[j].action))
					g_input_callback(g_groups[g]->controls[i].tag, code, g_inputs[j].action, g_inputs[j].dir, g_callbackUserData);
			}
		}
	}

	g_numInputs = 0;
}

//--------------------------------------------------------------------------------------------------------------------------------//

CTRLgroup ctrl_create_group()
{
	const uint32_t startSize = 4;

	CTRLgroup group;
	group.numControls = 0;
	group.arraySize = startSize;
	group.controls = CTRL_MALLOC(group.arraySize * sizeof(CTRLcontrol));

	for(uint32_t i = 0; i < group.arraySize; i++)
		group.controls[i].tag = CTRL_INVALID_TAG;

	return group;
}

void ctrl_free_group(CTRLgroup group)
{
	CTRL_FREE(group.controls);
}

//--------------------------------------------------------------------------------------------------------------------------------//

CTRLcontrol ctrl_control(uint32_t tag, CTRLcode code, uint32_t actions)
{
	CTRLcontrol control;
	control.tag = tag;
	control.inherit = 0;
	control.code = code;
	control.actions = actions;

	return control;
}

CTRLcontrol ctrl_control_inherit(CTRLgroup parentGroup, uint32_t parentTag, uint32_t tag, uint32_t actions)
{
	CTRLcontrol control;
	control.tag = tag;
	control.inherit = 1;
	control.parentIdx = CTRL_INVALID_INDEX;
	control.actions = actions;

	for(uint32_t i = 0; i < parentGroup.arraySize; i++)
		if(parentGroup.controls[i].tag == parentTag)
		{
			control.parentIdx = i;
			break;
		}
	
	return control;
}

//--------------------------------------------------------------------------------------------------------------------------------//

void ctrl_add_control(CTRLgroup* group, CTRLcontrol control)
{
	uint32_t i = 0;
	while(i < group->arraySize && group->controls[i].tag != CTRL_INVALID_TAG)
		i++;
	
	if(i >= group->arraySize)
		return;

	group->controls[i] = control;
	group->numControls++;
	if(group->numControls >= group->arraySize)
	{
		group->arraySize *= 2;
		group->controls = CTRL_REALLOC(group->controls, group->arraySize * sizeof(CTRLcontrol));

		for(uint32_t i = group->numControls; i < group->arraySize; i++)
			group->controls[i].tag = CTRL_INVALID_TAG;
	}
}

CTRLcontrol ctrl_get_control(CTRLgroup* group, uint32_t tag)
{
	uint32_t i = 0;
	while(i < group->arraySize && group->controls[i].tag != tag)
		i++;

	if(i >= group->arraySize)
	{
		CTRLcontrol invalidControl;
		invalidControl.tag = CTRL_INVALID_TAG;

		return invalidControl;
	}
	else
		return group->controls[i];
}

void ctrl_remove_control(CTRLgroup* group, uint32_t tag)
{
	uint32_t i = 0;
	while(i < group->arraySize && group->controls[i].tag != tag)
		i++;
	
	if(i < group->arraySize)
	{
		group->controls[i].tag = CTRL_INVALID_TAG;
		group->numControls--;
	}
}

void ctrl_set_control(CTRLgroup* group, uint32_t tag, CTRLcode newCode, uint32_t newActions)
{
	uint32_t i = 0;
	while(i < group->arraySize && group->controls[i].tag != tag)
		i++;

	if(i >= group->arraySize)
		return;
	
	if(newCode != CTRL_CODE_NONE)
	{
		group->controls[i].inherit = 0;
		group->controls[i].code = newCode;
	}
	if(newActions != 0)
		group->controls[i].actions = newActions;
}

//--------------------------------------------------------------------------------------------------------------------------------//

const char* CODE_NAMES[CTRL_CODE_COUNT] = {
	"NONE",

	"SPACE",
	"APOSTROPHE",
	"COMMA",
	"MINUS",
	"PERIOD",
	"SLASH",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"SEMICOLON",
	"EQUAL",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"LEFT BRACKET",
	"BACKSLASH",
	"RIGHT BRACKET",
	"GRAVE ACCENT",
	"ESCAPE",
	"ENTER",
	"TAB",
	"BACKSPACE",
	"INSERT",
	"DELETE",
	"RIGHT",
	"LEFT",
	"DOWN",
	"UP",
	"PAGE UP",
	"PAGE DOWN",
	"HOME",
	"END",
	"CAPS LOCK",
	"SCROLL LOCK",
	"NUM LOCK",
	"PRINT SCREEN",
	"PAUSE",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
	"F16",
	"F17",
	"F18",
	"F19",
	"F20",
	"F21",
	"F22",
	"F23",
	"F24",
	"F25",
	"NUMPAD 0",
	"NUMPAD 1",
	"NUMPAD 2",
	"NUMPAD 3",
	"NUMPAD 4",
	"NUMPAD 5",
	"NUMPAD 6",
	"NUMPAD 7",
	"NUMPAD 8",
	"NUMPAD 9",
	"NUMPAD DECIMAL",
	"NUMPAD DIVIDE",
	"NUMPAD MULTIPLY",
	"NUMPAD SUBTRACT",
	"NUMPAD ADD",
	"NUMPAD ENTER",
	"NUMPAD EQUAL",
	"LEFT SHIFT",
	"LEFT CONTROL",
	"LEFT ALT",
	"RIGHT SHIFT",
	"RIGHT CONTROL",
	"RIGHT ALT",

	"LEFT MOUSE BUTTON",
	"RIGHT MOUSE BUTTON",
	"MIDDLE MOUSE BUTTON",
	"MOUSE BUTTON 4",
	"MOUSE BUTTON 5",
	"MOUSE BUTTON 6",
	"MOUSE BUTTON 7",
	"MOUSE BUTTON 8",

	"SCROLL HORIZONTAL",
	"SCROLL VERTICAL"
};

const char* ctrl_get_code_name(CTRLcode code)
{
	if(code < CTRL_CODE_COUNT)
		return CODE_NAMES[code];
	else
		return "NONE";
}