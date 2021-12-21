; n = 7
; # terms = 169
((lambda (f1) (f1 #t)) (lambda (x1) ((lambda (f2) (f2 #t)) (lambda (x2) ((lambda (f3) (f3 #f)) (lambda (x3) ((lambda (f4) (f4 #t)) (lambda (x4) ((lambda (f5) (f5 #t)) (lambda (x5) ((lambda (f6) (f6 #t)) (lambda (x6) ((lambda (f7) (f7 #t)) (lambda (x7) ((lambda (z) (z x1 x2 x3 x4 x5 x6 x7)) (lambda (y1 y2 y3 y4 y5 y6 y7) y1))))))))))))))))
