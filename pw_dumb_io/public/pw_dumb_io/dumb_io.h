// Copyright 2019 The Pigweed Authors
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy
// of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.
#pragma once

// This module defines a simple and unoptimized interface for byte-by-byte
// input/output. This can be done over a logging system, stdio, UART, via a
// photodiode and modulated kazoo, or basically any way to get data in and out
// of an application.
//
// This facade doesn't dictate any policies on input and output data encoding,
// format, or transmission protocol. It only requires that backends return a
// Status::OK if the operation succeeds. Backends may provide useful error
// Status types, but depending on the implementation-specific Status values is
// NOT recommended. Since this facade provides a very vague I/O interface, it
// does NOT provide tests. Backends are expected to provide their own testing to
// validate correctness.
//
// The intent of this module for simplifying bringup or otherwise getting data
// in/out of a CPU in a way that is platform-agnostic. The interface is designed
// to be easy to understand. There's no initialization as part of this
// interface, there's no configuration, and the interface is no-frills WYSIWYG
// byte-by-byte i/o.
//
//
//          PLEASE DON'T BUILD PROJECTS ON TOP OF THIS INTERFACE.

#include <cstddef>
#include <cstring>

#include "pw_span/span.h"
#include "pw_status/status.h"
#include "pw_status/status_with_size.h"

namespace pw::dumb_io {

// Read a single byte from the dumb io backend.
// Implemented by: Backend
//
// This function will block until it either succeeds or fails to read a byte
// from the pw_dumb_io backend.
//
// Returns Status::OK if a byte was successfully read.
Status GetByte(std::byte* dest);

// Write a single byte out the dumb io backend.
// Implemented by: Backend
//
// This function will block until it either succeeds or fails to write a byte
// out the pw_dumb_io backend.
//
// Returns Status::OK if a byte was successfully read.
Status PutByte(std::byte b);

// Fill a byte span from the dumb io backend using GetByte().
// Implemented by: Facade
//
// This function is implemented by this facade and simply uses GetByte() to read
// enough bytes to fill the destination span. If there's an error reading a
// byte, the read is aborted and the contents of the destination span are
// undefined. This function blocks until either an error occurs, or all bytes
// are successfully read from the backend's GetByte() implementation.
//
// Return status is Status::OK if the destination span was successfully filled.
// In all cases, the number of bytes successuflly read to the destination span
// are returned as part of the StatusWithSize.
StatusWithSize GetBytes(span<std::byte> dest);

// Write span of bytes out the dumb io backend using PutByte().
// Implemented by: Facade
//
// This function is implemented by this facade and simply writes the source
// contents using PutByte(). If an error writing a byte is encountered, the
// write is aborted and the error status returned. This function blocks until
// either an error occurs, or all bytes are successfully read from the backend's
// PutByte() implementation.
//
// Return status is Status::OK if all the bytes from the source span were
// successfully written. In all cases, the number of bytes successfully written
// are returned as part of the StatusWithSize.
StatusWithSize PutBytes(span<const std::byte> src);

}  // namespace pw::dumb_io