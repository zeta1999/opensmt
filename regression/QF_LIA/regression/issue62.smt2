(set-logic QF_LIA)
(declare-fun i2 () Int)
(assert (= 8 i2))
(assert (= 0 i2))
(check-sat)
