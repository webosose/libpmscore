#if !defined(AFX_FSMACTION_H__7C6ABC20_3696_40ED_9DC4_8C06E73BFC85__INCLUDED_)
#define AFX_FSMACTION_H__7C6ABC20_3696_40ED_9DC4_8C06E73BFC85__INCLUDED_

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

// $Log: FSMAction.h,v $
// Revision 1.2  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.1  2002/06/28 11:10:36  vofka
// first time on sourceforge.net cvs
//
// Revision 1.7  2002/06/20 12:22:10  bobka
// LGPL license
//
// Revision 1.6  2002/06/20 11:27:02  ozone
// I4D-Code aus FSM entfernt
//
// Revision 1.5  2002/06/17 10:50:14  ozone
// CFSMActionIlluCommand impementiert.
// Damit ist es möglich ein belibiges Commando in I4D auszuführen.
//
// Revision 1.4  2001/12/19 17:14:00  bobka
// IncrVariable action added
//
// Revision 1.3  2001/11/28 15:42:00  bobka
// a lot of changes
//
// Revision 1.2  2001/11/23 09:27:57  bobka
// Load/Save funktioniert.
// FSM* im Fileformat entfernt.
// streams entfernt.
// Laeuft unter Win32 und unter WinCE (WinCE_STL Modul auschecken)
// static library unter WinCE funktioniert nicht, nur DLL.
//
// Revision 1.1  2001/11/15 10:37:31  bobka
// initial checkin
// not tested on linux!
//


// Base-Class
#include "FSMObject.h"

/////////////////////////////////////////////////////////////////////////
namespace FSM {

// forward reference
class CFiniteStateMachine;

/**
 * Base class for Actions
 */
class CFSMAction :  public CFSMObject
{
    public:
        friend class CFSMState;
        friend class CFSMTransition;

        // Overloadables
        virtual bool Execute(const char* event, void *a, void *b) =0;

    protected:

        CFSMAction();
        virtual ~CFSMAction();
    private:
};

/**
 * Action, that sets a variable to the specified value.
 */
class CFSMActionSetVariable :  public CFSMAction
{
    public:
        friend class CFSMState;
        friend class CFSMTransition;
        friend class CFiniteStateMachine;

        // Construction/Destruction
        static CFSMObject *Create();
        virtual void DeleteThis();

        // Get/Set
        void SetVariable(const char* varName);
        void SetValue(int val);

        const char* GetVariable() const;
        int         GetValue() const;

        // Overloaded
        virtual bool Execute(const char* event, void *a, void *b);
        virtual bool SetParameter(const char* paramName, const char* paramValue);

    protected:

        // Overloaded
        virtual bool SaveParameters(CFSMSaver* out) const;

    private:
        CFSMActionSetVariable();
        virtual ~CFSMActionSetVariable();

        string variableName;
        int value;
};

/**
 * Action, that increments a variable by the specified value (1 by default).
 */
class CFSMActionIncrVariable :  public CFSMAction
{
    public:
        friend class CFSMState;
        friend class CFSMTransition;
        friend class CFiniteStateMachine;

        // Construction/Destruction
        static CFSMObject *Create();
        virtual void DeleteThis();

        // Get/Set
        void SetVariable(const char* varName);
        void SetIncrement(int val);

        const char* GetVariable() const;
        int         GetIncrement() const;

        // Overloaded
        virtual bool Execute(const char* event, void *a, void *b);
        virtual bool SetParameter(const char* paramName, const char* paramValue);

    protected:

        // Overloaded
        virtual bool SaveParameters(CFSMSaver* out) const;

    private:
        CFSMActionIncrVariable();
        virtual ~CFSMActionIncrVariable();

        string variableName;
        int increment;
};

}; // namespace FSM

#endif // !defined(AFX_FSMACTION_H__7C6ABC20_3696_40ED_9DC4_8C06E73BFC85__INCLUDED_)
