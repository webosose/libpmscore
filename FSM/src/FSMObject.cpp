// $Log: FSMObject.cpp,v $
// Revision 1.2  2002/08/20 14:48:41  vofka
// FiniteStateMachine-config.h removed from installing. Loading exported to the loader class. Saver introduced. Error codes exported to FSMErrorCodes.
//
// Revision 1.1  2002/07/22 17:20:59  vofka
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

#include "FSM.h"
#include "FiniteStateMachine-config.h"
#include "FSMErrorCodes.h"

namespace FSM {

// Initialize the Name of the Class
const string ClassName = "";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFSMObject::CFSMObject()
{
    myStateMachine = NULL;
    ClassName = "";
}

CFSMObject::~CFSMObject()
{
    myStateMachine = NULL;
    ClassName = "";
}

//////////////////////////////////////////////////////////////////////
// Set/Get Methods
//////////////////////////////////////////////////////////////////////
void CFSMObject::SetFSM(CFiniteStateMachine *ptr)
{
    myStateMachine = ptr;
}

CFiniteStateMachine *CFSMObject::GetFSM() const
{
    return myStateMachine;
}

void CFSMObject::SetClassName(const char *name)
{
    ClassName = name;
}

const char*    CFSMObject::GetNameOfClass() const
{
    return ClassName.c_str();
}

bool CFSMObject::SetParameter(const char* paramName, const char* paramValue)
{
    iThrow(WARNING(FSM_UNKNOWN_PARAM), paramName, this->GetNameOfClass());
    return false;
}

///////////////////////////////////////////////////////////////////
//        Input/Output
///////////////////////////////////////////////////////////////////
bool CFSMObject::SaveParameters(CFSMSaver* out) const
{
    return true;
}

/////////////////////////////////////////////////
}; // namespace FSM

