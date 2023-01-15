# Introduction to Programming Systems Course 
## Tel Aviv University - Fall Semester 2022/2023

### Processes and Threads:
> #### 1) Homework 1: Build a shell under Linux

> #### 2) Homework 2: Dispatcher/Worker model with Linux pthreads
> > **How to Run**: hw2 cmdfile.txt num_threads num_counters log_enabled
> > > - cmdfile.txt: input text file containing the commands.
> > > - num_threads: number of worker threads to create. Max 4096.
> > > - num_counters: number of counter files to create. Max 100.
> > > - log_enabled: 1 to write trace files, 0 witout trace files.

### xv6:
> #### 3) Homework 3: Adding system calls in xv6 

> #### 4) Homework 4: Adding priorities to xv6 scheduler

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
