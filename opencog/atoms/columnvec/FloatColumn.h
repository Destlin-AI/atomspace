/*
 * opencog/atoms/column/FloatColumn.h
 *
 * Copyright (C) 2015, 2022, 2025 Linas Vepstas
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _OPENCOG_FLOAT_COLUMN_H
#define _OPENCOG_FLOAT_COLUMN_H

#include <opencog/atoms/base/Link.h>

namespace opencog
{
/** \addtogroup grp_atomspace
 *  @{
 */

/// The FloatColumn returns a StringValue vector of the s-expressions
/// for Atoms/Values contiained in the given Link or LinkValue.
///
/// For example,
///
///     FloatColumn
///         Link
///             Concept "foo"
///             Concept "bar"
///
/// will return a string vector of length two:
///
///     (StringValue "(Concept \"foo\")" "(Concept \"bar\")")
///
class FloatColumn : public Link
{
protected:
	ValuePtr do_execute(AtomSpace*, bool);

public:
	FloatColumn(const HandleSeq&&, Type = FLOAT_COLUMN);
	FloatColumn(const FloatColumn&) = delete;
	FloatColumn& operator=(const FloatColumn&) = delete;

	virtual bool is_executable() const { return true; }

	// Return a pointer to StringValue holding the s-expressions
	virtual ValuePtr execute(AtomSpace*, bool);

	static Handle factory(const Handle&);
};

LINK_PTR_DECL(FloatColumn)
#define createFloatColumn CREATE_DECL(FloatColumn)

/** @}*/
}

#endif // _OPENCOG_FLOAT_COLUMN_H
