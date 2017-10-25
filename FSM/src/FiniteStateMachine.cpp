// $Log: FiniteStateMachine.cpp,v $
// Revision 1.3  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.2  2002/08/13 11:06:32  vofka
// automake support added
//
// Revision 1.1  2002/07/22 17:21:00  vofka
// SimpleTransition, PushTransition, PopTransition added, not implemented yet
//
// Revision 1.15  2002/06/20 12:22:10  bobka
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
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
//    #pragma set woff 1174
#endif

#include "FSM.h"
#include "FiniteStateMachine-config.h"
#include "FSMErrorCodes.h"
#include "FiniteStateMachine.h"

using namespace std;

#ifdef HAVE_LIBIWRAPPER
#    include <ilibrary.h>
#else
#    ifdef WIN32
#        include <windows.h>
#    else
#        include <dlfcn.h>
#    endif
#endif

namespace FSM {
    #ifdef HAVE_LIBIWRAPPER
        using iLIB::iLibrary;
    #endif

////////////////////////////////////////////////////////////////////////
//        Constructor
////////////////////////////////////////////////////////////////////////
CFiniteStateMachine::CFiniteStateMachine()
:m_InitialState("")
,m_CurrentState(NULL)
,m_Running(false)
{
    RegisterBaseTypes();
}

////////////////////////////////////////////////////////////////////////
//        Destructor
////////////////////////////////////////////////////////////////////////
CFiniteStateMachine::~CFiniteStateMachine()
{
    if (m_Running) Stop();
    Clear();
}

////////////////////////////////////////////////////////////////////////
//        States
////////////////////////////////////////////////////////////////////////
CFSMState* CFiniteStateMachine::AddState(const char *stateName, const char *className)
{
    CFSMState *State = NULL;

    State = (CFSMState *)Create(className);
    if(State)
    {
        State->SetName(stateName);

        const pair<State_Map::iterator,bool>result = m_States.insert(State_Map::value_type(stateName, State));
        if(result.second == false)
        {
            iThrow(WARNING(FSM_STATE_DEFINED_TWICE), stateName);
        }
    }
    return State;
}

CFSMState*    CFiniteStateMachine::GetState(const char *stateName) const
{
    CFSMState* res = NULL;
    if (stateName)
    {
        State_Map::const_iterator i = m_States.find(stateName);
        if(i != m_States.end())
        {
            res = (*i).second;
        } else {
            iThrow(CRITICAL(FSM_STATE_NOT_FOUND), stateName);
        }
    } else
    {
            iThrow(CRITICAL(FSM_STATE_NOT_FOUND), "");
    }
    return res;
}

bool CFiniteStateMachine::DeleteState(CFSMState *state)
{
    bool res = false;

    State_Map::iterator i = m_States.begin();
    while( !( i == m_States.end() ) )
    {
        if ( (*i).second == state )
        {
            if(!(m_Running && m_CurrentState==state))
            {
                state->DeleteThis();
                m_States.erase(i);
                res = true;
            } else {
                iThrow(CRITICAL(FSM_CANNOT_DELETE_CURRENT), state->GetName());
            }
            break;
        }
        ++i;
    }
    if (res==false)
    {
        iThrow(CRITICAL(FSM_STATE_NOT_FOUND), state->GetName());
    }
    return res;
}

bool CFiniteStateMachine::DeleteState(const char *stateName)
{
    bool res = false;
    CFSMState* state = GetState(stateName);
    if (state)
    {
        res = DeleteState(state);
    } else
    {
        iRethrow();
    }
    return res;
}

State_Map::const_iterator CFiniteStateMachine::GetStatesBegin() const
{
    return m_States.begin();
}

State_Map::const_iterator CFiniteStateMachine::GetStatesEnd() const
{
    return m_States.end();
}

////////////////////////////////////////////////////////////////////////
//        Transitions
////////////////////////////////////////////////////////////////////////
CFSMTransition* CFiniteStateMachine::AddTransition(const char *stateName, const char* onEvent, const char *className)
{
    CFSMTransition* res = NULL;
    CFSMState* state = GetState(stateName);
    if (state)
    {
        res = state->AddTransition(onEvent, className);
        if (!res)
        {
            iRethrow();
        }
    } else {
        iRethrow();
    }
    return res;
}

CFSMTransition* CFiniteStateMachine::AddSimpleTransition(const char *stateName, const char *toStateName, const char* onEvent)
{
    CFSMTransition* res = NULL;
    CFSMState* state = GetState(stateName);
    if (state)
    {
        res = state->AddSimpleTransition(toStateName, onEvent);
        if (!res)
        {
            iRethrow();
        }
    } else {
        iRethrow();
    }
    return res;
}

/*
bool CFiniteStateMachine::DeleteTransition(const char* fromState, const char* event, const char* toState)
{
    bool res = false;
    CFSMState* state = NULL;
    CFSMTransition* trans = NULL;
    if (fromState)
    {
        state = GetState(fromState);
        if (state)
        {
            trans = state->GetTransition(event, toState);
            if (!trans)    iRethrow();
        } else // state not found
        {
            iRethrow();
        }
    } else
    {
        // alle states durchgehen
        State_Map::const_iterator item = m_States.begin();
        while(!(item==m_States.end()))
        {
            state = (*item).second;
            trans = state->GetTransition(event, toState);
            if (trans) break;
        }
        if(item==m_States.end())
        {
            iThrow(CRITICAL(FSM_TRANSITION_NOT_FOUND));
        }
    }

    if (trans)
    {
        res = state->DeleteTransition(trans);
        if (!res)
        {
            iRethrow();
        }
    } else {
        iRethrow();
    }
    return res;
}
*/

bool CFiniteStateMachine::DeleteTransition(const char* fromState, int index)
{
    bool res = false;
    CFSMState* state = NULL;
    CFSMTransition* trans = NULL;
    if (fromState)
    {
        state = GetState(fromState);
        if (state)
        {
            trans = state->GetTransition(index);
            if (!trans)    iRethrow();
        } else // state not found
        {
            iRethrow();
        }
    } else
    {
        iThrow(CRITICAL(FSM_STATE_NOT_FOUND), "");
    }

    if (trans)
    {
        res = state->DeleteTransition(trans);
        if (!res)
        {
            iRethrow();
        }
    } else {
        iRethrow();
    }
    return res;
}

////////////////////////////////////////////////////////////////////////
//        Conditions
////////////////////////////////////////////////////////////////////////
CFSMCondition* CFiniteStateMachine::AddCondition(const char *stateName, int index, const char *className)
{
    CFSMState *state = GetState(stateName);
    if (!state)
    {
        iRethrow();
        return NULL;
    }

    CFSMTransition *transition = state->GetTransition(index);
    if (!transition)
    {
        iRethrow();
        return NULL;
    }

    CFSMCondition *condition = transition->AddCondition(className);
    if (!condition)
    {
        iRethrow();
    }

    return condition;
}

////////////////////////////////////////////////////////////////////////
//        Actions
////////////////////////////////////////////////////////////////////////
CFSMAction* CFiniteStateMachine::AddEnterAction(const char *stateName, const char *className)
{
    CFSMAction* res = NULL;
    CFSMState* state = GetState(stateName);
    if (state)
    {
        res = state->AddEnterAction(className);
        if (!res)
        {
            iRethrow();
        }
    } else {
        iRethrow();
    }
    return res;
}

CFSMAction* CFiniteStateMachine::AddLeaveAction(const char *stateName, const char *className)
{
    CFSMAction* res = NULL;
    CFSMState* state = GetState(stateName);
    if (state)
    {
        res = state->AddLeaveAction(className);
        if (!res)
        {
            iRethrow();
        }
    } else {
        iRethrow();
    }
    return res;
}

CFSMAction* CFiniteStateMachine::AddAction(const char *stateName, int index, const char *className)
{
    CFSMState *state = GetState(stateName);
    if (!state)
    {
        iRethrow();
        return NULL;
    }

    CFSMTransition *transition = state->GetTransition(index);
    if (!transition)
    {
        iRethrow();
        return NULL;
    }

    CFSMAction *action = transition->AddAction(className);
    if (!action)
    {
        iRethrow();
    }

    return action;
}

////////////////////////////////////////////////////////////////////////
//        Variables
////////////////////////////////////////////////////////////////////////
bool CFiniteStateMachine::AddVariable(const char *varName, const int initValue)
{
    VM_VT v(varName, initValue);
    const pair<Var_Map::iterator,bool>result = m_Variables.insert(v);
    if(result.second == false)
    {
        iThrow(WARNING(FSM_VARAIBLE_DEFINED_TWICE), varName);
    }
    return result.second;
}

bool CFiniteStateMachine::DeleteVariable(const char* varName)
{
    bool res = false;

    Var_Map::iterator i = m_Variables.find(varName);
    if(!(i==m_Variables.end()))
    {
        m_Variables.erase(i);
        res = true;
    } else
    {
        iThrow(CRITICAL(FSM_VARIABLE_NOT_FOUND), varName);
    }
    return res;
}

bool CFiniteStateMachine::SetVariable(const char *varName, const int value)
{
    bool res = false;
    Var_Map::iterator i = m_Variables.find(varName);
    if(!(i==m_Variables.end()))
    {
        (*i).second = value;
        res = true;
    } else {
        iThrow(CRITICAL(FSM_VARIABLE_NOT_FOUND), varName);
    }
    return res;
}

bool CFiniteStateMachine::GetVariable(const char *varName, int &value) const
{
    bool res = false;
    Var_Map::const_iterator i = m_Variables.find(varName);
    if(!(i==m_Variables.end()))
    {
        value = (*i).second;
        res = true;
    } else {
        iThrow(CRITICAL(FSM_VARIABLE_NOT_FOUND), varName);
    }
    return res;
}

Var_Map::const_iterator CFiniteStateMachine::GetVarsBegin() const
{
    return m_Variables.begin();
}

Var_Map::const_iterator CFiniteStateMachine::GetVarsEnd() const
{
    return m_Variables.end();
}

////////////////////////////////////////////////////////////////////////
//        Operate FSM
////////////////////////////////////////////////////////////////////////
bool CFiniteStateMachine::SetInitialState(const char *stateName)
{
    bool res = false;
    if(m_Running)
    {
        iThrow(WARNING(FSM_INITIAL_AFTER_START));
    } else
    {
        m_InitialState = stateName;
        res = true;
    }
    return res;
}

const char* CFiniteStateMachine::GetInitialState() const
{
    return m_InitialState.c_str();
}

bool CFiniteStateMachine::Start(const char *stateName)
{
    if(!m_Running)
    {
        if(stateName!=NULL)
        {
            m_InitialState = stateName;
        }
        m_CurrentState = GetState(m_InitialState.c_str());
        if (!m_CurrentState)
        {
            iRethrow();
        } else
        {
            m_CurrentState->Activate();
            m_Running=true;
        }
    } else
    {
        iThrow(CRITICAL(FSM_ALREADY_STARTED));
    }
    return m_Running;
}

void CFiniteStateMachine::Stop()
{
    if (m_Running)
    {
        m_CurrentState->Deactivate();
        m_CurrentState=NULL;
        m_Running = false;
    }
}

bool CFiniteStateMachine::ProcessEvent(const char *Event, void *a, void *b, bool *consumed)
{
    bool res = false;
    if(m_Running)
    {
        CFSMTransition* trans = NULL;
        if (m_CurrentState->GetTransition(&trans, Event, a, b))
        {
            if (trans)
            {
                if(consumed)
                {
                    *consumed = true;
                }
                if(! (res = trans->Execute(Event, a, b)))
                {
                    iRethrow();
                }
            } else
            {
                if (consumed)
                {
                    *consumed = false;
                }
                res = true;
            }
        } else
        {
            iRethrow();
        }
    } else
    {
        iThrow(CRITICAL(FSM_NOT_STARTED));
    }
    return res;
}

CFSMState *CFiniteStateMachine::GetCurrentState() const
{
    return m_CurrentState;
}

bool CFiniteStateMachine::Clear()
{
    bool res = false;
    if (!m_Running)
    {
        // delete states
        State_Map::iterator s = m_States.begin();
        if(!(s==m_States.end()))
        {
            delete (*s).second;
            (*s).second = NULL;
            m_States.erase(s);
            s = m_States.begin(); // The first is erased. Now get the new begin.

        }

        // delete varaibles
        Var_Map::iterator v = m_Variables.begin();
        if(!(v==m_Variables.end()))
        {
            (*v).second = 0;
            m_Variables.erase(v);
            v = m_Variables.begin();
        }

        m_InitialState = "";
        res = true;
    } else
    {
        iThrow(CRITICAL(FSM_CLEAR_STARTED));
    }
    return res;
}

////////////////////////////////////////////////////////////////////////
//        Private Functions
////////////////////////////////////////////////////////////////////////

void CFiniteStateMachine::RegisterBaseTypes()
{
    Register("State", CFSMState::Create);
    Register("SimpleTransition", CFSMSimpleTransition::Create);
    Register("PushTransition", CFSMPushTransition::Create);
    Register("PopTransition", CFSMPopTransition::Create);
    Register("TestVariable", CFSMConditionTestVariable::Create);
    Register("SetVariable", CFSMActionSetVariable::Create);
    Register("IncrVariable", CFSMActionIncrVariable::Create);
}

bool CFiniteStateMachine::SwitchToState(CFSMState* newState)
{
    bool res = true;
    if (newState)
    {
        if (newState != m_CurrentState)
        {
            if (m_CurrentState) res = m_CurrentState->Deactivate();
            if (res)
            {
                m_CurrentState=newState;
                res = m_CurrentState->Activate();
                if (res)
                {
                    res = ProcessEvent("UPDATE");
                    if (!res)
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
        }
    } else
    {
        iThrow(CRITICAL(FSM_STATE_NOT_FOUND), "NULL");
    }
    return res;
}

bool CFiniteStateMachine::PushState(CFSMState* newState)
{
    bool res = SwitchToState(newState);
    if (res)
    {
        // push the state onto the stack
    } else
    {
        iRethrow();
    }
    return res;
}

bool CFiniteStateMachine::PopState()
{
    bool res = false;
    CFSMState *newState = NULL;

    // pop state from stack, return false if not stack ampty

    SwitchToState(newState);
    if( !res )
    {
        iRethrow();
    }
    return res;
}

//////////////////////////////////////////////////////////////////////
// Register dynamic loaded DLL
//////////////////////////////////////////////////////////////////////
void CFiniteStateMachine::Register(const char *className, FSMObjectCreateMethod create)
{
    // register
    if (FSMObjectCreationMethods.find(className)!=FSMObjectCreationMethods.end())
    {
        iThrow(WARNING(FSM_CLASS_REGISTERED_TWICE), className);
        return;
    }
    FSMObjectCreationMethods.insert(pair<const string,FSMObjectCreateMethod>(className,create));
}

//////////////////////////////////////////////////////////////////////
// Create Instance for named Class
//////////////////////////////////////////////////////////////////////
CFSMObject* CFiniteStateMachine::Create(const char *className)
{
    if (className==NULL || className[0]=='\0')
    {
        iThrow(WARNING(FSM_CANNOT_CREATE_CLASS), "");
        return NULL;
    }

    FSMObjectCreate_Map::iterator item = FSMObjectCreationMethods.find(className);
    if (item==FSMObjectCreationMethods.end())
    {
#ifndef _WIN32_WCE
        if(!LoadClass(className))
        {
            iRethrow();
            return NULL;
        }

        item = FSMObjectCreationMethods.find(className);
        if (item==FSMObjectCreationMethods.end())
        {
            iThrow(WARNING(FSM_CANNOT_CREATE_CLASS), className);
            return NULL;
        }
#else
        iThrow(WARNING(FSM_CANNOT_CREATE_CLASS), className);
        return NULL;
#endif
    }

    FSMObjectCreateMethod create = (*item).second;
    CFSMObject *obj = create();
    obj->SetFSM(this);
    obj->SetClassName(className);
    return (obj);
}

#ifndef _WIN32_WCE
//////////////////////////////////////////////////////////////////////
// Dynamiclly load external Object
//////////////////////////////////////////////////////////////////////
bool CFiniteStateMachine::LoadClass(const char *className)
{

    string libName("");

#    ifdef WIN32
    libName += className;
    libName += ".dll";
#    else
    libName += "lib";
    libName += className;
    libName += ".so";
#    endif

#    ifdef HAVE_LIBIWRAPPER
    iLibrary* lib = iLibrary::Load(libName.c_str());
#    else
#        ifdef WIN32
        HINSTANCE lib = LoadLibrary(libName.c_str());
#        else
        void *lib = dlopen(libName.c_str(), RTLD_NOW);
#        endif
#    endif

    if (lib==NULL)
    {
        return false;
    }

    InitClassF InitClass;
#    ifdef HAVE_LIBIWRAPPER
        InitClass = (InitClassF)lib->GetProc("InitClass");
#    else
#        ifdef WIN32
        InitClass = (InitClassF)GetProcAddress(lib, "InitClass");
#        else
        InitClass = (InitClassF)dlsym(lib, "InitClass");
#        endif
#    endif

    if (!InitClass)
    {
        iHandleLastError();
        return false;
    }

    (*InitClass)(this);

    // the library have to be unloaded at application exit !!!

    return true;
}
#endif


/////////////////////////////////////////////////
}; // namespace FSM

