#define QUO(name) #name

#define jmp_helper(COND, NAME, FLAG) \
	QUO(define) CONDITION COND \
	QUO(define) CNAME NAME \
	QUO(define) CONDITION_FLAG FLAG \
	CONDITION \
	CNAME \
	CONDITION_FLAG \
	QUO(define) CONDITION_FLAG \
	QUO(define) CNAME \
	QUO(define) CONDITION


#define x xxx

jmp_helper(mp, "mp", true)

x
