;
; Example chemistry app, in scheme
;
(use-modules (opencog))
(use-modules (opencog demo-types))

(define dihydrogen-monoxide
	(Molecule
		(SingleBond
			(O "big man oxygen")
			(H "one proton"))
		(SingleBond
			(O "big man oxygen")
			(H "another proton"))
	))

(format #t "Look ma! See what I made! This: ~A\n" dihydrogen-monoxide)

; The end!
; That's all folks!
