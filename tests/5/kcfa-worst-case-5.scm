; n = 5
; # terms = 125
((lambda (f1) (f1 #f)) (lambda (x1) ((lambda (f2) (f2 #t)) (lambda (x2) ((lambda (f3) (f3 #t)) (lambda (x3) ((lambda (f4) (f4 #f)) (lambda (x4) ((lambda (f5) (f5 #t)) (lambda (x5) ((lambda (z) (z x1 x2 x3 x4 x5)) (lambda (y1 y2 y3 y4 y5) y1))))))))))))
