/*
 * A quick implemetation of spin lock
 * Still WIP
 * A1phaZer0 @ sec-9.io
 */

typedef struct {
	char *name;
	int in_use;
} spinlock;

extern void init_spinlock(spinlock *, char *);
extern void aquire_spinlock(spinlock *);
extern void release_spinlock(spinlock *);
