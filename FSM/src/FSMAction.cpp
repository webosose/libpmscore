// $Log: FSMAction.cpp,v $
// Revision 1.2  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.1  2002/07/22 17:20:59  vofka
// SimpleTransition, PushTransition, PopTransition added, not implemented yet
//
// Revision 1.10  2002/06/20 12:22:10  bobka
// LGPL license
//
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

#include <string.h>

#include "FSM.h"
#include "FiniteStateMachine-config.h"
#include "FSMErrorCodes.h"

namespace FSM {

static const char* FSM_PARAM_VARIABLE = "Variable";
static const char* FSM_PARAM_VALUE = "Value";
static const char* FSM_PARAM_INCREMENT = "Increment";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFSMAction::CFSMAction()
{
}

CFSMAction::~CFSMAction()
{
}

/////////////////////////////////////////////////
//    CFSMActionSetVariable
/////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//        Get/Set
////////////////////////////////////////////////////////////////////////
void CFSMActionSetVariable::SetVariable(const char* varName)
{
    variableName = varName;
}

void CFSMActionSetVariable::SetValue(int val)
{
    value = val;
}

const char* CFSMActionSetVariable::GetVariable() const
{
    return variableName.c_str();
}

int CFSMActionSetVariable::GetValue() const
{
    return value;
}

////////////////////////////////////////////////////////////////////////
//        Overloaded
////////////////////////////////////////////////////////////////////////
bool CFSMActionSetVariable::Execute(const char* event, void *a, void *b)
{
    bool res = GetFSM()->SetVariable(variableName.c_str(), value);
    if (!res)
    {
        iRethrow();
    }
    return res;
}

bool CFSMActionSetVariable::SetParameter(const char* paramName, const char* paramValue)
{
    bool res = false;
    if (strcmp(paramName, FSM_PARAM_VARIABLE)==0)
    {
        variableName = paramValue;
        res = true;
    } else

    if (strcmp(paramName, FSM_PARAM_VALUE)==0)
    {
        value = atoi(paramValue);
        res = true;
    } else

    {
        iThrow(CRITICAL(FSM_UNKNOWN_PARAM), paramName, GetNameOfClass());
    }
    return res;
}

////////////////////////////////////////////////////////////////////////
//        Construction/Destruction
////////////////////////////////////////////////////////////////////////
CFSMObject* CFSMActionSetVariable::Create()
{
    CFSMObject* res = new CFSMActionSetVariable();
    return res;
}

void CFSMActionSetVariable::DeleteThis()
{
    delete this;
}

////////////////////////////////////////////////////////////////////////
//        Overloaded
////////////////////////////////////////////////////////////////////////
bool CFSMActionSetVariable::SaveParameters(CFSMSaver *out) const
{
    char buf[40];
    sprintf(buf, "%d", value);
    //if (!out->SaveParameter(FSM_PARAM_VARIABLE, variableName.c_str())) {iRethrow(); return false;}
    //if (!out->SaveParameter(FSM_PARAM_VALUE, buf)) {iRethrow(); return false;}
    return true;
}

////////////////////////////////////////////////////////////////////////
//    private
////////////////////////////////////////////////////////////////////////
CFSMActionSetVariable::CFSMActionSetVariable()
:value(0)
{
}

CFSMActionSetVariable::~CFSMActionSetVariable()
{
}

/////////////////////////////////////////////////
//    CFSMActionIncrVariable
/////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//        Get/Set
////////////////////////////////////////////////////////////////////////
void CFSMActionIncrVariable::SetVariable(const char* varName)
{
    variableName = varName;
}

void CFSMActionIncrVariable::SetIncrement(int val)
{
    increment = val;
}

const char* CFSMActionIncrVariable::GetVariable() const
{
    return variableName.c_str();
}

int CFSMActionIncrVariable::GetIncrement() const
{
    return increment;
}

////////////////////////////////////////////////////////////////////////
//        Overloaded
////////////////////////////////////////////////////////////////////////
bool CFSMActionIncrVariable::Execute(const char* event, void *a, void *b)
{
    bool res=false;
    int value;
    res = GetFSM()->GetVariable(variableName.c_str(), value);
    if (res)
    {
        res = GetFSM()->SetVariable(variableName.c_str(), value+increment);
    }

    if (!res)
    {
        iRethrow();
    }
    return res;
}

bool CFSMActionIncrVariable::SetParameter(const char* paramName, const char* paramValue)
{
    bool res = false;
    if (strcmp(paramName, FSM_PARAM_VARIABLE)==0)
    {
        variableName = paramValue;
        res = true;
    } else

    if (strcmp(paramName, FSM_PARAM_INCREMENT)==0)
    {
        increment = atoi(paramValue);
        res = true;
    } else

    {
        iThrow(CRITICAL(FSM_UNKNOWN_PARAM), paramName, GetNameOfClass());
    }
    return res;
}

////////////////////////////////////////////////////////////////////////
//        Construction/Destruction
////////////////////////////////////////////////////////////////////////
CFSMObject* CFSMActionIncrVariable::Create()
{
    CFSMObject* res = new CFSMActionIncrVariable();
    return res;
}

void CFSMActionIncrVariable::DeleteThis()
{
    delete this;
}

////////////////////////////////////////////////////////////////////////
//        Overloaded
////////////////////////////////////////////////////////////////////////
bool CFSMActionIncrVariable::SaveParameters(CFSMSaver* out) const
{
    char buf[40];
    sprintf(buf, "%d", increment);
    //if (!out->SaveParameter(FSM_PARAM_VARIABLE, variableName.c_str())) {iRethrow(); return false;}
    //if (!out->SaveParameter(FSM_PARAM_INCREMENT, buf)) {iRethrow(); return false;}
    return true;
}

////////////////////////////////////////////////////////////////////////
//    private
////////////////////////////////////////////////////////////////////////
CFSMActionIncrVariable::CFSMActionIncrVariable()
:increment(1)
{
}

CFSMActionIncrVariable::~CFSMActionIncrVariable()
{
}

}; // namespace FSM

