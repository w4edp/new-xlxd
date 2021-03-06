//
//  cambeserver.h
//  ambed
//
//  Created by Jean-Luc Deltombe (LX3JL) on 13/04/2017.
//  Copyright © 2015 Jean-Luc Deltombe (LX3JL). All rights reserved.
//  Copyright © 2020 Thomas A. Early, N7TAE
//
// ----------------------------------------------------------------------------
//    This file is part of ambed.
//
//    xlxd is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    xlxd is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
// ----------------------------------------------------------------------------

#ifndef cambeserver_h
#define cambeserver_h

#include "ccontroller.h"

////////////////////////////////////////////////////////////////////////////////////////
// class

class CAmbeServer
{
public:

	// operation
	bool Start(void);
	void Stop(void);

	// get
	const char *GetListenIp(void) const   { return m_Controller.GetListenIp(); }

	// set
	void SetListenIp(const char *ip)      { m_Controller.SetListenIp(ip); }

protected:
	// objects
	CController     m_Controller;

public:
#ifdef DEBUG_DUMPFILE
	std::ofstream        m_DebugFile;
#endif
};

////////////////////////////////////////////////////////////////////////////////////////
#endif /* cambeserver_h */
