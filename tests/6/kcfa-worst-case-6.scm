; n = 6
; # terms = 147
((lambda (f1) (f1 #f)) (lambda (x1) ((lambda (f2) (f2 #f)) (lambda (x2) ((lambda (f3) (f3 #f)) (lambda (x3) ((lambda (f4) (f4 #f)) (lambda (x4) ((lambda (f5) (f5 #f)) (lambda (x5) ((lambda (f6) (f6 #t)) (lambda (x6) ((lambda (z) (z x1 x2 x3 x4 x5 x6)) (lambda (y1 y2 y3 y4 y5 y6) y1))))))))))))))
