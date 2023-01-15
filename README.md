# Introduction to Programming Systems Course 
## Tel Aviv University - Fall Semester 2022/2023

### Processes and Threads:
> #### 1) Homework 1: Build a shell under Linux
> A simple shell in C under Linux, called hw1shell, executing user commands. The
commands *exit*, *cd* and *jobs* are internal commands implemented in the shell. Any
other command is an external command which executed using one of the *exec* system calls.
There is no for support redirection or pipes, nor multiple commands on the same line.

> #### 2) Homework 2: Dispatcher/Worker model with Linux pthreads
> A Dispatcher/Worker model in Linux using threads to offload work. 
> The dispatcher will spawn a configurable number of worker threads (*num_threads*).
> The commands to be executed are specified in a command file, read by the dispatcher.
> Some commands in the command file will be executed serially on the dispatcher, while some commands (jobs) will be placed on a shared work queue, and later off-loaded to an available worker thread.
> Synchronization features used so that when waiting for something, the thread will be putted to sleep instead of busy waiting.
Command syntax:
> > **How to Run**: hw2 cmdfile.txt num threads num counters log enabled
> > > - cmdfile.txt: input text file containing the commands.
> > > - num_threads: number of worker threads to create. Max 4096.
> > > - num_counters: number of counter files to create. Max 100.
> > > - log_enabled: 1 to write trace files, 0 witout trace files.

### xv6:
> #### 3) Homework 3: Adding system calls in xv6 
> In this exercise we added new system calls to xv6, which return information about processes to user space, and write a user space program, a simple version of *ps* in Linux, to display the list of processes.

> #### 4) Homework 4: Adding priorities to xv6 scheduler
> xv6 includes a round-robin scheduler which gives all processes equal priority. In this exercise,
we modified it so that it takes into account user defined process priorities.
We’ll modify xv6 such that each process is assigned a 3-bit exponential priority P. Priority 0 is
the lowest, and priority 7 is the highest.
By default, all processes start with priority 0, but the priority can be changed using new system
calls. We added system calls to xv6 to set/get process priorities. 
> We changeed the xv6 scheduler to take the priority P into account. 
> The priority indicates the base-2 logarithm of the maximum number of consecutive timer ticks the process may get during the round-robin scanning of the processes.
> As long as the process is runnable (not sleeping), it should be scheduled $2^P$ times in sequence before considering the next process.
> If before the process finished its $2^P$ ticks, it needs to sleep waiting on some event and is not runnable, it should not be scheduled and the scheduler should continue in the round robin scanning of the next processes.  
> We added a user-space program, *demosched.c*, which demonstrates the new scheduler priority.
It forks multiple children that run CPU bound code, sets different priorities, and demonstrate that processes finish computation in accordance to the set priorities.

> #### 5) Homework 5: Reading the xv6 filesystem in Linux  
> The goal of this exercise is to write a user-space application, which will run under Linux (not
under xv6), and be able to understand the xv6 filesystem internal data structures (superblock, inodes,
directory entries, etc), to the point where we can:
> 1. Display in Linux a listing of the xv6 root directory similar to the ls output from within xv6.
> 2. Be able to exract a file from within the root directory in the xv6 filesytem image, and write it as
a normal file in Linux.
> > **How to Run**: The command syntax is one of the following two lines:  
> > a) hw5 fs.img ls  
> > b) hw5 fs.img cp xv6file linuxfile  
> > > - fs.img is the xv6 filesystem image file.
> > > - If the second argument is *ls*, the root directory listing should be displayed as if we were running *ls* from within xv6.
> > > - If the second argument is *cp*, the contents of the file named *xv6file* is extracted from within the root directory of xv6, and writen to a file under linux named *linuxfile*.



> #### Running xv6:
> > **How to Run:**  
> >  Graphics Mode:  make -> make qemu-nox (to quit: Ctrl+A than X)  
> > Text Mode: make -> make qemu  
> >
> > **Debugging in vs code**: Run Build Task (Ctrl+Shift+B) -> Start Debugging (F5)  
> >  https://shawnzhong.com/2019/03/25/co-debug-xv6-on-windows-using-vscode/
