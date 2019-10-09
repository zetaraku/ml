(let
	(
		(sq
			(lambda (x)
				(* x x)
			)
		)
	)
	(sq 7)	; 49
)
(let
	(
		(a 2)
		(b 3)
	)
	(let
		(
			(c 4)
			(d 5)
		)
		(+ a b c d)	; 14
	)
)
(let
	(
		(fact
			(lambda (n)
				(if (> n 1) (* n (fact (- n 1))) 1)
			)
		)
	)
	(fact 5)	; 120
)
(let
	(
		(plusn
			(lambda (n)
				(lambda (x)
					(+ n x)
				)
			)
		)
	)
	(let
		(
			(plus5
				(plusn 5)
			)
		)
		(plus5 37)	; 42
	)
)
(
	(
		(lambda (f)
			(
				(lambda (self)
					(lambda (x)
						((f (self self)) x)
					)
				)
				(lambda (self)
					(lambda (x)
						((f (self self)) x)
					)
				)
			)
		)
		(lambda (fact)
			(lambda (n)
				(if (< n 1)
					1
					(* n (fact (- n 1)))
				)
			)
		)
	)
	5
)	; 120
(
	(
		(lambda (fn)
			(fn fn)
		) (lambda (f)
			(lambda (n)
				(if (= n 0)
					1
					(* n ((f f) (- n 1)))
				)
			)
		)
	)
	5
)	; 120
