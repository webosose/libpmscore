// $Log: FSMCondition.cpp,v $
// Revision 1.2  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.1  2002/07/22 17:20:59  vofka
// SimpleTransition, PushTransition, PopTransition added, not implemented yet
//
// Revision 1.8  2002/06/20 12:22:10  bobka
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


static const char* FSM_CONDITION_EQUAL      = "==";
static const char* FSM_CONDITION_NEQUAL     = "!=";
static const char* FSM_CONDITION_GREATER    = ">";
static const char* FSM_CONDITION_LESS       = "<";
static const char* FSM_CONDITION_GREATER_EQ = ">=";
static const char* FSM_CONDITION_LESS_EQ    = "<=";

static const char* FSM_PARAM_VARIABLE = "Variable";
static const char* FSM_PARAM_CONDITION = "Condition";
static const char* FSM_PARAM_VALUE = "Value";
static const char* FSM_PARAM_EXPRESSION    = "Expression";


//////////////////////////////////////////////////////////////////////
// Overloadables
//////////////////////////////////////////////////////////////////////
bool CFSMCondition::Activate()
{
    return true;
}

bool CFSMCondition::Deactivate()
{
    return true;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFSMCondition::CFSMCondition()
{
}

CFSMCondition::~CFSMCondition()
{
}

/////////////////////////////////////////////////////////////////////
//        CFSMConditionTestVariable
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//        Get/Set
/////////////////////////////////////////////////////////////////////
void CFSMConditionTestVariable::SetCondition(const char *varName, OP testCondition, int testValue)
{
    variableName = varName;
    condition=testCondition;
    value=testValue;
}

const char* CFSMConditionTestVariable::GetVariable() const
{
    return variableName.c_str();
}

CFSMConditionTestVariable::OP CFSMConditionTestVariable::GetCondition() const
{
    return condition;
}

int CFSMConditionTestVariable::GetValue() const
{
    return value;
}

bool CFSMConditionTestVariable::SetExpression(const char* expression)
{
    char var[1024];
    char cond[20];
    char value[50];
    const char *p = expression;
    bool res = false;

    char *dest = var;
    while (dest)
    {
        switch(*p)
        {
        case '=':
        case '>':
        case '<':
        case '!':
        case ' ':
        case '\t':
        case '\n':
        case '\0':
            *dest='\0';
            dest = NULL;
            break;
        default:
            *dest = *p;
            ++dest;
            ++p;
            break;
        }
    }

    dest = cond;
    while (dest)
    {
        switch(*p)
        {
        case '=':
        case '>':
        case '<':
        case '!':
            *dest = *p;
            ++dest;
            ++p;
            break;
        case ' ':
        case '\t':
        case '\n':
        case '\0':
            ++p;
            break;
        default:
            *dest='\0';
            dest = NULL;
            break;
        }
    }

    dest = value;
    while (dest)
    {
        switch(*p)
        {
        case '\0':
        case  ' ':
        case '\t':
        case '\n':
            *dest='\0';
            dest = NULL;
            break;
        default:
            *dest = *p;
            ++dest;
            ++p;
            break;
        }
    }

    if (SetParameter(FSM_PARAM_VARIABLE, var))
    {
        if (SetParameter(FSM_PARAM_CONDITION, cond))
        {
            if (SetParameter(FSM_PARAM_VALUE, value))
            {
                res = true;
            }
        }
    }
    return res;
}

////////////////////////////////////////////////////////////////////////
//        Overloaded
////////////////////////////////////////////////////////////////////////

bool CFSMConditionTestVariable::SetParameter(const char *paramName, const char* paramValue)
{
    bool res = false;
    if(strncmp(paramName, FSM_PARAM_VALUE, strlen(FSM_PARAM_VALUE))==0)
    {
        value = atoi(paramValue);
        res = true;
    } else

    if(strncmp(paramName, FSM_PARAM_VARIABLE, strlen(FSM_PARAM_VARIABLE))==0)
    {
        variableName = paramValue;
        res = true;
    } else

    if(strncmp(paramName, FSM_PARAM_CONDITION, strlen(FSM_PARAM_CONDITION))==0)
    {
        if(strncmp(paramValue, FSM_CONDITION_EQUAL, strlen(FSM_CONDITION_EQUAL))==0)
        {
            condition = EQUAL;
            res = true;
        } else

        if(strncmp(paramValue, FSM_CONDITION_NEQUAL, strlen(FSM_CONDITION_NEQUAL))==0)
        {
            condition = NEQUAL;
            res = true;
        } else

        if(strncmp(paramValue, FSM_CONDITION_GREATER_EQ, strlen(FSM_CONDITION_GREATER_EQ))==0)
        {
            condition = GREATER_EQ;
            res = true;
        } else

        if(strncmp(paramValue, FSM_CONDITION_LESS_EQ, strlen(FSM_CONDITION_LESS_EQ))==0)
        {
            condition = LESS_EQ;
            res = true;
        } else

        if(strncmp(paramValue, FSM_CONDITION_GREATER, strlen(FSM_CONDITION_GREATER))==0)
        {
            condition = GREATER;
            res = true;
        } else

        if(strncmp(paramValue, FSM_CONDITION_LESS, strlen(FSM_CONDITION_LESS))==0)
        {
            condition = LESS;
            res = true;
        } else

        {
            iThrow(CRITICAL(FSM_UNKNOWN_CONDITION_VALUE), paramValue, paramName, GetNameOfClass());
        }
    } else

    if(strcmp(paramName, FSM_PARAM_EXPRESSION)==0)
    {
        res = SetExpression(paramValue);
    } else


    {
        iThrow(CRITICAL(FSM_UNKNOWN_PARAM), paramName, GetNameOfClass());
    }

    return res;
}

bool CFSMConditionTestVariable::CheckCondition(const char* event, void *a, void *b)
{
    bool res = false;
    int    varValue=0;

    if(variableName.empty())
    {
        iThrow(WARNING(FSM_PARAM_NOT_SET), GetNameOfClass());
        return false;
    }

    if( !GetFSM()->GetVariable(variableName.c_str(), varValue) )
    {
        iHandleLastError();
        return false;
    }

    switch(condition)
    {
        case EQUAL:
            res = (varValue==value);
            break;
        case NEQUAL:
            res = (varValue!=value);
            break;
        case GREATER:
            res = (varValue>value);
            break;
        case LESS:
            res = (varValue<value);
            break;
        case GREATER_EQ:
            res = (varValue>=value);
            break;
        case LESS_EQ:
            res = (varValue<=value);
            break;
    }
    return res;
}

////////////////////////////////////////////////////////////////////////
//        Construction/Destruction
////////////////////////////////////////////////////////////////////////
CFSMObject* CFSMConditionTestVariable::Create()
{
    return new CFSMConditionTestVariable;
}

void CFSMConditionTestVariable::DeleteThis()
{
    delete this;
}

////////////////////////////////////////////////////////////////////////
//        Save
////////////////////////////////////////////////////////////////////////
bool CFSMConditionTestVariable::SaveParameters(CFSMSaver* out) const
{
    const char* cond=NULL;
    //if (!out->SaveParameter(FSM_PARAM_VARIABLE, variableName.c_str())) {iRethrow(); return false;}
    switch(condition)
    {
        case EQUAL:
            cond=FSM_CONDITION_EQUAL;
            break;
        case NEQUAL:
            cond=FSM_CONDITION_NEQUAL;
            break;
        case GREATER:
            cond=FSM_CONDITION_GREATER;
            break;
        case LESS:
            cond=FSM_CONDITION_LESS;
            break;
        case GREATER_EQ:
            cond=FSM_CONDITION_GREATER_EQ;
            break;
        case LESS_EQ:
            cond=FSM_CONDITION_LESS_EQ;
            break;
        default:
            iThrow(CRITICAL(FSM_UNKNOWN_CONDITION), condition);
            return false;
            break;
    }
    //if (!out->SaveParameter(FSM_PARAM_CONDITION, cond)) {iRethrow(); return false;}
    char buf[20];
    sprintf(buf, "%d", value);
    //if (!out->SaveParameter(FSM_PARAM_VALUE, buf)) {iRethrow(); return false;}
    return true;
}

////////////////////////////////////////////////////////////////////////
//        Constructor/Destructor
////////////////////////////////////////////////////////////////////////
CFSMConditionTestVariable::CFSMConditionTestVariable()
{
}

CFSMConditionTestVariable::~CFSMConditionTestVariable()
{
}

/////////////////////////////////////////////////
}; // FSM

