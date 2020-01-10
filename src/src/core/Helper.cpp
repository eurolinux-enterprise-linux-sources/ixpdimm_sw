/*
 * Copyright (c) 2016, Intel Corporation
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

#include <common/uid/uid.h>
#include <core/exceptions/InvalidArgumentException.h>
#include <core/NvmLibrary.h>
#include "Helper.h"

std::string core::Helper::uidToString(const NVM_UID uid)
{
	LogEnterExit logging(__FUNCTION__, __FILE__, __LINE__);
	NVM_UID result;
	uid_copy(uid, result);
	return std::string(result);
}

void core::Helper::stringToUid(const std::string &string, NVM_UID uid)
{
	LogEnterExit logging(__FUNCTION__, __FILE__, __LINE__);
	if (string.length() > NVM_MAX_UID_LEN - 1)
	{
		throw InvalidArgumentException("stringUid");
	}
	uid_copy(string.c_str(), uid);
}

bool core::Helper::isValidNamespaceUid(std::string uid)
{
	return uid.length() == COMMON_GUID_STR_LEN - 1;
}

bool core::Helper::isValidPoolUid(std::string uid)
{
	return uid.length() == COMMON_GUID_STR_LEN - 1;
}

std::string core::Helper::getErrorMessage(const int errorCode)
{
	LogEnterExit logging(__FUNCTION__, __FILE__, __LINE__);

	NvmLibrary &lib = NvmLibrary::getNvmLibrary();
	return lib.getErrorMessage(errorCode);
}
