(if (> 5 4)
	(and #t #t)
	(- 0 -42)
)
(if (= 24 42)
	(+ 2 3)
	(or #t #f)
)
(if (= 0 0)
	(if (= 0 1)
		(+ 2 3)
		42
	)
	(or #t #f)
)
