(set-logic QF_LRA)
(declare-fun v1 () Real)
(assert (<= v1 (+ v1 (* (- 1) 3))))
(check-sat)
(exit)
