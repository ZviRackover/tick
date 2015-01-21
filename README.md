# tick
Run and measure Windows process elapsed time from the command line.
Use Win32 API to achieve a lean tool for measuring:
- Process elapsed time
- Total time executed in User mode
- Total time executed in Kernel mode

Example of usage, execute the sleep program with argument=5:
>tick.exe sleep 5
user: 0.015625s
kernel: 0s
Elapsed: 5.02539s
