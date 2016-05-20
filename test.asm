	JUMP _pop_start_
_pop_start_:
	OPEN_SCOPE
	PUSH_INT 1
	BIND x
	PUSH_FLOAT 1.1
	BIND y
	PUSH_SYMBOL x
	PUSH_SYMBOL y
	GT
	JUMP_FALSE _pop_0_else_
	PUSH_SYMBOL x
	PUSH_INT 1
	PUSH_SYMBOL print
	CALL
	JUMP _pop_0_endif_
_pop_0_else_:
	PUSH_SYMBOL y
	PUSH_INT 1
	PUSH_SYMBOL print
	CALL
_pop_0_endif_:
	CLOSE_SCOPE
	HALT
