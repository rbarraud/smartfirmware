/*  Copyright (c) 1992-2005 CodeGen, Inc.  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Redistributions in any form must be accompanied by information on
 *     how to obtain complete source code for the CodeGen software and any
 *     accompanying software that uses the CodeGen software.  The source code
 *     must either be included in the distribution or be available for no
 *     more than the cost of distribution plus a nominal fee, and must be
 *     freely redistributable under reasonable conditions.  For an
 *     executable file, complete source code means the source code for all
 *     modules it contains.  It does not include source code for modules or
 *     files that typically accompany the major components of the operating
 *     system on which the executable file runs.  It does not include
 *     source code generated as output by a CodeGen compiler.
 *
 *  THIS SOFTWARE IS PROVIDED BY CODEGEN AS IS AND ANY EXPRESS OR IMPLIED
 *  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  (Commercial source/binary licensing and support is also available.
 *   Please contact CodeGen for details. http://www.codegen.com/)
 */


#include <stdlib.h>
#include <stdlibx.h>
#include <string.h>
#include <stringx.h>
#include <stdio.h>
#include <ctype.h>
#include <pool.h>

class _StringPoolElem
{
public:
    char const *_p;

    _StringPoolElem() { _p = NULL; }
    _StringPoolElem(_StringPoolElem const &e) { _p = strdup(e._p); }
    ~_StringPoolElem() { strfree(_p); }
    void operator=(_StringPoolElem const &e)
	    { strfree(_p); _p = strdup(e._p); }
    boolean operator==(_StringPoolElem const &e) { return streq(_p, e._p); }
};

static inline unsigned long
strhash(_StringPoolElem const &e)
{
    return strhash(e._p);
}

declare_pool(_StringPool, _StringPoolElem)
implement_pool(_StringPool, _StringPoolElem, strhash)


static _StringPool *_strpool = NULL;
static _StringPoolElem e;

char const *
strget(char const *s)
{
    if (_strpool == NULL)
	_strpool = new _StringPool;

    e._p = s;
    _StringPoolElem &e2 = _strpool->get(e);
    e._p = NULL;
    return e2._p;
}

void
strput(char const *s)
{
    if (_strpool == NULL)
    	return;

    e._p = s;
    _strpool->put(e);
    e._p = NULL;
}

char const *
strgeti(char const *s)
{
    char *str = strnew(strlen(s));
    char *p = str;

    while (*s != '\0')
	*p++ = tolower(*s++);

    *p = '\0';
    const char *ret = strget(str);
    strfree(str);
    return ret;
}

void
strputi(char const *s)
{
    char *str = strnew(strlen(s));
    char *p = str;

    while (*s != '\0')
	*p++ = tolower(*s++);

    *p = '\0';
    strput(str);
    strfree(str);
}

void
clearstringpool()
{
    if (_strpool == NULL)
    	return;

    _strpool->clear();
}
