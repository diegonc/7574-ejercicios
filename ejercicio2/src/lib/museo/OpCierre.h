#ifndef OP_CIERRE_H
#define OP_CIERRE_H

struct OpCierre {
	/* tipo de mensaje
	 *   identifica a la persona a la que se envía
	 *   la notificación de cierre del museo mediante
	 *   el PID del proceso que la representa.
	 */
	long mtype;
};

#endif /* OP_CIERRE_H */
