/*
 * Copyright (c) 2015 2016, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Intel Corporation nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * This file contains the provider for the ManagementSoftwareIdentity instance
 * which represents the version of the NVM DIMM management software.
 */


#include "ManagementSoftwareIdentityFactory.h"
#include <nvm_management.h>
#include <LogEnterExit.h>
#include <server/BaseServerFactory.h>
#include <libinvm-cim/ExceptionBadParameter.h>
#include <string/revision.h>
#include <core/exceptions/LibraryException.h>
#include <exception/NvmExceptionLibError.h>
#include <NvmStrings.h>

wbem::software::ManagementSoftwareIdentityFactory::ManagementSoftwareIdentityFactory(
		core::system::SystemService &systemService)
throw (wbem::framework::Exception) : m_systemService(systemService)
{}

wbem::software::ManagementSoftwareIdentityFactory::~ManagementSoftwareIdentityFactory()
{}


void wbem::software::ManagementSoftwareIdentityFactory::populateAttributeList(framework::attribute_names_t &attributes)
throw (wbem::framework::Exception)
{
	// add key attributes
	attributes.push_back(INSTANCEID_KEY);

	// add non-key attributes
	attributes.push_back(ELEMENTNAME_KEY);
	attributes.push_back(MAJORVERSION_KEY);
	attributes.push_back(MINORVERSION_KEY);
	attributes.push_back(REVISIONNUMBER_KEY);
	attributes.push_back(BUILDNUMBER_KEY);
	attributes.push_back(VERSIONSTRING_KEY);
	attributes.push_back(MANUFACTURER_KEY);
	attributes.push_back(CLASSIFICATIONS_KEY);
	attributes.push_back(ISENTITY_KEY);

}

/*
 * Retrieve a specific instance given an object path
 */
wbem::framework::Instance* wbem::software::ManagementSoftwareIdentityFactory::getInstance(
		framework::ObjectPath &path, framework::attribute_names_t &attributes)
throw (wbem::framework::Exception)
{
	LogEnterExit logging(__FUNCTION__, __FILE__, __LINE__);

	// create the instance, initialize with attributes from the path
	framework::Instance *pInstance = new framework::Instance(path);
	try
	{
		m_hostName = m_systemService.getHostName();
		checkAttributes(attributes);

		// make sure the instance ID passed in matches this host
		framework::Attribute instanceID = path.getKeyValue(INSTANCEID_KEY);
		if (instanceID.stringValue() == getInstanceId())
		{
			core::Result<core::system::SoftwareInfo> swInfo = m_systemService.getSoftwareInfo();

			// ElementName - mgmt sw name + host name
			if (containsAttribute(ELEMENTNAME_KEY, attributes))
			{
				framework::Attribute a(getElementName(), false);
				pInstance->setAttribute(ELEMENTNAME_KEY, a, attributes);
			}
			// MajorVersion - Mgmt sw major version number
			if (containsAttribute(MAJORVERSION_KEY, attributes))
			{
				framework::Attribute a(swInfo.getValue().getMgmtSoftwareMajorVersion(),
						false);
				pInstance->setAttribute(MAJORVERSION_KEY, a, attributes);
			}
			// MinorVersion - Mgmt sw minor version number
			if (containsAttribute(MINORVERSION_KEY, attributes))
			{
				framework::Attribute a(swInfo.getValue().getMgmtSoftwareMinorVersion(),
						false);
				pInstance->setAttribute(MINORVERSION_KEY, a, attributes);
			}
			// RevisionNumber - Mgmt sw hotfix number
			if (containsAttribute(REVISIONNUMBER_KEY, attributes))
			{
				framework::Attribute a(swInfo.getValue().getMgmtSoftwareHotfixVersion(),
						false);
				pInstance->setAttribute(REVISIONNUMBER_KEY, a, attributes);
			}
			// BuildNumber - Mgmt sw build number
			if (containsAttribute(BUILDNUMBER_KEY, attributes))
			{
				framework::Attribute a(swInfo.getValue().getMgmtSoftwareBuildVersion(),
						false);
				pInstance->setAttribute(BUILDNUMBER_KEY, a, attributes);
			}
			// VersionString - Mgmt sw version as a string
			if (containsAttribute(VERSIONSTRING_KEY, attributes))
			{
				framework::Attribute a(swInfo.getValue().getMgmtSoftwareVersion(), false);
				pInstance->setAttribute(VERSIONSTRING_KEY, a, attributes);
			}
			// Manufacturer - Intel
			if (containsAttribute(MANUFACTURER_KEY, attributes))
			{
				framework::Attribute a("Intel", false);
				pInstance->setAttribute(MANUFACTURER_KEY, a, attributes);
			}
			// Classifications - 3, "Configuration Software"
			if (containsAttribute(CLASSIFICATIONS_KEY, attributes))
			{
				framework::UINT16_LIST classifications;
				classifications.push_back(MANAGEMENTSWIDENTITY_CLASSIFICATIONS_CONFIGSW);
				framework::Attribute a(classifications, false);
				pInstance->setAttribute(CLASSIFICATIONS_KEY, a, attributes);
			}
			// IsEntity = true
			if (containsAttribute(ISENTITY_KEY, attributes))
			{
				framework::Attribute a(true, false);
				pInstance->setAttribute(ISENTITY_KEY, a, attributes);
			}
		}
		else
		{
			throw framework::ExceptionBadParameter(INSTANCEID_KEY.c_str());
		}
	}
	catch (framework::Exception &) // clean up and re-throw
	{
		delete pInstance;
		throw;
	}
	catch (core::LibraryException &e)
	{
		delete pInstance;
		throw exception::NvmExceptionLibError(e.getErrorCode());
	}

	return pInstance;
}

/*
 * Return the object path for the single instance for this server
 */
wbem::framework::instance_names_t* wbem::software::ManagementSoftwareIdentityFactory::getInstanceNames()
throw (wbem::framework::Exception)
{
	LogEnterExit logging(__FUNCTION__, __FILE__, __LINE__);

	framework::instance_names_t *pNames = new framework::instance_names_t();
	try
	{
		m_hostName = m_systemService.getHostName();
		framework::attributes_t keys;

		// Instance ID = "HostSoftware" + host name
		keys[INSTANCEID_KEY] = framework::Attribute(getInstanceId(), true);

		// create the object path
		framework::ObjectPath path(m_hostName, NVM_NAMESPACE,
				MANAGEMENTSWIDENTITY_CREATIONCLASSNAME, keys);
		pNames->push_back(path);
	}
	catch (framework::Exception &) // clean up and re-throw
	{
		delete pNames;
		throw;
	}
	catch (core::LibraryException &e)
	{
		delete pNames;
		throw exception::NvmExceptionLibError(e.getErrorCode());
	}
	return pNames;
}

std::string wbem::software::ManagementSoftwareIdentityFactory::getInstanceId()
{
	LogEnterExit logging(__FUNCTION__, __FILE__, __LINE__);

	return MANAGEMENTSWIDENTITY_INSTANCEID + m_hostName;
}

std::string wbem::software::ManagementSoftwareIdentityFactory::getElementName()
{
	LogEnterExit logging(__FUNCTION__, __FILE__, __LINE__);

	return std::string(NVM_DIMM_NAME_LONG" Software Version ") + m_hostName;
}

