#ifndef UTILS_VERIFY_SIGNATURE_HPP
#define UTILS_VERIFY_SIGNATURE_HPP
/*
* Copyright 2014 MTSec, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*  Titles			                                          Authors	         Date
 * - Copyright 2014 Facebook, Inc. Licensed under the Apache License, Version 2.0 (the "License");
 *                                                     R.Chatsiri         23/06/2014
 */

#include <stdint.h>
#include <cstddef>

/*
 * Checksum functions
 */

namespace utils{

/**
 * Compute the CRC-32C checksum of a buffer, using a hardware-accelerated
 * implementation if available or a portable software implementation as
 * a default.
 *
 * @note CRC-32C is different from CRC-32; CRC-32C starts with a different
 *       polynomial and thus yields different results for the same input
 *       than a traditional CRC-32.
 */
uint32_t crc32c(const uint8_t* data, size_t nbytes,
    uint32_t startingChecksum = ~0U);

} // utils

#endif /* UTILS_VERIFY_SIGNATURE_HPP */
