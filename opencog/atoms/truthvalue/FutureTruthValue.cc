/*
 * opencog/atoms/truthvalue/FutureTruthValue.cc
 *
 * Copyright (C) 2020 Linas Vepstas
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
 *
 */

#include <opencog/util/platform.h>
#include <opencog/util/exceptions.h>

#include <opencog/atoms/execution/EvaluationLink.h>
#include <opencog/atoms/value/ValueFactory.h>
#include "FutureTruthValue.h"

using namespace opencog;

FutureTruthValue::FutureTruthValue(const Handle& h)
	: SimpleTruthValue(0, 0), _future(h), _as(h->getAtomSpace())
{
	_type = FUTURE_TRUTH_VALUE;
	update();
}

FutureTruthValue::FutureTruthValue(const HandleSeq&& oset)
	: SimpleTruthValue(0, 0)
{
	if (1 != oset.size())
		throw SyntaxException(TRACE_INFO,
			"Expecting exactly one Atom!");

	_type = FUTURE_TRUTH_VALUE;
	_future = oset[0];
	_as = _future->getAtomSpace();
	update();
}

FutureTruthValue::~FutureTruthValue()
{}

void FutureTruthValue::update(void) const
{
	// The wrapped predicate, when evaluated, should produce
	// two numbers, the strength and the confidence.
	if (_future->get_type() == FORMULA_PREDICATE_LINK)
	{
		TruthValuePtr tvp = EvaluationLink::do_evaluate(_as, _future);
		_value = tvp->value();
	}
	else if (_future->is_evaluatable())
	{
		TruthValuePtr tvp = _future->evaluate(_as);
		_value = tvp->value();
	}
	else if (_future->is_executable())
	{
		ValuePtr vp = _future->execute(_as);
		if (not nameserver().isA(vp->get_type(), FLOAT_VALUE))
			throw SyntaxException(TRACE_INFO,
				"Expecting FloatValue, got %s",
					vp->to_string().c_str());
		_value = FloatValueCast(vp)->value();
	}
	else
	{
		TruthValuePtr tvp = _future->getTruthValue();
		_value = tvp->value();
	}
}

strength_t FutureTruthValue::get_mean() const
{
	update();
	return _value[MEAN];
}

std::string FutureTruthValue::to_string(const std::string& indent) const
{
	update();
	std::string rv = indent + "(FutureTruthValue\n";
		rv += _future->to_short_string(indent + "   ") + "\n";
	rv += indent + "   ; Current sample:\n";
	rv += indent + "   ; " + SimpleTruthValue::to_string() + "\n)";
	return rv;
}

bool FutureTruthValue::operator==(const Value& rhs) const
{
	if (FUTURE_TRUTH_VALUE != rhs.get_type()) return false;

	const FutureTruthValue *ftv = dynamic_cast<const FutureTruthValue *>(&rhs);
	return ftv->_future == _future;
}

DEFINE_VALUE_FACTORY(FUTURE_TRUTH_VALUE,
	createFutureTruthValue, const Handle&)
DEFINE_VALUE_FACTORY(FUTURE_TRUTH_VALUE,
	createFutureTruthValue, const HandleSeq&&)
