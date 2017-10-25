// $Log: FSMTransition.cpp,v $
// Revision 1.2  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.1  2002/07/22 17:21:00  vofka
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

#include <cstring>

#include "FSM.h"
#include "FiniteStateMachine-config.h"
#include "FSMErrorCodes.h"

namespace FSM {

static const char* FSM_PARAM_TOSTATE = "ToState";

// Initialize the Name of the Class
const std::string ClassName = "CFSMTransition";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFSMTransition::CFSMTransition()
:m_Event("ANY")
,m_FromState("")
{
}

CFSMTransition::~CFSMTransition()
{
    for(Condition_List::iterator i = m_Conditions.begin() ; i != m_Conditions.end() ; ++i)
    {
        (*i)->DeleteThis();
    }

    for(Action_List::iterator j = m_Actions.begin() ; j != m_Actions.end() ; ++j)
    {
        (*j)->DeleteThis();
    }

    m_Actions.clear();
    m_Conditions.clear();
    m_Event="ANY";
    m_FromState = "";
}

////////////////////////////////////////////////////////////////////////
//        Actions
////////////////////////////////////////////////////////////////////////
CFSMAction* CFSMTransition::AddAction(const char *className)
{
    CFSMAction *action;

    action = (CFSMAction *)GetFSM()->Create(className);
    if(action==NULL)
    {
        iRethrow();
        return NULL;
    }

    m_Actions.push_back(action);
    return action;
}

CFSMAction* CFSMTransition::GetAction(int index) const
{
    CFSMAction* res = NULL;
    Action_List::const_iterator item = m_Actions.begin();
    int n=0;
    for (n=0; n<index; ++n)
    {
        if(item==m_Actions.end()) break;
        ++item;
    }
    if (item!=m_Actions.end())
    {
        res = (*item);
    }
    if (!res)
    {
        iThrow(CRITICAL(FSM_ACTION_NOT_FOUND));
    }
    return res;
}

bool CFSMTransition::DeleteAction(CFSMAction *action)
{
    bool res = false;
    Action_List::iterator i = m_Actions.begin();
    while(!(i==m_Actions.end()))
    {
        if ((*i)==action)
        {
            (*i)->DeleteThis();
            m_Actions.erase(i);
            res = true;
            break;
        }
        ++i;
    }
    if(!res)
    {
        iThrow(CRITICAL(FSM_ACTION_NOT_FOUND));
    }
    return res;
}

Action_List::const_iterator CFSMTransition::GetActionsBegin() const
{
    return m_Actions.begin();
}

Action_List::const_iterator CFSMTransition::GetActionsEnd() const
{
    return m_Actions.end();
}

////////////////////////////////////////////////////////////////////////
//        Conditions
////////////////////////////////////////////////////////////////////////
CFSMCondition* CFSMTransition::AddCondition(const char *className)
{
    CFSMCondition *condition;

    condition = (CFSMCondition *)GetFSM()->Create(className);
    if(condition==NULL)
    {
        iRethrow();
        return NULL;
    }

    m_Conditions.push_back(condition);
    return condition;
}

CFSMCondition* CFSMTransition::GetCondition(int index) const
{
    CFSMCondition* res = NULL;
    Condition_List::const_iterator item = m_Conditions.begin();
    int n=0;
    for (n=0; n<index; ++n)
    {
        if(item==m_Conditions.end()) break;
        ++item;
    }
    if (item!=m_Conditions.end())
    {
        res = (*item);
    }
    if (!res)
    {
        iThrow(CRITICAL(FSM_ACTION_NOT_FOUND));
    }
    return res;
}

bool CFSMTransition::DeleteCondition(CFSMCondition *condition)
{
    bool res = false;
    Condition_List::iterator i = m_Conditions.begin();
    while(!(i==m_Conditions.end()))
    {
        if ((*i)==condition)
        {
            (*i)->DeleteThis();
            m_Conditions.erase(i);
            res = true;
            break;
        }
        ++i;
    }
    if(!res)
    {
        iThrow(CRITICAL(FSM_CONDITION_NOT_FOUND));
    }
    return res;
}

Condition_List::const_iterator CFSMTransition::GetConditionsBegin() const
{
    return m_Conditions.begin();
}

Condition_List::const_iterator CFSMTransition::GetConditionsEnd() const
{
    return m_Conditions.end();
}

////////////////////////////////////////////////////////////////////////
//        Get / Set
////////////////////////////////////////////////////////////////////////
void CFSMTransition::SetFromState(const char *stateName)
{
    m_FromState = stateName;
}

void CFSMTransition::SetOnEvent(const char *event)
{
    m_Event = event;
}

CFSMState* CFSMTransition::GetFromState() const
{
    CFSMState *res = NULL;
    res = GetFSM()->GetState(m_FromState.c_str());
    if (!res)
    {
        iRethrow();
    }
    return res;
}

const char* CFSMTransition::GetOnEvent() const
{
    if (m_Event.empty()) return "";
    else return m_Event.c_str();
}

////////////////////////////////////////////////////////////////////////
//        Operate Transition
////////////////////////////////////////////////////////////////////////
bool CFSMTransition::Check(bool &retval, const char *event, void *a, void *b)
{
    bool res=true;
    retval=true;

    if (m_Event.empty())
    {
        iThrow(WARNING(FSM_PARAM_NOT_SET), "OnEvent", GetNameOfClass());
        return false;
    }
    if (m_Event!="ANY" && m_Event!=event)
    {
        retval = false;
        return true;
    }

    for(Condition_List::iterator i = m_Conditions.begin() ; i != m_Conditions.end() ; ++i)
    {
        retval = (*i)->CheckCondition(event, a, b);
        if(retval==false)
        {
            break;
        }
    }
    return res;
}

bool CFSMTransition::Execute(const char* event, void* a, void* b)
{
    bool res = true;

    // execute actions
    Action_List::const_iterator j = m_Actions.begin();
    while(!(j==m_Actions.end()))
    {
        res = (*j)->Execute(event, a, b);
        if (!res)
        {
            iRethrow();
            break;
        }
        ++j;
    }

    return res;
}

////////////////////////////////////////////////////////////////////////
//        Overloadables
////////////////////////////////////////////////////////////////////////
bool CFSMTransition::Activate()
{
    bool res = true;
    for(Condition_List::iterator i = m_Conditions.begin() ; i != m_Conditions.end() ; ++i)
    {
        res = (*i)->Activate();
        if(!res)
        {
            iRethrow();
            break;
        }
    }
    return res;
}

bool CFSMTransition::Deactivate()
{
    bool res = true;
    for(Condition_List::iterator i = m_Conditions.begin() ; i != m_Conditions.end() ; ++i)
    {
        res = (*i)->Deactivate();
        if(!res)
        {
            iRethrow();
            break;
        }
    }
    return res;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFSMSimpleTransition::CFSMSimpleTransition()
:m_ToState("")
{
}

CFSMSimpleTransition::~CFSMSimpleTransition()
{
    m_ToState = "";
}

////////////////////////////////////////////////////////////////////////
//        Get / Set
////////////////////////////////////////////////////////////////////////

void CFSMSimpleTransition::SetToState(const char *stateName)
{
    m_ToState = stateName;
}

CFSMState*
CFSMSimpleTransition::GetToState() const
{
    CFSMState *res = NULL;
    if (m_ToState.empty())
    {
        res = GetFSM()->GetState(GetFromState()->GetName());
    } else
    {
        res = GetFSM()->GetState(m_ToState.c_str());
    }
    if (!res)
    {
        iRethrow();
    }
    return res;
}


////////////////////////////////////////////////////////////////////////
//        Overloaded
////////////////////////////////////////////////////////////////////////
bool CFSMSimpleTransition::SaveParameters(CFSMSaver* out) const
{
    //if (!out->SaveParameter(FSM_PARAM_TOSTATE, GetToState()->GetName())) {iRethrow(); return false;}
    return true;
}

bool CFSMSimpleTransition::SetParameter(const char* paramName, const char* paramValue)
{
    bool res = false;
    if (strcmp(paramName, FSM_PARAM_TOSTATE)==0)
    {
        SetToState(paramValue);
        res = true;
    } else

    {
        iThrow(CRITICAL(FSM_UNKNOWN_PARAM), paramName, GetNameOfClass());
    }
    return res;
}

////////////////////////////////////////////////////////////////////////
//        Operate Transition
////////////////////////////////////////////////////////////////////////

bool CFSMSimpleTransition::Execute(const char* event, void* a, void* b)
{
    bool res = CFSMTransition::Execute(event, a, b);
    if (res)
    {
        // switch to the new state
        CFSMState *newState = GetToState();
        if (newState)
        {
            if( ! GetFSM()->SwitchToState( newState ) )
            {
                iRethrow();
            }
        } else
        {
            iRethrow();
        }
    } else
    {
        iRethrow();
    }

    return res;
}

////////////////////////////////////////////////////////////////////////
//        Construction/Destruction
////////////////////////////////////////////////////////////////////////
CFSMObject * CFSMSimpleTransition::Create()
{
    return new CFSMSimpleTransition;
}

void CFSMSimpleTransition::DeleteThis()
{
    delete this;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFSMPushTransition::CFSMPushTransition()
{
}

CFSMPushTransition::~CFSMPushTransition()
{
}


////////////////////////////////////////////////////////////////////////
//        Overloaded
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//        Operate Transition
////////////////////////////////////////////////////////////////////////

bool CFSMPushTransition::Execute(const char* event, void* a, void* b)
{
    bool res = CFSMTransition::Execute(event, a, b);
    if (res)
    {
        // switch to the new state
        CFSMState *newState = GetToState();
        if (newState)
        {
            if( ! GetFSM()->PushState( newState ) )
            {
                iRethrow();
            }
        } else
        {
            iRethrow();
        }
    } else
    {
        iRethrow();
    }

    return res;
}

////////////////////////////////////////////////////////////////////////
//        Construction/Destruction
////////////////////////////////////////////////////////////////////////
CFSMObject * CFSMPushTransition::Create()
{
    return new CFSMPushTransition;
}

void CFSMPushTransition::DeleteThis()
{
    delete this;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFSMPopTransition::CFSMPopTransition()
{
}

CFSMPopTransition::~CFSMPopTransition()
{
}


////////////////////////////////////////////////////////////////////////
//        Overloaded
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//        Operate Transition
////////////////////////////////////////////////////////////////////////

bool CFSMPopTransition::Execute(const char* event, void* a, void* b)
{
    bool res = CFSMTransition::Execute(event, a, b);
    if (res)
    {
        // switch to the new state
        if( ! GetFSM()->PopState() )
        {
            iRethrow();
        }
    } else
    {
        iRethrow();
    }

    return res;
}

////////////////////////////////////////////////////////////////////////
//        Construction/Destruction
////////////////////////////////////////////////////////////////////////
CFSMObject * CFSMPopTransition::Create()
{
    return new CFSMPopTransition;
}

void CFSMPopTransition::DeleteThis()
{
    delete this;
}
/////////////////////////////////////////////////

}; // namespace FSM

