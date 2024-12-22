;
; string-of-test.scm -- Test the StringOfLink for basic fuction
;
(use-modules (opencog) (opencog exec))
(use-modules (opencog test-runner))

(opencog-test-runner)
(define tname "string-of-test")
(test-begin tname)
; -----------

(define node-from-node
	(cog-execute! (StringOf (Type 'Concept) (Predicate "bar"))))
; (format #t "Node from node got ~A\n" node-to-node)

(test-assert "Node-from-node"
	(equal? node-from-node (Concept "bar")))

; -----------

(cog-set-value! (Anchor "anch") (Predicate "key")
	(StringValue "a" "b" "c"))

(define node-from-string
	(cog-execute! (StringOf (Type 'Concept)
		(ValueOf (Anchor "anch") (Predicate "key")))))

(format #t "Got node from string ~A\n" node-from-string)

(test-assert "Node-from-string"
	(equal? node-from-string (Concept "a")))

; -----------

(cog-execute!
	(SetValue (Anchor "anch") (Predicate "strkey")
		(StringOf (Type 'StringValue)
			(Concept "do-da"))))

(define string-from-node
	(cog-value (Anchor "anch") (Predicate "strkey")))
(format #t "Got string from node ~A\n" string-from-node)

;(test-assert "string-from-node"
;	(equal? string-from-node (Concept "a")))

; -----------
(test-end tname)
(opencog-test-end)
