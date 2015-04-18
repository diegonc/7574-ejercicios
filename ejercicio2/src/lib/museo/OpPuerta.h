#ifndef OP_PUERTA_H
#define OP_PUERTA_H

enum {
	PUERTA_CAUSA_CIERRE  = 1,
	PUERTA_CAUSA_ENTRADA = 2,
	PUERTA_CAUSA_SALIDA  = 3
};

struct OpPuerta {
	/* tipo de mensaje
	 *   indica la puerta que recibe el mensaje
	 *   mediante su identificador
	 */
	long mtype;
	/* causa del mensaje
	 *   indica el tipo de procesamiento que el
	 *   proceso puerta debe realizar
	 */
	long causa;
	/* identificación de la persona.
	 * utilizado solo por las causas:
	 *    - PUERTA_CAUSA_ENTRADA
	 *    - PUERTA_CAUSA_SALIDA
	 */
	long persona;
};

#endif /* OP_PUERTA_H */
