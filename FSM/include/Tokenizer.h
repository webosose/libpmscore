#if !defined(AFX_TOKENIZER_H__ADD073B4_5613_4A48_86C6_871FB8D2946F__INCLUDED_)
#define AFX_TOKENIZER_H__ADD073B4_5613_4A48_86C6_871FB8D2946F__INCLUDED_

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

// $Log: Tokenizer.h,v $
// Revision 1.2  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.1  2002/06/28 11:10:36  vofka
// first time on sourceforge.net cvs
//
// Revision 1.7  2002/06/20 12:22:11  bobka
// LGPL license
//
// Revision 1.6  2001/12/21 12:01:21  bobka
// laeuft auf linux
//
// Revision 1.5  2001/12/07 13:50:59  bobka
// clean up improved (delete tokenizer)
// CTokenizer exported
// LoadFSM public
//
// Revision 1.4  2001/12/06 16:30:37  bobka
// comments <!-- ... --> support added
// closed tag < .. /> support added
//
// Revision 1.3  2001/11/28 15:42:01  bobka
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

#include <map>
#include <string>
#include <cstdio>
#include <fstream>

/////////////////////////////////////////////////////////////////////////
namespace FSM {
    using std::map;
    using std::string;
    using std::less;
    #ifndef __SGI_STL_STRING
        using std::ifstream;
    #endif

typedef map< string, string > Attrib_Map;
typedef Attrib_Map::value_type TAM_VT;

// externals
class CFiniteStateMachine;
class CFSMState;

class CTokenizer
{
    public:
        friend class CTOKActionSetTag;
        friend class CTOKActionSetAttribute;

        CTokenizer(FILE *inFile);
        virtual ~CTokenizer();

        bool    GetTag();
        const char*    GetTagName();

        bool    GetAttribute(char *AttribName, char *retValue, int len);
        bool    IsTagClosed();

    protected:
        Attrib_Map    Attributes;
        string        tag;

    private:
        FILE        *inFile;
        char name[255];
        char* name_p;
        char value[255];
        char* value_p;
        char tagname[255];
        char* tagname_p;
        CFiniteStateMachine *fsm;
        CFSMState *state;
        bool tagClosed;
};

}; // namespace FSM

#endif // !defined(AFX_TOKENIZER_H__ADD073B4_5613_4A48_86C6_871FB8D2946F__INCLUDED_)
