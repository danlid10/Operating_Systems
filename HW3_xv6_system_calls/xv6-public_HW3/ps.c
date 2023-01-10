#include "types.h"
#include "user.h"

// Returns process state name by its number.
const char *state_str(int state)
{
    switch (state)
    {
        case 0:     return "UNUSED  ";   
        case 1:     return "EMBRYO  ";   
        case 2:     return "SLEEPING";   
        case 3:     return "RUNNABLE";   
        case 4:     return "RUNNING ";   
        case 5:     return "ZOMBIE  ";    
        default:    return "INVALID ";
    }
}

// Used to add spaces before PID prints to align columns.
void add_spaces(int pid)
{
    if (pid < 10)
        printf(1, "   ");
    else if (pid < 100)
        printf(1, "  ");
    else if (pid < 1000)
        printf(1, " ");
}

int main(void)
{
    struct processInfo proc_info;
    int max_pid, cur_pid;

    max_pid = getMaxPid();
    printf(1, "Total number of active processes: %d\n", getNumProc());
    printf(1, "Maximum PID: %d\n", max_pid);
    printf(1, " PID    STATE      PPID    SZ     NFD  NRSWITCH\n");

    for (cur_pid = 1; cur_pid <= max_pid; cur_pid++)
    {
        if (getProcInfo(cur_pid, &proc_info) < 0)  
            continue;

        add_spaces(cur_pid);
        printf(1, "%d", cur_pid);
        printf(1, "   %s", state_str(proc_info.state));
        add_spaces(proc_info.ppid);
        printf(1, "   %d", proc_info.ppid);
        printf(1, "    %d", proc_info.sz);
        printf(1, "    %d", proc_info.nfd);
        printf(1, "     %d\n", proc_info.nrswitch);
    }
    
    exit();
}