# Introduction to Programming Systems Course 
## Tel Aviv University - Fall Semester 2022/2023

### Processes and Threads:
> #### 1) Homework 1: Build a shell under Linux

> #### 2) Homework 2: Dispatcher/Worker model with Linux pthreads
> > **How to Run**:   ./hw2 <cmdfile.txt> <num_threads> <num_counters> <log_enabled>
> > > - cmdfile.txt: input text file containing the commands.
> > > - num_threads: number of worker threads to create. Max 4096.
> > > - num_counters: number of counter files to create. Max 100.
> > > - log_enabled: 1 to write trace files, 0 witout trace files.

### xv6:
> #### 3) Homework 3: Adding system calls in xv6 

> #### 4) Homework 4: Adding priorities to xv6 scheduler

> #### 5) Homework 5: 


> #### Running xv6:
> > **How to Run:**  
> >  Graphics Mode:  make -> make qemu-nox (to quit: Ctrl+A than X)  
> > Text Mode: make -> make qemu  
> >
> > **Debugging in vs code**: Run Build Task (Ctrl+Shift+B) -> Start Debugging (F5)  
> >  https://shawnzhong.com/2019/03/25/co-debug-xv6-on-windows-using-vscode/
