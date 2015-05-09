#include <errno.h>
#include <fcntl.h>
#include <logging/Logger.h>
#include <logging/LoggerRegistry.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <system/System.h>
#include <unistd.h>

SystemErrorException::SystemErrorException () throw ()
	: _number (errno)
{
}

SystemErrorException::SystemErrorException (int err) throw ()
	: _number (err)
{
}

pid_t System::spawn (const char *file, char* const argv[])
{
	// Se utiliza un pipe para determinar en el padre si exec fallo o no
	// en el hijo.
	// ver: http://davmac.wordpress.com/2008/11/25/forkexec-is-forked-up/
	int fds[2];
	pid_t pid;
	LevelId level = LoggerRegistry::getInstance ().defaultLevel ();
	const char* filename = LoggerRegistry::getInstance ().filename ().c_str ();

	if (pipe (fds) == -1) {
		return -1;
	}

	pid = fork ();
	if (pid == 0) {
		close (fds[0]);
		// Se establece el flag FD_CLOEXEC para que si exec se ejecuta
		// exitosamente el descriptor se cierre.
		// Esto provoca que en el padre read devuelva 0-
		fcntl (fds[1], F_SETFD, fcntl (fds[1], F_GETFD) | FD_CLOEXEC);
		int err = execvp (file, argv);
		(void) err;
		// Si llega hasta acá es que hubo error en exec
		// Primero logeamos el error 
		SystemErrorException e;
		LoggerRegistry& r = LoggerRegistry::getInstance ();
		r.filename (filename);
		r.application ("spawned");
		r.defaultLevel (level);
		Logger& logger = LoggerRegistry::getLogger ("System");
		logger << "Error ejecutando " << file << " : (" << e.number () << ") "
				<< e.what () << Logger::endl;

		// Luego notificamos al padre el código de error a traves
		// del pipe.
		int reason = e.number ();
		ssize_t written = write (fds[1], &reason, sizeof (reason));
		if (written != sizeof reason) {
			logger << Level::ERROR
				<< "No se pudo comunicar el código de error al padre."
				<< "write devolvió: " << written
				<< Logger::endl;
		}
		close (fds[1]);

		// Finalmente salimos del proceso hijo
		_exit (1);
	} else if (pid == -1) {
		int forkError = errno;
		close (fds[0]);
		close (fds[1]);
		errno = forkError;
		return -1;
	} else {
		close (fds[1]);
		int childErr = 0;
		// El padre se bloquea hasta que el hijo haga exec
		ssize_t err = read (fds[0], &childErr, sizeof (childErr));
		close (fds[0]);
		// Si err es 0 el exec del hijo fue exitoso. Sino el valor
		// de errno que nos comunicó el hijo queda en la variable
		// childErr.
		if (err > 0) {
			errno = childErr;
			return -1;
		}
	}
	return pid;
}

pid_t System::spawn (const char* file, std::vector<const char*>& args)
{
	Logger& logger = LoggerRegistry::getLogger ("System::spawn");
	logger << Level::DEBUG
		<< "Lanzando " << file << ". Args: ";

	logger << args[0];
	for (size_t i=1; i < args.size (); i++)
		if (args[i] != NULL)
			logger << " " << args[i];
	logger << Logger::endl;

	return spawn (file, (char * const *) &args[0]);
}

void System::semrm (const IPCName& name)
{
	Logger& logger = LoggerRegistry::getLogger ("System");
	logger << "Eliminando semáforo ["
			<< name.path << "::" << name.index << "]" << Logger::endl;

	key_t token = ftok (name.path.c_str (), name.index);
	logger << "ftok devolvió " << token << Logger::endl;
	System::check (token);

	int id = semget (token, 0, 0);
	logger << "semget devolvió " << id << Logger::endl;
	System::check (id);

	int err = semctl (id, 0, IPC_RMID);
	logger << "semctl devolvió " << err << Logger::endl;
	System::check (err);
}

void System::shmrm (const IPCName& name)
{
	Logger& logger = LoggerRegistry::getLogger ("System");
	logger << "Eliminando memoria compartida ["
			<< name.path << "::" << name.index << "]" << Logger::endl;

	key_t token = ftok (name.path.c_str (), name.index);
	logger << Level::DEBUG
			<< "ftok devolvió " << token << Logger::endl;
	System::check (token);

	int id = shmget (token, 0, 0);
	logger << Level::DEBUG
			<< "shmget devolvió " << id << Logger::endl;
	System::check (id);

	int err = shmctl (id, IPC_RMID, NULL);
	logger << Level::DEBUG
			<< "shmctl devolvió " << err << Logger::endl;
	System::check (err);
}

void System::mqrm (const IPCName& name)
{
	Logger& logger = LoggerRegistry::getLogger ("System");
	logger << "Eliminando cola ["
			<< name.path << "::" << name.index << "]" << Logger::endl;

	key_t token = ftok (name.path.c_str (), name.index);
	logger << Level::DEBUG
			<< "ftok devolvió " << token << Logger::endl;
	System::check (token);

	int id = msgget (token, 0);
	logger << Level::DEBUG
			<< "msgget devolvió " << id << Logger::endl;
	System::check (id);

	int err = msgctl (id, IPC_RMID, NULL);
	logger << Level::DEBUG
			<< "msgctl devolvió " << err << Logger::endl;
	System::check (err);
}
