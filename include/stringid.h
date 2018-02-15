/**
 * @file   stringid.h
 *
 * @section LICENSE
 *
 * The MIT License
 *
 * @copyright Copyright (c) 2017 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#pragma once

#include <string>
#include <stdint.h>

namespace Jarvis {
    class StringID {
        uint16_t _id;
        static bool get(const char *, StringID &stringid, bool add);
    public:
        static bool lookup(const char *name, StringID &stringid)
            { return get(name, stringid, false); }
        StringID(const char *name = 0) { (void)get(name, *this, true); }
        StringID(int id) : _id(id) { }
        bool operator<(const StringID &a) const { return _id < a._id; }
        bool operator==(const StringID &a) const { return _id == a._id; }
        bool operator!=(const StringID &a) const { return _id != a._id; }
        std::string name() const;
        // TODO For debugging only
        uint16_t id() const { return _id; }
    };
};
