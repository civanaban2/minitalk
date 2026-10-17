# Minitalk

A C client/server communication project that transfers text using Unix signals.
The client encodes a message as individual bits, and the server reconstructs
and prints it. Developed as part of the 42 curriculum to practice inter-process
communication, signal handlers, and synchronization.

## Build

Requirements: a C compiler, Make, Git, and a Unix-like environment with POSIX
signals. The project uses the author's `libft` repository as a pinned submodule.

```sh
git clone --recurse-submodules https://github.com/civanaban2/minitalk.git
cd minitalk
make
```

For an existing clone without initialized submodules:

```sh
git submodule update --init --recursive
make
```

Outputs:

- `build/server`
- `build/client`

The Makefile first builds `libft/libft.a`, then links the client and server.
It compiles the project sources with `-Wall -Wextra -Werror`.

## Usage

Start the server in one terminal:

```sh
./build/server
# Server PID: 12345
```

In another terminal, pass the **actual PID printed by your running server**:

```sh
./build/client 12345 "Hello from the client"
# Message sent
```

The server prints the received text followed by a newline. Quote messages
containing spaces. The server stays running for subsequent messages; stop it
with **Ctrl+C**. Use one client at a time and wait for its completion before
starting the next.

The example PID is illustrative, not a fixed value. Do not use zero, negative,
stale, or unrelated process IDs. The current argument checks are incomplete;
use only the PID of the intended server.

## Protocol

```text
Client                           Server
  | --- initial signal --------> |
  | <------ acknowledgment ----- |
  | --- 32 message-length bits ->|
  | <--- acknowledgment per bit -|
  | --- 8 bits per text byte --->|
  | <--- acknowledgment per bit -|
  |                              | reconstruct and print
```

- `SIGUSR1` represents a set bit; `SIGUSR2` represents a cleared bit.
- Length and character bits are sent least-significant bit first.
- `sigaction()` with `SA_SIGINFO` provides the sender PID.
- The length header lets the server allocate its message buffer before payload
  reception.
- The client uses acknowledgments to pace its signal transmission and waits
  with `pause()` between signals.
- Server-side idle handling resets transfer state after approximately one
  second without an interrupting signal.

This is a byte-oriented text protocol, not a socket or network service. Shell
arguments cannot contain embedded NUL bytes, so it is not a general binary-file
transfer interface. UTF-8 text is transmitted as bytes rather than decoded as
Unicode characters by the program.

## Build targets

| Command | Action |
| --- | --- |
| `make` | Build both executables and the libft dependency |
| `make clean` | Remove project objects and run `fclean` in libft |
| `make fclean` | Also remove the `build/` directory |
| `make re` | Clean and rebuild; use without parallel `-j` execution |

`libft` is pinned by the parent repository's gitlink. Initializing the submodule
checks out that recorded revision; it does not automatically select the newest
libft commit. This documentation update does not change that link.

## Structure

```text
Makefile
.gitmodules
inc/minitalk.h       # Shared transfer-state structure
src/client.c        # Handshake, bit encoding and acknowledgments
src/server.c        # Message reconstruction and output
libft/              # Git submodule
```

## Scope and limitations

This is an educational single-client protocol, not a robust IPC library.
Multiple simultaneous clients are not supported. PID validation, signal-call
error handling, empty-message behavior, signal-mask initialization, and handler
async-signal safety need further review. A missing or unresponsive server can
leave the client waiting indefinitely.

The implementation assumes a 32-bit length header and uses platform-specific
integer typedefs. Portability to every POSIX environment is not guaranteed.
These are notes about the existing code, not changes to its behavior.

## License

MIT. See [LICENSE](LICENSE). The libft submodule retains its own repository
history and files.
