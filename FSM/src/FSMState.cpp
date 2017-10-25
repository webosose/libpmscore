// $Log: FSMState.cpp,v $
// Revision 1.2  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.1  2002/07/22 17:20:59  vofka
// SimpleTransition, PushTransition, PopTransition added, not implemented yet
//
// Revision 1.9  2002/06/20 12:22:10  bobka
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

#include "FSM.h"
#include "FiniteStateMachine-config.h"
#include "FSMErrorCodes.h"

namespace FSM {

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFSMState::CFSMState()
{
}

CFSMState::~CFSMState()
{
    for(Transition_List::iterator i = m_Transitions.begin() ; i != m_Transitions.end() ; ++i)
    {
        (*i)->DeleteThis();
    }
    for(Action_List::iterator j = m_EnterActions.begin() ; j != m_EnterActions.end() ; ++j)
    {
        (*j)->DeleteThis();
    }
    for(Action_List::iterator n = m_LeaveActions.begin() ; n != m_LeaveActions.end() ; ++n)
    {
        (*n)->DeleteThis();
    }
    m_Transitions.clear();
    m_EnterActions.clear();
    m_LeaveActions.clear();
}

////////////////////////////////////////////////////////////////////////
//        Transitions
////////////////////////////////////////////////////////////////////////
CFSMTransition* CFSMState::AddTransition(const char* onEvent, const char *className)
{
    CFSMTransition *Transition = NULL;

    Transition = (CFSMTransition *)GetFSM()->Create(className);
    if(Transition==NULL)
    {
        iRethrow();
        return NULL;
    }

    Transition->SetFromState(GetName());
    Transition->SetOnEvent(onEvent);

    m_Transitions.push_back(Transition);

    return Transition;
}

CFSMTransition* CFSMState::AddSimpleTransition(const char *toStateName, const char* onEvent)
{
    CFSMSimpleTransition *Transition = (CFSMSimpleTransition *)GetFSM()->Create("SimpleTransition");

    if(Transition==NULL)
    {
        iRethrow();
        return NULL;
    }

    Transition->SetFromState(GetName());
    Transition->SetToState(toStateName);
    Transition->SetOnEvent(onEvent);

    m_Transitions.push_back(Transition);

    return Transition;
}

/*
CFSMTransition* CFSMState::GetTransition(const char* event) const
{
    CFSMTransition* trans = NULL;
    Transition_List::const_iterator i = Transitions.begin();
    while(!(i==Transitions.end()))
    {
        if (strcmp((*i)->GetOnEvent(), event)==0)
        {
            trans = (*i);
            break;
        }
        ++i;
    }
    return trans;
}
*/

int CFSMState::GetTransitionNumber() const
{
    return m_Transitions.size();
}

CFSMTransition* CFSMState::GetTransition(int index) const
{
    CFSMTransition* res = NULL;
    Transition_List::const_iterator item = m_Transitions.begin();
    int n=0;
    for (n=0; n<index; ++n)
    {
        if(item==m_Transitions.end()) break;
        ++item;
    }
    if (item!=m_Transitions.end())
    {
        res = (*item);
    }
    if (!res)
    {
        iThrow(CRITICAL(FSM_TRANSITION_NOT_FOUND));
    }
    return res;
}

/*
CFSMTransition* CFSMState::GetTransition(const char *event, const char* toState) const
{
    CFSMTransition* res = NULL;
    CFSMState* to = NULL;

    if (toState)
    {
        to = GetFSM()->GetState(toState);
        if (!to)
        {
            iRethrow();
            return NULL;
        }
    }

    Transition_List::const_iterator item = m_Transitions.begin();
    while (item!=m_Transitions.end())
    {
        res = (*item);
        if (res && to)
        {
            if (res->GetToState()!=to) res=NULL;
        }
        if (res && event)
        {
            if (strcmp(res->GetOnEvent(),event)==0) res=NULL;
        }
        if (res) break;
        ++item;
    }
    if (!res)
    {
        iThrow(CRITICAL(FSM_TRANSITION_NOT_FOUND));
    }
    return res;
}
*/

bool CFSMState::DeleteTransition(CFSMTransition *trans)
{
    bool res = false;
    Transition_List::iterator i = m_Transitions.begin();
    while(!(i==m_Transitions.end()))
    {
        if ((*i)==trans)
        {
            (*i)->DeleteThis();
            m_Transitions.erase(i);
            res = true;
            break;
        }
        ++i;
    }
    if(!res)
    {
        iThrow(CRITICAL(FSM_TRANSITION_NOT_FOUND));
    }
    return res;
}

Transition_List::const_iterator CFSMState::GetTransitionsBegin() const
{
    return m_Transitions.begin();
}

Transition_List::const_iterator CFSMState::GetTransitionsEnd() const
{
    return m_Transitions.end();
}

////////////////////////////////////////////////////////////////////////
//        Actions
////////////////////////////////////////////////////////////////////////
CFSMAction* CFSMState::AddEnterAction(const char *className)
{
    CFSMAction *action = NULL;

    action = (CFSMAction *)GetFSM()->Create(className);
    if(action==NULL)
    {
        iRethrow();
        return NULL;
    }

    m_EnterActions.push_back(action);

    return action;
}

CFSMAction* CFSMState::AddLeaveAction(const char *className)
{
    CFSMAction *action = NULL;

    action = (CFSMAction *)GetFSM()->Create(className);
    if(action==NULL)
    {
        iRethrow();
        return NULL;
    }

    m_LeaveActions.push_back(action);

    return action;
}

bool CFSMState::DeleteAction(CFSMAction *action)
{
    bool res = false;
    Action_List::iterator i = m_EnterActions.begin();
    while(!(i==m_EnterActions.end()))
    {
        if ((*i)==action)
        {
            (*i)->DeleteThis();
            m_EnterActions.erase(i);
            res = true;
            break;
        }
        ++i;
    }

    if(!res)
    {
        i = m_LeaveActions.begin();
        while(!(i==m_LeaveActions.end()))
        {
            if ((*i)==action)
            {
                (*i)->DeleteThis();
                m_LeaveActions.erase(i);
                res = true;
                break;
            }
            ++i;
        }
    }

    if(!res)
    {
        iThrow(CRITICAL(FSM_ACTION_NOT_FOUND));
    }
    return res;
}

CFSMAction* CFSMState::GetEnterAction(int index) const
{
    CFSMAction* res = NULL;
    Action_List::const_iterator item = m_EnterActions.begin();
    int n=0;
    for (n=0; n<index; ++n)
    {
        if(item==m_EnterActions.end()) break;
        ++item;
    }
    if (item!=m_EnterActions.end())
    {
        res = (*item);
    }
    if (!res)
    {
        iThrow(CRITICAL(FSM_ACTION_NOT_FOUND));
    }
    return res;
}

CFSMAction* CFSMState::GetLeaveAction(int index) const
{
    CFSMAction* res = NULL;
    Action_List::const_iterator item = m_LeaveActions.begin();
    int n=0;
    for (n=0; n<index; ++n)
    {
        if(item==m_LeaveActions.end()) break;
        ++item;
    }
    if (item!=m_LeaveActions.end())
    {
        res = (*item);
    }
    if (!res)
    {
        iThrow(CRITICAL(FSM_ACTION_NOT_FOUND));
    }
    return res;
}

Action_List::const_iterator CFSMState::GetEnterActionsBegin() const
{
    return m_EnterActions.begin();
}

Action_List::const_iterator CFSMState::GetEnterActionsEnd() const
{
    return m_EnterActions.end();
}

Action_List::const_iterator CFSMState::GetLeaveActionsBegin() const
{
    return m_LeaveActions.begin();
}

Action_List::const_iterator CFSMState::GetLeaveActionsEnd() const
{
    return m_LeaveActions.end();
}

///////////////////////////////////////////////////////////
//        Get/Set
///////////////////////////////////////////////////////////
const char* CFSMState::GetName() const
{
    return m_Name.c_str();
}

void CFSMState::SetName(const char* name)
{
    m_Name = name;
}

////////////////////////////////////////////////////////////////////////
//        Operate State
////////////////////////////////////////////////////////////////////////
bool CFSMState::GetTransition(CFSMTransition** trans, const char *Event, void *a, void *b)
{
    bool res = true;
    bool transit = false;
    *trans = NULL;
    Transition_List::const_iterator i = m_Transitions.begin();
    while( !(i==m_Transitions.end()) )
    {
        res = (*i)->Check(transit, Event, a, b);
        if (!res)
        {
            iRethrow();
            break;;
        }
        if (transit)
        {
            *trans=(*i);
            break;
        }
        ++i;
    }
    return res;
}

bool CFSMState::Activate()
{
    bool res = true;
    Action_List::const_iterator j = m_EnterActions.begin();
    while(!(j==m_EnterActions.end()))
    {
        res = (*j)->Execute("ACTIVATE", (void*)GetName(), (void*)GetNameOfClass());
        if (!res)
        {
            iRethrow();
            break;
        }
        ++j;
    }

    if (res)
    {
        Transition_List::const_iterator i = m_Transitions.begin();
        while(!(i==m_Transitions.end()))
        {
            res = (*i)->Activate();
            if (!res)
            {
                iRethrow();
                break;
            }
            ++i;
        }
    }

    return res;
}

bool CFSMState::Deactivate()
{
    bool res = true;

    Transition_List::const_iterator i = m_Transitions.begin();
    while(!(i==m_Transitions.end()))
    {
        res = (*i)->Deactivate();
        if (!res)
        {
            iRethrow();
            break;
        }
        ++i;
    }

    if (res)
    {
        Action_List::const_iterator j = m_LeaveActions.begin();
        while(!(j==m_LeaveActions.end()))
        {
            res = (*j)->Execute("DEACTIVATE", (void*)GetName(), (void*)GetNameOfClass());
            if (!res)
            {
                iRethrow();
                break;
            }
            ++j;
        }
    }

    return res;
}

///////////////////////////////////////////////////////////
//     Construction/Destruction
///////////////////////////////////////////////////////////
CFSMObject* CFSMState::Create()
{
    return new CFSMState;
}

void CFSMState::DeleteThis()
{
    delete this;
}

/////////////////////////////////////////////////
}; // namespace FSM

