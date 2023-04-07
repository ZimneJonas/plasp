
(define (domain hello-deaf)

(:requirements :strips :negative-preconditions :numeric-fluents)

(:predicates
        (can_hear)
)

(:functions
    (hello_counter)
)


(:action say-hello
     :parameters ()
     :precondition (not (can_hear))
     :effect (and (can_hear))
)
)

(define (problem hello-3-times) 
(:domain hello-deaf)

(:init
    (not (can_hear))
)
(:goal 
        (can_hear)
)
)