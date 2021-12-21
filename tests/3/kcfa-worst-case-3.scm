; n = 3
; # terms = 81
((lambda (f1) (f1 #t)) (lambda (x1) ((lambda (f2) (f2 #t)) (lambda (x2) ((lambda (f3) (f3 #t)) (lambda (x3) ((lambda (z) (z x1 x2 x3)) (lambda (y1 y2 y3) y1))))))))
