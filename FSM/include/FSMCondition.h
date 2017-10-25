#if !defined(AFX_FSMCONDITION_H__A3DB3A96_76D7_4B69_A67C_8F6A8601F9B9__INCLUDED_)
#define AFX_FSMCONDITION_H__A3DB3A96_76D7_4B69_A67C_8F6A8601F9B9__INCLUDED_

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

// $Log: FSMCondition.h,v $
// Revision 1.2  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.1  2002/06/28 11:10:36  vofka
// first time on sourceforge.net cvs
//
// Revision 1.5  2002/06/20 12:22:10  bobka
// LGPL license
//
// Revision 1.4  2001/12/07 11:14:50  bobka
// <!--  ... <...> ... --> fixed
// <Param Name="Expression" Value="bla=42" /> to ConditionTestVariable added
// <Variable .. /> fixed
// '>=' and '<=' bug fixed (was hidden by '>' and '<')
// SetAttribute erro at loading fixed (was not reported)
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

class CFSMCondition :  public CFSMObject
{
    public:
        friend class CFSMTransition;

        // Overloadables
        virtual bool CheckCondition(const char* event, void *a, void *b) = 0;

    protected:

        // Overloadables
        virtual bool Activate();   // called when the state is activated
        virtual bool Deactivate(); // called when the state is deactivated

        CFSMCondition();
        virtual ~CFSMCondition();
    private:
};

class CFSMConditionTestVariable :  public CFSMCondition
{
    public:
        friend class CFSMTransition;
        friend class CFiniteStateMachine;

        enum OP {
            EQUAL,
            NEQUAL,
            GREATER,
            LESS,
            GREATER_EQ,
            LESS_EQ
        };

        // Construction/Destruction
        static CFSMObject *Create();
        virtual void DeleteThis();

        // Get/Set
        void SetCondition(const char* varName, OP testCondition, int testValue);
        bool SetExpression(const char* expression); // "bla==42"

        const char* GetVariable()  const;
        OP          GetCondition() const;
        int         GetValue()     const;

        // Overloaded
        virtual bool SetParameter(const char* paramName, const char* paramValue);
        virtual bool CheckCondition(const char* event, void *a, void *b);

    protected:

        // Overloaded
        virtual bool SaveParameters(CFSMSaver *out) const;

    private:
        CFSMConditionTestVariable();
        virtual ~CFSMConditionTestVariable();

        string    variableName;
        OP        condition;
        int        value;
};

}; // namespace FSM

#endif // !defined(AFX_FSMCONDITION_H__A3DB3A96_76D7_4B69_A67C_8F6A8601F9B9__INCLUDED_)
