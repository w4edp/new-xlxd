//
//  cpeer.cpp
//  xlxd
//
//  Created by Jean-Luc Deltombe (LX3JL) on 10/12/2016.
//  Copyright © 2016 Jean-Luc Deltombe (LX3JL). All rights reserved.
//  Copyright © 2020 Thomas A. Early, N7TAE
//
// ----------------------------------------------------------------------------
//    This file is part of xlxd.
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

#include "main.h"
#include <string.h>
#include "creflector.h"
#include "cpeer.h"


////////////////////////////////////////////////////////////////////////////////////////
// constructor


CPeer::CPeer()
{
    ::memset(m_ReflectorModules, 0, sizeof(m_ReflectorModules));
    m_ConnectTime = std::time(nullptr);
    m_LastHeardTime = std::time(nullptr);
}

CPeer::CPeer(const CCallsign &callsign, const CIp &ip, const char *modules, const CVersion &version)
{
    m_Callsign = callsign;
    m_Ip = ip;
    ::memset(m_ReflectorModules, 0, sizeof(m_ReflectorModules));
    ::strncpy(m_ReflectorModules, modules, sizeof(m_ReflectorModules)-1);
    m_Version = version;
    m_LastKeepaliveTime.Now();
    m_ConnectTime = std::time(nullptr);
    m_LastHeardTime = std::time(nullptr);
}

CPeer::CPeer(const CPeer &peer)
{
    m_Callsign = peer.m_Callsign;
    m_Ip = peer.m_Ip;
    ::memcpy(m_ReflectorModules, peer.m_ReflectorModules, sizeof(m_ReflectorModules));
    m_Version = peer.m_Version;
    m_LastKeepaliveTime = peer.m_LastKeepaliveTime;
    m_ConnectTime = peer.m_ConnectTime;
    m_LastHeardTime = peer.m_LastHeardTime;
}

////////////////////////////////////////////////////////////////////////////////////////
// destructors

CPeer::~CPeer()
{
    m_Clients.clear();
}

////////////////////////////////////////////////////////////////////////////////////////
// operators

bool CPeer::operator ==(const CPeer &peer) const
{
    if (peer.m_Callsign != m_Callsign)
		return false;
    if (peer.m_Ip != m_Ip)
		return false;
    if (! (peer.m_Version == m_Version))
		return false;
	auto it1 = cbegin();
	auto it2 = peer.cbegin();
    while (true)
    {
		if (it1==cend() && it2==peer.cend())
			break;
		if (it1==cend() || it2==peer.cend())
			return false;
		if (*it1 != *it2)
			return false;
        it1++;
		it2++;
    }
    return true;
}


////////////////////////////////////////////////////////////////////////////////////////
// status

bool CPeer::IsAMaster(void) const
{
    for ( auto it=cbegin(); it!=cend(); it++ )
    {
        if ((*it)->IsAMaster())
			return true;
    }
    return false;
}

void CPeer::Alive(void)
{
    m_LastKeepaliveTime.Now();;
    for ( auto it=begin(); it!=end(); it++ )
    {
        (*it)->Alive();
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// reporting

void CPeer::WriteXml(std::ofstream &xmlFile)
{
    xmlFile << "<PEER>" << std::endl;
    xmlFile << "\t<Callsign>" << m_Callsign << "</Callsign>" << std::endl;
    xmlFile << "\t<IP>" << m_Ip.GetAddress() << "</IP>" << std::endl;
    xmlFile << "\t<LinkedModule>" << m_ReflectorModules << "</LinkedModule>" << std::endl;
    xmlFile << "\t<Protocol>" << GetProtocolName() << "</Protocol>" << std::endl;
    char mbstr[100];
    if (std::strftime(mbstr, sizeof(mbstr), "%A %c", std::localtime(&m_ConnectTime)))
    {
        xmlFile << "\t<ConnectTime>" << mbstr << "</ConnectTime>" << std::endl;
    }
    if (std::strftime(mbstr, sizeof(mbstr), "%A %c", std::localtime(&m_LastHeardTime)))
    {
        xmlFile << "\t<LastHeardTime>" << mbstr << "</LastHeardTime>" << std::endl;
    }
    xmlFile << "</PEER>" << std::endl;
}

void CPeer::GetJsonObject(char *Buffer)
{
    char sz[512];
    char mbstr[100];
    char cs[16];

    if (std::strftime(mbstr, sizeof(mbstr), "%A %c", std::localtime(&m_LastHeardTime)))
    {
        m_Callsign.GetCallsignString(cs);

        ::sprintf(sz, "{\"callsign\":\"%s\",\"linkedto\":\"%s\",\"time\":\"%s\"}",
                  cs,
                  m_ReflectorModules,
                  mbstr);
        ::strcat(Buffer, sz);
    }
}
