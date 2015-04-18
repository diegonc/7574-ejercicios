#ifndef OP_PERSONA_H
#define OP_PERSONA_H

const long PERSONA_CAUSA_NOTIF_ENTRADA = 1;
const long PERSONA_CAUSA_NOTIF_LLENO   = 2;
const long PERSONA_CAUSA_NOTIF_CERRADO = 3;
const long PERSONA_CAUSA_CONF_SALIDA   = 4;

struct OpPersona {
	/* tipo de mensaje enviado a la persona
	 *   en este campo se envía el PID del
	 *   proceso persona al que se dirige
	 *   el mensaje.
	 */
	long mtype;
	/* causa del mensaje */
	long causa;
};

#endif /* OP_PERSONA_H */
