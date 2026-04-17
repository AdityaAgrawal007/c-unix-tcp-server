# TCP Client-Server with POSIX Signal Handling

Most TCP server implementations you'll find online ignore the zombie process
problem entirely. This one doesn't.

This is a concurrent TCP server written in C that forks a child process per
client connection and correctly reaps terminated children using a POSIX-compliant
`SIGCHLD` handler. The server exposes an addition service: clients send two
integers, the server adds them and writes back the result.

## What's actually happening

The server uses the classic Unix concurrent model: `accept` in the parent,
`fork` per connection, service logic in the child. The parent closes the
connected socket immediately after forking. The child closes the listening
socket. This is not optional hygiene — it's how reference counting on file
descriptors works. If you skip either close, you leak descriptors or keep
connections alive past their lifetime.

The signal handler uses `waitpid` with `WNOHANG` in a loop, not a single
`wait`. A single `wait` misses children that terminate while the handler is
already running. The loop drains all pending `SIGCHLD` signals before
returning. `errno` is saved and restored so interrupted syscalls in the parent
don't get corrupted errno values.

`Signal()` wraps `sigaction` instead of the legacy `signal()` call. On Linux,
`signal()` behavior is implementation-defined. `sigaction` is portable and
explicit. For everything except `SIGALRM`, `SA_RESTART` is set so slow system
calls like `accept` automatically restart after the signal handler returns
instead of failing with `EINTR`. `SIGALRM` deliberately does not set
`SA_RESTART` because alarm-based timeouts need `EINTR` to propagate.

The `str_add` server-side function reads a line, parses two `long` integers
with `sscanf`, and writes back the sum. It handles malformed input explicitly
rather than silently failing.

## Build

```bash
make
```

Produces `bin/tcp_server` and `bin/client`. Requires GCC and a POSIX-compliant
system (Linux, macOS).

Requires GCC. Tested on Linux.
 
## Run
 
```bash
# terminal 1
./bin/tcp_server
 
# terminal 2
./bin/client 127.0.0.1
```
 
Type two integers:
 
```
3 7
Server replied: 10
```
 
## Structure
 
```
.
├── include/
│   └── utils.h
└── src/
    ├── tcp_server.c
    ├── client.c
    ├── str_cli.c
    ├── str_add.c
    ├── signal.c
    └── SIGCHLD_handler.c
```
