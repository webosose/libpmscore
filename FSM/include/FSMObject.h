#if !defined(AFX_FSMOBJECT_H__4F9C75B5_BAD4_42EF_83C4_B8819F85CD2E__INCLUDED_)
#define AFX_FSMOBJECT_H__4F9C75B5_BAD4_42EF_83C4_B8819F85CD2E__INCLUDED_

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

// $Log: FSMObject.h,v $
// Revision 1.4  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.3  2002/08/13 11:06:32  vofka
// automake support added
//
// Revision 1.2  2002/07/22 17:20:59  vofka
// SimpleTransition, PushTransition, PopTransition added, not implemented yet
//
// Revision 1.1  2002/06/28 11:10:36  vofka
// first time on sourceforge.net cvs
//
// Revision 1.4  2002/06/20 12:22:10  bobka
// LGPL license
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

#include <string>

/////////////////////////////////////////////////////////////////////////
namespace FSM {
    using std::string;


class CFiniteStateMachine;
class CFSMSaver;


class CFSMObject
{
        friend class CFiniteStateMachine;
        friend class CFSMSaver;

    public:

        // Every Object knows his FSM
        CFiniteStateMachine *GetFSM() const;

        /**
         * The class name is set by FiniteStateMachine::Create().
         * So do not make Create methods public.
         */
        const char*    GetNameOfClass() const; // GetClassName confuses VC.Net

        // Overloadable
        virtual bool SetParameter(const char* paramName, const char* paramValue);

    protected:
        // services for derived
        void SetFSM(CFiniteStateMachine *ptr);
        void SetClassName(const char *name);

        // overloadables

        /**
         * Called by the saver. Do not call directly.
         * Overload and call out->SaveParameter() for each parameter.
         */
        virtual bool SaveParameters(CFSMSaver* out) const;

        /**
         * "Destructor"
         * Simple call delete this.
         * This is needed because of the memory management in shared libraries.
         * Provide a static method "Create" in derived classes that returns a new instance
         * of that class and use DeleteThis to destroy it.
         * The constructor and destructor shuld be protected.
         */
        virtual void DeleteThis() =0;

        CFSMObject();
        virtual ~CFSMObject();
    private:

        // Identify Classname
        string ClassName;

        // Pointer to my Statemachine
        CFiniteStateMachine *myStateMachine;
};

}; // namespace FSM

#endif // !defined(AFX_FSMOBJECT_H__4F9C75B5_BAD4_42EF_83C4_B8819F85CD2E__INCLUDED_)
