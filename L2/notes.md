Technical University of Moldova
Faculty of Computers, Informatics and Microelectronics

# OS Lab 2: Meet the OS You Will Build (xv6)

**Student:** Marin, FAF-241
**Checked by: Patricia Reitman
**Environment:** Ubuntu on WSL2, xv6-riscv (revision 06aad25) on QEMU

---

## Part 2. Using xv6 (Observe)

**(1) Three programs xv6 ships with:**
`cat`, `echo` and `ls`. (`ls` also lists others, for example `grep`, `wc`, `sh`, `mkdir`, `rm`, `kill` and `usertests`.)

**(2) Which two OS features must exist for a pipe to work?**
- **Processes:** the shell has to create (`fork`) and start (`exec`) two programs, such as `ls` and `grep`, which run at the same time.
- **Communication between processes (the `pipe` system call):** the kernel provides a buffer that connects the output of the first program to the input of the second, through file descriptors.

In `ls | grep c`, `ls` writes its list into the pipe and `grep` reads it and prints only the lines that contain `c`.

**(3) xv6 shell vs the Linux shell from Lab 1:**
It works the same way at the basic level (prompt, commands, pipes), but it is much smaller and simpler, with no history, no tab completion and far fewer commands and features.

---

## Part 3. Reading the source (Observe)

**(1) System calls used by `user/cat.c`, and what each asks the kernel to do:**
- `open(name, O_RDONLY)`: open a file by its name and give back a file descriptor.
- `read(fd, buf, n)`: read up to `n` bytes from that file descriptor into the buffer.
- `write(1, buf, n)`: write `n` bytes to file descriptor 1, the screen.
- `close(fd)`: release the file descriptor when the file has been read.
- `exit(...)`: end the program and tell the kernel its exit status (also used on errors).

Error messages use `fprintf(2, ...)`, which writes to file descriptor 2 (standard error).

**(2) Where is `sys_read` implemented?**
In `kernel/sysfile.c`, at **line 69** (`sys_write` is at line 83). I found it with `grep -n "sys_read\|sys_write" kernel/sysfile.c`.

**(3) Difference between `kernel/` and `user/`:**
`kernel/` is the operating system itself, which runs privileged and controls the hardware, memory, files and processes, while `user/` holds ordinary programs like `ls`, `cat` and the shell that can only use those resources by asking the kernel through system calls.

---

## Part 4. sleep program (note)

`user/sleep.c` builds and works in xv6:
```
$ ls | grep sleep
sleep          2 5 35280
$ sleep 10
$ sleep
usage: sleep <ticks>
```
****Note:** the lab sheet says to call `sleep()`, but in this xv6 version the system call is named `pause()` (see `user/user.h`: `int pause(int);`). With `sleep()` the build failed with "implicit declaration of function 'sleep'", so `sleep.c` calls `pause()`. The program is still named `sleep`. ☺ 
---

## Conclusion

In this lab I started xv6 and used its small shell to run `ls`, `cat`, `echo` and `wc`, and to join programs with a pipe, which only works because the kernel can create processes and connect them. By reading `user/cat.c` I saw that an ordinary program does its work through a few system calls (`open`, `read`, `write`, `close`, `exit`), and I found that `sys_read` is implemented in `kernel/sysfile.c`, so `kernel/` is the system itself and `user/` holds the programs that ask it for services. I also wrote my own `sleep` program and added it to the Makefile. It first failed to build, because this version of xv6 names the system call `pause()` instead of `sleep()`. After I changed that call it worked, and it showed me the whole way from a C file to a program that runs in xv6.
