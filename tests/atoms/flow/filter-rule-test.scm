;
; filter-rule-test.scm -- Test of filters with rules.
;
(use-modules (opencog) (opencog exec))
(use-modules (opencog test-runner))

(opencog-test-runner)
(define tname "filter-rule-test")
(test-begin tname)

; -----------
(cog-set-value! (Concept "a") (Predicate "k")
	(LinkValue (Concept "baz") (Concept "ni") (Concept "goh")))

(define fthree
	(Filter
		(LinkSignature (Type 'LinkValue)
			(Variable "$from") (Variable "$to") (Variable "$msg"))
		(LinkSignature (Type 'LinkValue)
			(ValueOf (Concept "a") (Predicate "k")))))

(define e-fthree (cog-execute! fthree))
(test-assert "filter three"
	(equal? e-fthree (LinkValue
		(LinkValue (Concept "baz") (Concept "ni") (Concept "goh")))))

; -----------

(test-end tname)

(opencog-test-end)
