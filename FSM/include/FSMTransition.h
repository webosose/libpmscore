#if !defined(AFX_FSMTRANSITION_H__0E355DD5_595A_429E_950B_03A7F175172E__INCLUDED_)
#define AFX_FSMTRANSITION_H__0E355DD5_595A_429E_950B_03A7F175172E__INCLUDED_

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

// $Log: FSMTransition.h,v $
// Revision 1.3  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.2  2002/07/22 17:21:00  vofka
// SimpleTransition, PushTransition, PopTransition added, not implemented yet
//
// Revision 1.1  2002/06/28 11:10:36  vofka
// first time on sourceforge.net cvs
//
// Revision 1.6  2002/06/20 12:22:10  bobka
// LGPL license
//
// Revision 1.5  2002/05/03 23:21:57  bobka
// Loader externalisiert, aber der alte ist noch drin.
// Iterators for states und variables added.
// some minor bugs fixed.
//
// Revision 1.4  2002/01/16 11:53:44  bobka
// ???
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
class CFSMAction;
class CFSMCondition;

typedef list< CFSMAction* > Action_List;
typedef list< CFSMCondition* > Condition_List;

/**
 * Base class for transitions.
 */
class CFSMTransition :  public CFSMObject
{
    public:
        friend class CFSMState;
        friend class CFiniteStateMachine;

        // Actions
        CFSMAction* AddAction(const char *className);
        /** @return the index-th action. index==0 -> first action. NULL if out of range */
        CFSMAction* GetAction(int index) const;
        bool DeleteAction(CFSMAction *action);

        /**
         * Use (*iterator) to get the pointer to the action.
         * Use ++iterator to get the next iterator.
         * @return the begin-iterator of the actions container
         */
        Action_List::const_iterator GetActionsBegin() const;

        /** @return the end-iterator of the actions container */
        Action_List::const_iterator GetActionsEnd() const;

        // Conditions
        CFSMCondition* AddCondition(const char *className="TestVariable");
        /** @return the index-th condition. index==0 -> first condition. NULL if out of range */
        CFSMCondition* GetCondition(int index) const;
        bool DeleteCondition(CFSMCondition *condition);

        /**
         * Use (*iterator) to get the pointer to the condition.
         * Use ++iterator to get the next iterator.
         * @return the begin-iterator of the conditions container
         */
        Condition_List::const_iterator GetConditionsBegin() const;

        /** @return the end-iterator of the conditions container */
        Condition_List::const_iterator GetConditionsEnd() const;

        // Get/Set
        CFSMState*   GetFromState() const;        // get the FromState Attribute
        const char * GetOnEvent() const;          // get the OnEvent Attribute
        void SetOnEvent(const char* event="ANY"); //

    protected:

        // Set
        void SetFromState(const char *stateName);

        /// overloadables ///

        /** the answer is in result!!!
         *  @return is false if there where errors.
         *  Do not execute actions here, just check.
         */
        virtual bool Check(bool &result, const char *event, void *a, void *b);

        /**
         * Called if the transition match.
         * Execute actions.
         */
        virtual bool Execute(const char* event, void *a, void *b);

        /**
         * Called when the state is activated.
         * Calls Activate() of conditions (e.g. to start a timer)
         */
        virtual bool Activate();

        /**
         * Called when the state is deactivated
         * Calls Deactivate() of conditions (e.g. to stop a timer)
         */
        virtual bool Deactivate();

        CFSMTransition();
        virtual ~CFSMTransition();
    private:

        Action_List m_Actions;
        Condition_List m_Conditions;
        string m_FromState;
        string m_ToState;
        string m_Event;
};

/**
 * Simple transition.
 */
class CFSMSimpleTransition :  public CFSMTransition
{
    public:
        friend class CFSMState;
        friend class CFiniteStateMachine;

        // Get/Set
        CFSMState*   GetToState() const;          // get the ToState Attribute
        void SetToState(const char *stateName);   // set the ToState attribute

        virtual bool SetParameter(const char* paramName, const char* paramValue);

    protected:
        // Construction/Destruction
        static CFSMObject *Create();
        virtual void DeleteThis();


        /// overloaded ///

        /**
         * overloaded CFSMObject::SaveParameters.
         */
        virtual bool SaveParameters(CFSMSaver* out) const;

        /**
         * Called if the transition match.
         * Execute actions and switch to a new state.
         */
        virtual bool Execute(const char* event, void *a, void *b);

        CFSMSimpleTransition();
        virtual ~CFSMSimpleTransition();
    private:

        string m_ToState;
};

/**
 * Push transition. The state is pushed to the stack.
 */
class CFSMPushTransition :  public CFSMSimpleTransition
{
    public:
        friend class CFSMState;
        friend class CFiniteStateMachine;

    protected:
        // Construction/Destruction
        static CFSMObject *Create();
        virtual void DeleteThis();


        /// overloaded ///

        /**
         * Called if the transition match.
         * Execute actions and switch to a new state with push.
         */
        virtual bool Execute(const char* event, void *a, void *b);

        CFSMPushTransition();
        virtual ~CFSMPushTransition();
    private:

};

/**
 * Pop transition. The state is poped from stack.
 */
class CFSMPopTransition :  public CFSMTransition
{
    public:
        friend class CFSMState;
        friend class CFiniteStateMachine;

    protected:
        // Construction/Destruction
        static CFSMObject *Create();
        virtual void DeleteThis();


        /// overloaded ///

        /**
         * Called if the transition match.
         * Execute actions and switch to a new state with push.
         */
        virtual bool Execute(const char* event, void *a, void *b);

        CFSMPopTransition();
        virtual ~CFSMPopTransition();
    private:

};

}; // namespace FSM

#endif // !defined(AFX_FSMTRANSITION_H__0E355DD5_595A_429E_950B_03A7F175172E__INCLUDED_)
