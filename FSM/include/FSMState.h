#if !defined(AFX_FSMSTATE_H__72C06899_E8FF_4311_A306_97314E778192__INCLUDED_)
#define AFX_FSMSTATE_H__72C06899_E8FF_4311_A306_97314E778192__INCLUDED_

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

// $Log: FSMState.h,v $
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
// Revision 1.5  2002/06/20 12:22:10  bobka
// LGPL license
//
// Revision 1.4  2002/05/03 23:21:56  bobka
// Loader externalisiert, aber der alte ist noch drin.
// Iterators for states und variables added.
// some minor bugs fixed.
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
// Revision 1.1  2001/11/15 10:37:32  bobka
// initial checkin
// not tested on linux!
//

// Base-Class
#include "FSMObject.h"

/////////////////////////////////////////////////////////////////////////
namespace FSM {

// forward reference
class CFiniteStateMachine;
class CFSMTransition;
class CFSMAction;
class CFSMCondition;

typedef list< CFSMTransition* > Transition_List;
typedef list< CFSMAction* > Action_List;

class CFSMState : public CFSMObject
{
    public:
        friend class CFiniteStateMachine;

        //Transitions
        CFSMTransition* AddTransition(const char* onEvent="ANY", const char *className="SimpleTransition");
        CFSMTransition* AddSimpleTransition(const char *toStateName, const char* onEvent="ANY");

        /** @return the number of transitions */
        int GetTransitionNumber() const;

        /** @return the index-th transition. index==0 -> first transition. NULL if out of range */
        CFSMTransition* GetTransition(int index) const;

        /** Delete one transition */
        bool DeleteTransition(CFSMTransition* trans);

        /**
         * Use (*iterator) to get the pointer to the transition.
         * Use ++iterator to get the next iterator.
         * @return the begin-iterator of the transitions container
         */
        Transition_List::const_iterator GetTransitionsBegin() const;

        /** @return the end-iterator of the transitions container */
        Transition_List::const_iterator GetTransitionsEnd() const;

        // Enter/Leave Actions
        /** Create an action, that is executed at entering the state */
        CFSMAction* AddEnterAction(const char *className);

        /** Create an action, that is executed at leaving the state */
        CFSMAction* AddLeaveAction(const char *className);

        /** Delete an action */
        bool DeleteAction(CFSMAction *action);

        /** @return the index-th enter action. index==0 -> first action. NULL if out of range */
        CFSMAction* GetEnterAction(int index) const;

        /** @return the index-th leave action. index==0 -> first action. NULL if out of range */
        CFSMAction* GetLeaveAction(int index) const;

        /**
         * Use (*iterator) to get the pointer to the action.
         * Use ++iterator to get the next iterator.
         * @return the begin-iterator of the actions container
         */
        Action_List::const_iterator GetEnterActionsBegin() const;

        /** @return the end-iterator of the actions container */
        Action_List::const_iterator GetEnterActionsEnd() const;

        /**
         * Use (*iterator) to get the pointer to the action.
         * Use ++iterator to get the next iterator.
         * @return the begin-iterator of the actions container
         */
        Action_List::const_iterator GetLeaveActionsBegin() const;

        /** @return the end-iterator of the actions container */
        Action_List::const_iterator GetLeaveActionsEnd() const;

        // Get/Set
        const char*    GetName() const;

    protected:
        // Construction/Destruction
        static CFSMObject *Create();
        virtual void DeleteThis();

        // Set
        void SetName(const char *name);

        // overloadables called by CFiniteStateMachine
        /**
         * Get the Transition, that would be triggered.
         * @param trans pointer, that is set by this method.
         * @param event the id of the event
         * @param a event parameter
         * @param b event parameter
         * @return false if no transition would be triggered.
         */
        virtual bool GetTransition(CFSMTransition** trans, const char *event, void *a, void *b);

        /**
         * FSM has changed into this state.
         * Enter Actions are executed.
         */
        virtual bool Activate();

        /**
         * FSM has changed the state.
         * Leave Actions are executed.
         */
        virtual bool Deactivate();

        /** the list of transitions. */
        Transition_List m_Transitions;

        /** the list of enter actions. */
        Action_List m_EnterActions;

        /** the list of leave actions. */
        Action_List m_LeaveActions;

        /** Use create to create a new instance. */
        CFSMState();

        /** Use DeleteThis() method instead of delete, because of memory. */
        virtual ~CFSMState();

    private:

        string m_Name;
};

}; // namespace FSM

#endif // !defined(AFX_FSMSTATE_H__72C06899_E8FF_4311_A306_97314E778192__INCLUDED_)
