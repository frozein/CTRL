#ifndef CTRL_H
#define CTRL_H

#if __cplusplus
extern "C"
{
#endif

#include "ctrlcodes.h"
#include <stdint.h>

//--------------------------------------------------------------------------------------------------------------------------------//

#define CTRL_INVALID_TAG   UINT32_MAX
#define CTRL_INVALID_INDEX UINT32_MAX

//a single control, relating a gameplay event ("tag") to an input
typedef struct CTRLcontrol
{
	uint32_t tag;       //user-defined tag for identifying the associated gameplay event

	uint8_t inherit;    //whether or not this control should inherit its code and mods from another control
	uint32_t parentIdx; //index into the group's controls array where the parent is located, NOT the parent's tag
	
	CTRLcode code;      //the raw input that triggers this control
	uint32_t actions;   //a bit mask of all of allowed actions that will trigger the control
	uint32_t mods;      //a bit mask of all the required mods that allow the control to be triggered
} CTRLcontrol;

//a group of controls
typedef struct CTRLgroup
{
	uint32_t numControls;
	uint32_t arraySize;
	CTRLcontrol* controls;
} CTRLgroup;

//a single raw input
typedef struct CTRLinput
{
	CTRLcode code;
	CTRLaction action;
	uint32_t mods;
	float dir; //for scrolling / joystick input
} CTRLinput;

//--------------------------------------------------------------------------------------------------------------------------------//
//redefine the below macros if you wish to provide a custom memory allocator

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
/* Initializes the GLFW backend implementation. NOTE: if you use the glfw backend, DO NOT call glfwSetKeyCallback(), glfwSetMouseButtonCallback(), 
 * or glfwSetScrollCallback(). CTRL requires these callbacks to function.
 * @param window the GLFW window that was created for this application
 */
void ctrl_init_impl_glfw(GLFWwindow* window);

/* Updaates the backend implementation, DO NOT CALL DIRECTLY 
 */
void ctrl_update_impl();

/* Sets the input callback, CTRL will call this function whenever a control has been triggered
 * @param input_callback the callback function, with parameters (uint32_t tag, CTRLcode code, CTRLaction action, uint32_t mods, float dir, void* userData)
 * @param userData the user-provided pointer to pass to the callback
 */
void ctrl_set_callback(void (*input_callback)(uint32_t, CTRLcode, CTRLaction, uint32_t, float, void*), void* userData);
/* Sets the currently active groups, only groups set by this function will have their controls triggrered
 * @param numGroups the number of groups to set
 * @param groups the array groups to set
 */
void ctrl_set_groups(uint32_t numGroups, CTRLgroup** groups);

/* Pushes a raw input into the queue of unhandled inputs. Do not call unless you wish to inject your own inputs
 * @param input the input to push
 */
void ctrl_push_input(CTRLinput input);
/* Returns whether a specified input is currently held down
 * @param code the coe to check
 * @returns 1 if the input is held, 0 otherwise
 */
uint8_t ctrl_code_held(CTRLcode code);
/* Processes the current queue of inputs, calling the callback function last set in ctrl_set_callback if any controls were triggered.
 * You should call this once per frame
 */
void ctrl_process_input();

/* Creates an empty group. Call ctrl_free_group to free its contents and avoid a memory leak
 * @returns an empty group
 */
CTRLgroup ctrl_create_group();
/* Frees a group
 * @param group the group to free
 */
void ctrl_free_group(CTRLgroup group);

/* Creates a control with the given parameters
 * @param tag the controls tag, used to associate it with a gameplay event
 * @param code the raw input code that triggers this control
 * @param actions the bit mask of actions that trigger this control
 * @param mods the bit mask of mods required to trigger this control
 * @returns a control with the given parameters
 */
CTRLcontrol ctrl_control(uint32_t tag, CTRLcode code, uint32_t actions, uint32_t mods);
/* Crates a control that inherits from a parent with the given parameters
 * @param parentGroup the group that the parent exists in
 * @param parentTag the parent's tag
 * @param tag the controls tag, used to associate it with a gameplay event 
 * @param actions the bit mask of actions that trigger this control
 * @returns a control with the given parameters
 */
CTRLcontrol ctrl_control_inherit(CTRLgroup parentGroup, uint32_t parentTag, uint32_t tag, uint32_t actions);

/* Adds a control to a group
 * @param group the group to add the control to
 * @param control the control to add
 */
void ctrl_add_control(CTRLgroup* group, CTRLcontrol control);
/* Gets a control given its tag
 * @param group the group the control belongs to
 * @param tag the control's tag
 * @returns the control
 */
CTRLcontrol ctrl_get_control(CTRLgroup* group, uint32_t tag);
/* Sets an existing control's parameters
 * @param group the group the control belongs to
 * @param tag the control's tag
 * @param newCode the control's new code, or CTRL_CODE_NONE to keep the existing code
 * @param newActions the control's new actions, or 0 to keep the existing actions
 * @param newMods the control's new mods, will only be set if the code was sets
 */
void ctrl_set_control(CTRLgroup* group, uint32_t tag, CTRLcode newCode, uint32_t newActions, uint32_t newMods);

/* Marks a control to be set to the next input. For example, if this funciton was called, and then the user left clicked, the control's
 * code would be set to CTRL_MOUSE_BUTTON_LEFT, and any other controls that would have otherwise been triggrered by this click will not be.
 * After a single input, normal behavior resumes
 * @param group the group the control belongs to
 * @param tag the control's tag
 * @param cancelCode the code that will cancel this set. For example, if set to CTRL_KEY_ESCAPE, the setting will be canceled if the escape key is pressed
 * @param control_set_callback the function that will be called once the control is set, with parameters (uint8_t wasSet, CTRLcode code, void* userData)
 * @param userData the user-defined pointer to pass to the callback function
 */
void ctrl_set_control_to_next_input(CTRLgroup* group, uint32_t tag, CTRLcode cancelCode, void (*control_set_callback)(uint8_t, CTRLcode, void*), void* userData);

/* Returns the name of a given input code as a string
 * @param code the input code to get the name of
 * @returns the name of the input code
 */
const char* ctrl_get_code_name(CTRLcode code);

#if __cplusplus
} //extern "C" {
#endif

#endif