Technical University of Moldova
Faculty of Computers, Informatics and Microelectronics

# OS Lab 1: The OS as a Resource Manager (Track A, Linux)

**Student:** Marin, FAF-241
** CHecked by: Patricia Reitman
**Environment:** Ubuntu on WSL2 (Windows), kernel 6.18.54.1-microsoft-standard-WSL2

---

## Part 1. Files and directories (file management)

**(1) Who owns the files, and which group?**
The owner is `marin` and the group is also `marin`. In `ls -l` the 3rd column is the owner and the 4th is the group. Ubuntu creates a group with the same name as the user by default.

**(2) What do the ten characters mean after `chmod 600`?**
`-rw-------` splits into four parts:
- `-` : a regular file (a directory would show `d`)
- `rw-` : the owner can read and write, but not execute
- `---` : the group has no permissions
- `---` : everyone else has no permissions

So only `marin` can read or change the file. The number 600 says the same thing: 6 = read (4) + write (2), and the two 0s mean no access.

**(3) Two directories under `/`:**
- `/etc` : system-wide configuration files (for example `adduser.conf`, `apt`, `bash.bashrc`).
- `/home` : the personal folders of users (mine is `/home/marin`).

**Interesting output:** the permissions of `note.txt` before and after `chmod 600`:
```
-rw-r--r-- 1 marin marin 24 Oct 23 12:36 note.txt
-rw------- 1 marin marin 24 Oct 23 12:36 note.txt
```
Before, the group and others could read the file. After, only I can.

---

## Part 2. Processes (process management)

**(1) PID of process number 1, and what is it?**
PID 1 is `systemd`, run by root. It is the first process the OS starts at boot, and it starts and manages the other services.

**(2) Roughly how many processes on the idle VM?**
About 27. `top` showed `Tasks: 27 total, 1 running, 26 sleeping, 0 stopped, 0 zombie`. Almost all of them were sleeping, because nothing was busy.

**(3) What does the `State:` line say for the sleeping process?**
`State:  S (sleeping)`. The `sleep` process is waiting for its timer to finish, so it is not using the CPU.

**Interesting output:** the `/proc` entry of my `sleep` process:
```
Name:   sleep
State:  S (sleeping)
Pid:    1780
PPid:   426
Uid:    1000    1000    1000    1000
```
`PPid: 426` is my `bash` shell, which started `sleep`. This shows that every process has a parent.

---

## Part 3. Memory (memory management)

**(1) Total RAM and free RAM?**
From `free -h`: about 11 GiB total (`MemTotal: 12099480 kB`) and about 10 GiB free. Only 594 MiB was in use.

**(2) What is swap, and how much is configured?**
Swap is disk space the OS uses as an extension of RAM. When RAM is full, the OS can move rarely used memory pages to swap to make room. Here 3.0 GiB of swap is configured and 0 B is used, since there is plenty of free RAM.

**(3) How much resident memory (VmRSS) does a bare `sleep` use?**
`VmRSS: 3396 kB`, which is about 3.3 MB. This is small, but it is not zero. Even a program that does nothing needs memory for its code and the shared C library it loads. It also does not seem much next to 11 GiB of RAM.

**Interesting output:**
```
               total        used        free      shared  buff/cache   available
Mem:            11Gi       594Mi        10Gi       4.1Mi       299Mi        10Gi
Swap:          3.0Gi          0B       3.0Gi
```

---

## Part 4. Devices and storage (I/O management)

**(1) Which device is `/` mounted on?**
`/dev/sdd`, an ext4 filesystem of about 1 TB (1.3 GB used). `lsblk` and `mount` confirm it: `/dev/sdd on / type ext4`.

**(2) One `/dev` entry and what it stands for:**
- `/dev/sdd` is the (virtual) disk that holds my root filesystem.
- `/dev/console` (shown as `crw-------`) stands for the system console, which is the terminal.

**(3) What does "everything is a file" mean?**
The OS shows disks, terminals and other devices as entries in the file tree under `/dev`, so I can look at them with the same commands (`ls -l`, and so on) as normal files. The first letter of `ls -l` shows the kind: `c` for character devices like `console`, `d` for directories, `l` for links.

**Interesting output:** the block devices from `lsblk`:
```
NAME MAJ:MIN RM   SIZE RO TYPE MOUNTPOINTS
sda    8:0    0 722.9M  1 disk
sdb    8:16   0    97M  1 disk /usr/lib/linux-tools/6.18.54.1-microsoft-standard-WSL2
sdc    8:32   0     3G  0 disk [SWAP]
sdd    8:48   0     1T  0 disk /
```
`sdc` is the 3 GB swap disk from Part 3, and `sdd` is the root disk. WSL gives Linux virtual disks, so these are not physical drives.

---

## Conclusion

The operating system manages four resources: files, processes, memory and devices. I saw files with `ls -l` (owners and permissions), processes with `ps aux` or `top` (PID 1 is `systemd`), and memory with `free -h` (11 GiB RAM and 3 GiB swap). I saw devices with `lsblk` (the disks behind `/dev`, with `/` on `/dev/sdd`), which also showed that the OS presents devices to me as files.
