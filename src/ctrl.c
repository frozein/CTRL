#include <ctrl.h>

//--------------------------------------------------------------------------------------------------------------------------------//

#define CTRL_MAX_GROUPS 32
#define CTRL_MAX_INPUTS 64

static uint32_t g_numGroups = 0;
static CTRLgroup* g_groups[CTRL_MAX_GROUPS];

static uint32_t g_numInputs = 0;
static CTRLinput g_inputs[CTRL_MAX_INPUTS];

static void (*g_input_callback)(uint32_t, CTRLcode, CTRLaction, float, void*) = NULL;
static void* g_inputCallbackUserData                                          = NULL;

static CTRLcontrol* g_controlToSet                              = NULL;
static CTRLcode g_cancelControlSetCode                          = CTRL_CODE_NONE;
static void (*g_control_set_callback)(uint8_t, CTRLcode, void*) = NULL;
static void* g_controlSetCallbackUserData                       = NULL;

//--------------------------------------------------------------------------------------------------------------------------------//

void ctrl_set_callback(void (*input_callback)(uint32_t, CTRLcode, CTRLaction, float, void*), void* userData)
{
	g_input_callback = input_callback;
	g_inputCallbackUserData = userData;
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
	if(g_controlToSet && (input.action & CTRL_PRESS))
	{
		uint8_t set = input.code != g_cancelControlSetCode;
		if(set)
			g_controlToSet->code = input.code;

		if(g_control_set_callback)
			g_control_set_callback(set, input.code, g_controlSetCallbackUserData);

		g_controlToSet = NULL;
		g_control_set_callback = NULL;
		g_controlSetCallbackUserData = NULL;

		return;
	}

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

	uint32_t holdMods = 0;
	holdMods |= (ctrl_code_held(CTRL_KEY_LEFT_SHIFT  ) || ctrl_code_held(CTRL_KEY_RIGHT_SHIFT  )) * CTRL_MOD_SHIFT;
	holdMods |= (ctrl_code_held(CTRL_KEY_LEFT_CONTROL) || ctrl_code_held(CTRL_KEY_RIGHT_CONTROL)) * CTRL_MOD_CONTROL;
	holdMods |= (ctrl_code_held(CTRL_KEY_LEFT_ALT    ) || ctrl_code_held(CTRL_KEY_RIGHT_ALT    )) * CTRL_MOD_ALT;

	for(uint32_t g = 0; g < g_numGroups; g++)
	{
		for(uint32_t i = 0; i < g_groups[g]->numControls; i++)
		{
			uint32_t parent = i;
			while(g_groups[g]->controls[parent].inherit && g_groups[g]->controls[parent].parentIdx != CTRL_INVALID_INDEX)
				parent = g_groups[g]->controls[parent].parentIdx;

			uint32_t actions = g_groups[g]->controls[i].actions;
			CTRLcode code = g_groups[g]->controls[parent].code;
			uint32_t mods = g_groups[g]->controls[parent].mods;

			if(actions & CTRL_HOLD)
			{
				if(ctrl_code_held(code) && (mods == CTRL_MOD_ANY || mods == holdMods))
					g_input_callback(g_groups[g]->controls[i].tag, code, CTRL_HOLD, 0.0f, g_inputCallbackUserData);

				if(actions == CTRL_HOLD)
					continue;
			}

			for(uint32_t j = 0; j < g_numInputs; j++)
			{
				if(code == g_inputs[j].code && (actions & g_inputs[j].action) && (mods == CTRL_MOD_ANY || mods == g_inputs[j].mods))
					g_input_callback(g_groups[g]->controls[i].tag, code, g_inputs[j].action, g_inputs[j].dir, g_inputCallbackUserData);
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

	return group;
}

void ctrl_free_group(CTRLgroup group)
{
	CTRL_FREE(group.controls);
}

//--------------------------------------------------------------------------------------------------------------------------------//

CTRLcontrol ctrl_control(uint32_t tag, CTRLcode code, uint32_t actions, uint32_t mods)
{
	CTRLcontrol control;
	control.tag = tag;
	control.inherit = 0;
	control.code = code;
	control.actions = actions;
	control.mods = mods;

	return control;
}

CTRLcontrol ctrl_control_inherit(CTRLgroup parentGroup, uint32_t parentTag, uint32_t tag, uint32_t actions)
{
	CTRLcontrol control;
	control.tag = tag;
	control.inherit = 1;
	control.parentIdx = CTRL_INVALID_INDEX;
	control.actions = actions;

	for(uint32_t i = 0; i < parentGroup.numControls; i++)
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
	group->controls[group->numControls++] = control;
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
	while(i < group->numControls && group->controls[i].tag != tag)
		i++;

	if(i >= group->numControls)
	{
		CTRLcontrol invalidControl;
		invalidControl.tag = CTRL_INVALID_TAG;

		return invalidControl;
	}
	else
		return group->controls[i];
}

void ctrl_set_control(CTRLgroup* group, uint32_t tag, CTRLcode newCode, uint32_t newActions, uint32_t newMods)
{
	uint32_t i = 0;
	while(i < group->numControls && group->controls[i].tag != tag)
		i++;

	if(i >= group->numControls)
		return;
	
	if(newCode != CTRL_CODE_NONE)
	{
		group->controls[i].inherit = 0;
		group->controls[i].code = newCode;
		group->controls[i].mods = newMods;
	}
	if(newActions != 0)
		group->controls[i].actions = newActions;
}

//--------------------------------------------------------------------------------------------------------------------------------//

void ctrl_set_control_to_next_input(CTRLgroup* group, uint32_t tag, CTRLcode cancelCode, void (*control_set_callback)(uint8_t, CTRLcode, void*), void* userData)
{
	uint32_t i = 0;
	while(i < group->numControls && group->controls[i].tag != tag)
		i++;
	
	if(i >= group->numControls)
		return;

	group->controls[i].inherit = 0;
	g_controlToSet = &group->controls[i];
	g_cancelControlSetCode = cancelCode;
	g_control_set_callback = control_set_callback;
	g_controlSetCallbackUserData = userData;
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