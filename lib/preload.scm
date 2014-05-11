(define (null? x) (eqv? x '()))

(define (not arg)
    (if arg #f #t))

(define (or . args)
    (if (null? args)
        #f
        (if (car args)
            (car args)
            (apply or (cdr args)))))

(define (and . args)
    (if (null? args)
        #t
        (if (car args)
            (apply and (cdr args))
            #f)))
