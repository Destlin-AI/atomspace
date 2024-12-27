/*
 * ConcatenateLink.cc
 *
 * Copyright (C) 2015, 2022 Linas Vepstas
 *
 * Author: Linas Vepstas <linasvepstas@gmail.com>  January 2009
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the
 * exceptions at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <opencog/atomspace/AtomSpace.h>
#include <opencog/atoms/value/LinkValue.h>

#include "ConcatenateLink.h"

using namespace opencog;

ConcatenateLink::ConcatenateLink(const HandleSeq&& oset, Type t)
	: CollectionOfLink(std::move(oset), t)
{
	if (not nameserver().isA(t, CONCATENATE_LINK))
	{
		const std::string& tname = nameserver().getTypeName(t);
		throw InvalidParamException(TRACE_INFO,
			"Expecting an ConcatenateLink, got %s", tname.c_str());
	}

	_out_type = LINK_VALUE;
}

// ---------------------------------------------------------------

/// Return a concatenation of lists.
ValuePtr ConcatenateLink::flatten(AtomSpace* as, const Handle& base)
{
	// We expect a link. We can throw, or we can be silent!?
	// I dunno. Flip a coin.
	if (not base->is_link()) return base;
	HandleSeq oset;
	for (const Handle& oli : base->getOutgoingSet())
	{
		if (oli->is_link())
		{
			const HandleSeq& los = oli->getOutgoingSet();
			oset.insert(oset.end(), los.begin(), los.end());
		}
		else
			oset.push_back(oli);
	}
	if (not _have_typespec) _out_type = base->get_type();
	return as->add_link(_out_type, std::move(oset));
}

// ---------------------------------------------------------------

/// Return a concatenation of lists.
ValuePtr ConcatenateLink::execute(AtomSpace* as, bool silent)
{
	int coff = 0;
	if (_have_typespec) coff = 1;

	// If the given Atom is not executable, we deal with a the
	// simplest case of just collapsing a plain link.
	Handle base(_outgoing[coff]);
	if (not base->is_executable())
		return flatten(as, base);

	// Same as above, but this time, we execute, and branch
	// over all the various possibilities.
	ValuePtr vp = base->execute(as, silent);
	if (vp->is_atom())
		return flatten(as, HandleCast(vp));

	if (not vp->is_type(LINK_VALUE))
		throw InvalidParamException(TRACE_INFO,
			"ConcatenateLink expects a LinkValue, got %s",
			vp->to_string().c_str());

	LinkValuePtr lvp = LinkValueCast(vp);
	HandleSeq hs = lvp->to_handle_seq();
	return as->add_link(_out_type, std::move(hs));
}

DEFINE_LINK_FACTORY(ConcatenateLink, CONCATENATE_LINK)

/* ===================== END OF FILE ===================== */
