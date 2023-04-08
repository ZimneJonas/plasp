#const horizon=1.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Horizon, must be defined externally
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

time(0..horizon).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Establish initial state
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

initialState(X, value(X, false)) :- variable(X), not initialState(X, value(X, true)).
holds(Variable, Value, 0) :- initialState(Variable, Value).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Compute derived predicates
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% conjunctive preconditions
satisfied(DerivedPredicate, type(and), T) :- derivedPredicate(DerivedPredicate, type(and)), holds(Variable, Value, T) : precondition(DerivedPredicate, type(and), Variable, Value); time(T).

% disjunctive preconditions
satisfied(DerivedPredicate, type(or), T) :- precondition(DerivedPredicate, type(or), Variable, Value), holds(Variable, Value, T), time(T).

holds(DerivedVariable, Value, T) :- satisfied(DerivedPredicate, Type, T), postcondition(DerivedPredicate, Type, effect(unconditional), DerivedVariable, Value).

holds(derivedVariable(DerivedVariable), value(DerivedVariable, false), T) :- derivedVariable(DerivedVariable), not holds(derivedVariable(DerivedVariable), value(DerivedVariable, true), T), time(T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Perform actions
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

1 {occurs(Action, T) : action(Action)} 1 :- time(T), T > 0.

% Check preconditions
:- occurs(Action, T), precondition(Action, Variable, Value), not holds(Variable, Value, T - 1).

% Apply effects
caused(Variable, Value, T) :-
	occurs(Action, T),
	postcondition(Action, Effect, Variable, Value),
	holds(VariablePre, ValuePre, T - 1) : precondition(Effect, VariablePre, ValuePre).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Compute numerical rules
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Check preconditions 			(plasp translates all comparisons to lessThen equation X>Y -> Y<X // X>=Y -> Y<X+1)
:- occurs(Action, T), precondition(Action, lessThan(numericVariable(V1), int(Y))), 		 		not X<Y, holds(numericVariable(V1), value(numericVariable(V1), X), T-1).
:- occurs(Action, T), precondition(Action, lessThan(int(X), numericVariable(V2))), 		 		not X<Y, holds(numericVariable(V2), value(numericVariable(V2), Y), T-1).
:- occurs(Action, T), precondition(Action, lessThan(numericVariable(V1),numericVariable(V2))), 	not X<Y, holds(numericVariable(V1), value(numericVariable(V1), X), T-1), 
																										 holds(numericVariable(V2), value(numericVariable(V2), Y), T-1).

% numerical operators 			#TODO can't handle multiple operations in one step on same variable
	% increase
holds(numericVariable(V), value(numericVariable(V), Value+1), T) :- caused(numericVariable(V), value(numericVariable(V), increase) , T), holds(numericVariable(V), value(numericVariable(V), Value), T-1).
	% add
holds(numericVariable(V1), value(numericVariable(V1), X+Y), T)
	:- caused(numericVariable(V1), value(numericVariable(V1), add(int(Y))), T), 				holds(numericVariable(V1), value(numericVariable(V1), X), T-1).	
	:- caused(numericVariable(V1), value(numericVariable(V1), add(numericVariable(V2))), T),	holds(numericVariable(V1), value(numericVariable(V1), X), T-1), 
																								holds(numericVariable(V2), value(numericVariable(V2), Y), T-1).
	
	% multiply
hold(numericVariable(V1), value(numericVariable(V1), X*Y), T) 	
	:- caused(numericVariable(V1), value(numericVariable(V1), multiply(int(Y))), T), 			holds(numericVariable(V1), value(numericVariable(V1), X), T-1).
	:- caused(numericVariable(V1), value(numericVariable(V1), multiply(numericVariable(V2))),T),holds(numericVariable(V1), value(numericVariable(V1), X), T-1), 
																								holds(numericVariable(V2), value(numericVariable(V2), Y), T-1).
	

% inertia
holds(numericVariable(V), Value, T) :- holds(numericVariable(V), Value, T - 1), not modified(numericVariable(V), T), time(T).

% goal
:- goal(lessThan(numericVariable(V1), int(Y))), 			 not X<Y, 	holds(numericVariable(V1), value(numericVariable(V1), X), T).
:- goal(lessThan(int(X), numericVariable(V2))), 			 not X<Y, 	holds(numericVariable(V2), value(numericVariable(V2), Y), T).
:- goal(lessThan(numericVariable(V1), numericVariable(V2))), not X<Y, 	holds(numericVariable(V1), value(numericVariable(V1), X), T), 
																		holds(numericVariable(V2), value(numericVariable(V2), Y), T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Inertia rules
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

modified(Variable, T) :- caused(Variable, Value, T).

holds(variable(V), Value, T) :- caused(variable(V), Value, T).
holds(variable(V), Value, T) :- holds(variable(V), Value, T - 1), not modified(variable(V), T), time(T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Variables and mutex groups
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Check that variables have unique values
:- variable(Variable), not 1 {holds(Variable, Value, T) : contains(Variable, Value)} 1, time(T).

% Check mutexes
:- mutexGroup(MutexGroup), not {holds(Variable, Value, T) : contains(MutexGroup, Variable, Value)} 1, time(T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Verify that goal is met
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:- goal(Variable, Value), not holds(Variable, Value, horizon).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#show occurs/2.
