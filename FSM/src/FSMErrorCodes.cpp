// $Log: FSMErrorCodes.cpp,v $
// Revision 1.1  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
#ifdef _MSC_VER
    #pragma warning( disable : 4786 ) // 'identifier' : identifier was truncated to 'number' characters in the debug information
//    #pragma warning( disable : 4503 ) // 'identifier' : decorated name length exceeded, name was truncated
//    #pragma warning( disable : 4250 ) // 'class1' : inherits 'class2::member' via dominance
    #pragma warning( disable : 4251 ) // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
#endif
#ifdef __INTEL_COMPILER
    #pragma warning( disable : 985  ) // == C4786
#endif

#include "FiniteStateMachine-config.h"
#include "FSMErrorCodes.h"

namespace FSM {
    iErrorCode FSM_OPEN_FILE_FAILED("Could not open file \"%s\".");
    iErrorCode FSM_LOAD_STARTED("Cannot load a running FSM.");
    iErrorCode FSM_LOAD_NOT_IMPLEMENTED("Not implemented.");
    iErrorCode FSM_TRANSITION_NOT_FOUND("Transition not found.");
    iErrorCode FSM_ACTION_NOT_FOUND("Action not found.");
    iErrorCode FSM_CONDITION_NOT_FOUND("Condition not found.");
    iErrorCode FSM_STATE_DEFINED_TWICE("State \"%s\" is not unique.");
    iErrorCode FSM_STATE_NOT_FOUND("State \"%s\" not found.");
    iErrorCode FSM_CANNOT_DELETE_CURRENT("Cannot delete the state \"%s\": the state is current");
    iErrorCode FSM_VARAIBLE_DEFINED_TWICE("Varaible \"%s\" is not unique.");
    iErrorCode FSM_VARIABLE_NOT_FOUND("Varaible \"%s\" not found.");
    iErrorCode FSM_INITIAL_AFTER_START("Initial state set on an already running FSM.");
    iErrorCode FSM_ALREADY_STARTED("Start failed: already running");
    iErrorCode FSM_NOT_STARTED("ProcessEvent of a not started FSM called.");
    iErrorCode FSM_CLEAR_STARTED("Cannot clear a runnig FSM.");
    iErrorCode FSM_TAG_NOT_FOUND("Tag <%s> not found.");
    iErrorCode FSM_MISSED_ATTRIBUTE("Attribute \"%s\" of tag <%s>missed.");
    iErrorCode FSM_UNKNOWN_TAG("Unknown tag <%s> inside of <%s>.");
    iErrorCode FSM_UNKNOWN_PARAM("Unknown parameter \"%s\"of class \"%s\"");
    iErrorCode FSM_UNKNOWN_CONDITION_VALUE("Unknown condition value \"%s\" for parameter \"%s\" of class \"%s\".");
    iErrorCode FSM_UNKNOWN_CONDITION("Unknown condition value \"%d\".");
    iErrorCode FSM_PARAM_NOT_SET("Parameter \"%s\" of \"%s\" not set.");
    iErrorCode FSM_CLASS_REGISTERED_TWICE("Class \"%s\" registered twice.");
    iErrorCode FSM_CANNOT_CREATE_CLASS("Cannot create class \"%s\"");
    iErrorCode FSM_PARAM_NOT_READ("Could not read parameter \"%s\" from \"%s\".");

}; // namespace FSM
