#if !defined(AFX_FSMACTION_H__7C6ABC20_3696_40ED_9DC4_6666666__INCLUDED_)
#define AFX_FSMACTION_H__7C6ABC20_3696_40ED_9DC4_6666666__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
 * The Finite State Machine (FSM) Library
 * Copyright (C) 2002 Siemens C-LAB Paderborn
 * All rights reserved.
 *
 * This program is  free  software;  you can redistribute it and/or modify it
 * under the terms of the  GNU Lesser General Public License  as published by
 * the  Free Software Foundation;  either version 2.1 of the License,  or (at
 * your option) any later version.
 *
 * This  program  is  distributed in  the  hope that it will  be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or  FITNESS FOR A  PARTICULAR PURPOSE.  See the  GNU Lesser General Public
 * License for more details.
 *
 * You should  have received  a copy of the GNU Lesser General Public License
 * along with  this program;  if not, write to the  Free Software Foundation,
 * Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

// $Log: FiniteStateMachine.h,v $
// Revision 1.4  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.3  2002/08/13 11:06:32  vofka
// automake support added
//
// Revision 1.2  2002/07/22 17:21:00  vofka
// SimpleTransition, PushTransition, PopTransition added, not implemented yet
//
// Revision 1.1  2002/06/28 11:10:36  vofka
// first time on sourceforge.net cvs
//
// Revision 1.10  2002/06/20 12:22:10  bobka
// LGPL license
//
// Revision 1.9  2002/05/03 23:21:57  bobka
// Loader externalisiert, aber der alte ist noch drin.
// Iterators for states und variables added.
// some minor bugs fixed.
//
// Revision 1.8  2002/03/07 10:02:35  bobka
// Loader separated. Only Linux tested.
//
// Revision 1.7  2001/12/17 15:10:26  bobka
// no SGI-STL,
// some little bugs fixed
// not testet on LINUX!
//
// Revision 1.6  2001/12/07 13:50:59  bobka
// clean up improved (delete tokenizer)
// CTokenizer exported
// LoadFSM public
//
// Revision 1.5  2001/12/06 16:30:37  bobka
// comments <!-- ... --> support added
// closed tag < .. /> support added
//
// Revision 1.4  2001/12/03 10:24:55  bobka
// uses iError on WinCE
//
// Revision 1.3  2001/11/28 15:42:00  bobka
// a lot of changes
//
// Revision 1.2  2001/11/23 09:27:58  bobka
// Load/Save funktioniert.
// FSM* im Fileformat entfernt.
// streams entfernt.
// Laeuft unter Win32 und unter WinCE (WinCE_STL Modul auschecken)
// static library unter WinCE funktioniert nicht, nur DLL.
//
// Revision 1.1  2001/11/15 10:37:33  bobka
// initial checkin
// not tested on linux!
//

/** @file
 * The main file defining the CFiniteStateMachine class.
 *
 * @author Jörg Stöcklein
 */

#include <map>
#include <list>
#include <string>
#include <utility>


/////////////////////////////////////////////////////////////////////////
namespace FSM {
    using std::map;
    using std::list;
    using std::string;
    using std::less;


// forward reference
class CFiniteStateMachine;
class CFSMObject;
class CFSMState;
class CFSMTransition;
class CFSMAction;
class CFSMCondition;

typedef map< string, CFSMState* > State_Map;
typedef State_Map::value_type SM_VT;

typedef map< string, int > Var_Map;
typedef Var_Map::value_type VM_VT;

typedef void (*InitClassF)(CFiniteStateMachine* fsm);
typedef CFSMObject* (*FSMObjectCreateMethod)();

typedef map< string, FSMObjectCreateMethod > FSMObjectCreate_Map;
typedef FSMObjectCreate_Map::value_type FSM_C_VT;

class CFiniteStateMachine
{
    public:
        //Constructor
        CFiniteStateMachine();

        //Destructor
        ~CFiniteStateMachine();

        // States
        CFSMState* AddState(const char *stateName, const char *className="State");
        CFSMState* GetState(const char *stateName) const;
        bool       DeleteState(CFSMState *state);
        bool       DeleteState(const char *stateName);

        /**
         * Use (*iterator) to get the pointer to the state.
         * Use ++iterator to get the next iterator.
         * @return the begin-iterator of the states container
         */
        State_Map::const_iterator GetStatesBegin() const;

        /** @return the end-iterator of the states container */
        State_Map::const_iterator GetStatesEnd() const;

        // Transitions
        CFSMTransition* AddTransition(const char* fromStateName, const char* onEvent="ANY", const char* className="SimpleTransition");
        CFSMTransition* AddSimpleTransition(const char* fromStateName, const char* toStateName, const char* onEvent="ANY");

        /**
         * No more supported. Use the DeteleTransition(const char* stateName, int index)
         *
         * Delete the first transition that matches given parameters. NULL if not found or error.
         * @param fromState NULL==any
         * @param event NULL==any
         * @param toState NULL==any
         */
        //bool DeleteTransition(const char* fromState=NULL, const char* event=NULL, const char* toState=NULL);

        /**
         * Delete the specified transition. NULL if not found or error.
         * @param fromState the name of the state
         * @param index index-th transition. index==0 -> first transition
         */
        bool DeleteTransition(const char* fromState, int index);

        // Conditions
        /**
         * Create a new condition of the given class and add it to a transition.
         * @param stateName the state id.
         * @param transition the transition number of the state. First transition has the number 0.
         * @param className the class name of the condition.
         * @return new created condition or NULL if failed.
         */
        CFSMCondition* AddCondition(const char *stateName, int transition, const char *className="TestVariable");

        // Actions
        /**
         * Create a new action to a state, that is executed each time the state is entered.
         */
        CFSMAction* AddEnterAction(const char *stateName, const char *className);

        /**
         * Create a new action to a state, that is executed each time the state is leaved.
         */
        CFSMAction* AddLeaveAction(const char *stateName, const char *className);

        /**
         * Create a new action of the given class and add it to a transition.
         * @param stateName the state id.
         * @param transition the transition number of the state. First transition has the number 0.
         * @param className the class name of the action.
         * @return new created action or NULL if failed.
         */
        CFSMAction* AddAction(const char *stateName, int transition, const char *className);

        // Variables
        /**
         * @todo strng values for variables
         */
        bool AddVariable(const char *varName, const int initValue = 0);

        /**
         */
        bool DeleteVariable(const char *varName);

        /**
         * @todo string values for variables
         */
        bool SetVariable(const char *varName, const int value);

        /**
         * @todo string values for variables
         */
        bool GetVariable(const char *varName, int &value) const;

        /** @return the begin-iterator of the variables container */
        Var_Map::const_iterator GetVarsBegin() const;

        /** @return the end-iterator of the variables container */
        Var_Map::const_iterator GetVarsEnd() const;

        // Operate FSM
        bool SetInitialState(const char *stateName);
        const char* GetInitialState() const;
        bool Start(const char *stateName=0);
        void Stop();
        bool ProcessEvent(const char *Event, void *a=NULL, void *b=NULL, bool *consumed=NULL);
        CFSMState *GetCurrentState() const;
        bool Clear(); // delete all states and all variables.

        /** Set a new state */
        bool SwitchToState(CFSMState* newState);

        /** Set a new state and push it onto the stack
         * @todo not implemented yet
         */
        bool PushState(CFSMState* newState);

        /** Pop a state from the stack and switch to it
         * @todo not implemented yet
         */
        bool PopState();

        void Register(const char *className, FSMObjectCreateMethod create);

        /** create an object of certain class */
        CFSMObject* Create(const char *className);

    protected:
#ifndef _WIN32_WCE
        bool LoadClass(const char *className);
#endif

    private:
        void RegisterBaseTypes();

        // Global registration map
        FSMObjectCreate_Map FSMObjectCreationMethods;
        State_Map   m_States;
        Var_Map     m_Variables;
        CFSMState*  m_CurrentState;
        string      m_InitialState;
        bool        m_Running;
};

}; // namespace FSM

#endif
