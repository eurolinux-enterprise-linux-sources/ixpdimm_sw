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
 * This file contains the provider for the FWEventLog instances
 * that contain FW debug logs.
 */


#ifndef	_WBEM_SUPPORT_FWEVENTLOG_FACTORY_H_
#define	_WBEM_SUPPORT_FWEVENTLOG_FACTORY_H_

#include <string>

#include <exception/NvmExceptionLibError.h>
#include <framework_interface/NvmInstanceFactory.h>

namespace wbem
{
namespace support
{
// object path strings
static const std::string FWEVENTLOG_ELEMENTNAME = "FW Debug Log for: "; //!< Element Name static
static const std::string FWEVENTLOG_CREATIONCLASSNAME = std::string(NVM_WBEM_PREFIX) + "FWEventLog"; //!< Creation ClassName static

static const int CIMFWEVENTLOG_DISABLED = 32768;
static const int CIMFWEVENTLOG_ERROR = 32769;
static const int CIMFWEVENTLOG_WARNING = 32770;
static const int CIMFWEVENTLOG_INFO = 32771;
static const int CIMFWEVENTLOG_DEBUG = 32772;
static const int CIMFWEVENTLOG_UNKNOWN = 0;

/*!
 * Represents the support data maintained by the management software
 */
class NVM_API FWEventLogFactory  : public framework_interface::NvmInstanceFactory
{
	public:
		// constructor
		FWEventLogFactory();
		~FWEventLogFactory();

		// methods required for CIM provider support, note that if
		// you don't see an expected method here the inherited version is used and
		// it throws an unsupported exception
		framework::Instance* getInstance(framework::ObjectPath &path,
				framework::attribute_names_t &attributes) throw (framework::Exception);
		framework::instance_names_t* getInstanceNames() throw (framework::Exception);

		/*!
		 * Provider for nvm_get_fw_log_level
		 * @param[in] device_uid
		 * 		The device identifier. @n
		 * @param[out,in] p_log_level
		 * 		A buffer for the log_level, allocated by the caller. @n
		 * @return Returns one of the following @link #return_code return_codes: @endlink @n
		 * 		#NVM_SUCCESS @n
		 * 		#NVM_ERR_NOTSUPPORTED @n
		 * 		#NVM_ERR_NOMEMORY @n
		 * 		#NVM_ERR_INVALIDPARAMETER @n
		 */
		int (*m_GetFwLogLevel)(const NVM_UID device_uid, enum fw_log_level *p_log_level);

		/*!
		 * Removes direct dependency on static method
		 * @return
		 */
		std::vector<std::string> (*m_GetManageabledeviceUids)();

	private:
		void populateAttributeList(framework::attribute_names_t &attributes)
			throw (framework::Exception);
};

} // server
} // wbem
#endif  // #ifndef _WBEM_SUPPORT_FWEVENTLOG_FACTORY_H_
