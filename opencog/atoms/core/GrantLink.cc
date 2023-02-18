/*
 * GrantLink.cc
 *
 * Copyright (C) 2015,2023 Linas Vepstas
 *
 * Author: Linas Vepstas <linasvepstas@gmail.com>  May 2015
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

#include <opencog/atoms/base/ClassServer.h>

#include "GrantLink.h"

using namespace opencog;

void GrantLink::init(void)
{
	if (not nameserver().isA(get_type(), GRANT_LINK))
		throw SyntaxException(TRACE_INFO,
			"Expecting a GrantLink, got %s",
				nameserver().getTypeName(get_type()).c_str());

	// Must have name and body
	if (2 != _outgoing.size())
		throw SyntaxException(TRACE_INFO,
			"Expecting name and definition, got size %d", _outgoing.size());

	// Perform some additional checks in the UniqueLink init method
	UniqueLink::init();
}

GrantLink::GrantLink(const HandleSeq&& oset, Type t)
	: UniqueLink(std::move(oset), t)
{
	init();
}

GrantLink::GrantLink(const Handle& name, const Handle& defn)
	: UniqueLink(HandleSeq({name, defn}), GRANT_LINK)
{
	init();
}

/// Content-based comparison. Due to the uniqueness constraint,
/// Two GrantLinks are "identical" if and only if the first outgoing
/// Atom is the same. The second one does not affect results.
bool GrantLink::operator==(const Atom& other) const
{
	// If other points to this, then have equality.
	if (this == &other) return true;

	// Rule out obvious mis-matches, based on the hash.
	if (get_hash() != other.get_hash()) return false;
	if (get_type() != other.get_type()) return false;

	// Perform a content-compare on the first Atom in the outgoing set.
	const Handle& rhs = other.getOutgoingAtom(0);
	if (*((AtomPtr)_outgoing[0]) != *((AtomPtr)rhs))
		return false;

	return true;
}

/// We hash only the first Atom in the OutgoingSet; it is the one
/// that dtermines uniqueness. The second Atom does not matter.
ContentHash GrantLink::compute_hash() const
{
   // The nameserver().getTypeHash() returns hash of the type name
	// string, and is thus independent of all other type declarations.
	// 1<<44 - 377 is prime
	ContentHash hsh = ((1ULL<<44) - 377) * nameserver().getTypeHash(get_type());

	const Handle& h(_outgoing[0]);

	hsh += (hsh <<5) ^ (353 * h->get_hash()); // recursive!

	// Bit-mixing copied from murmur64. Yes, this is needed.
	hsh ^= hsh >> 33;
	hsh *= 0xff51afd7ed558ccdL;
	hsh ^= hsh >> 33;
	hsh *= 0xc4ceb9fe1a85ec53L;
	hsh ^= hsh >> 33;

	// Links will always have the MSB set.
	ContentHash mask = ((ContentHash) 1ULL) << (8*sizeof(ContentHash) - 1);
	hsh |= mask;

	if (Handle::INVALID_HASH == hsh) hsh -= 1;
	return hsh;
}

/**
 * Get the definition associated with the alias.
 * This will be the second atom of some GrantLink, where
 * `alias` is the first.
 */
Handle GrantLink::get_definition(const Handle& alias, const AtomSpace* as)
{
	Handle uniq(get_unique(alias, GRANT_LINK, false, as));
	return uniq->getOutgoingAtom(1);
}

Handle GrantLink::get_link(const Handle& alias, const AtomSpace* as)
{
	return get_unique(alias, GRANT_LINK, false, as);
}

DEFINE_LINK_FACTORY(GrantLink, GRANT_LINK)

/* ===================== END OF FILE ===================== */
