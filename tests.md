## No Isolation 

*100 connections + 10 connections per second*


Connection rate: 10.0 conn/s (99.8 ms/conn, <=15 concurrent connections)
Connection time [ms]:
```
min 0.6 avg 1.7 max 7.2 median 1.5 stddev 1.1
```
Connection time [ms]: connect 0.8
Connection length [replies/conn]: 1.000

Request rate: 10.0 req/s (99.8 ms/req)
Request size [B]: 66.0

Reply rate [replies/s]: min 7.2 avg 10.0 max 13.0 stddev 1.5 (9 samples)
Reply time [ms]: response 0.8 transfer 0.0
Reply size [B]: header 215.0 content 402.0 footer 0.0 (total 617.0)
Reply status: 1xx=0 2xx=500 3xx=0 4xx=0 5xx=0

CPU time [s]: user 35.58 system 13.83 (user 71.3% system 27.7% total 99.0%)
Net I/O: 6.7 KB/s (0.1*10^6 bps)

Errors: total 0 client-timo 0 socket-timo 0 connrefused 0 connreset 0
Errors: fd-unavail 0 addrunavail 0 ftab-full 0 other 0

## Kernel + User-Level Isolation 

*100 connections + 10 connections per second*

The kernel level isolation consisted of modifying the kernel boot parameters. To modify the kernel boot parameters:
- `sudo vim /etc/default/grub` - Edit the grub file
- `GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"` - Find this line and add the kernel boot parameters
- `sudo update-grub` - Update the grub file 
- `sudo reboot` - Reboot to allow the changes to take effect. 

The user level isolation with consist of pinning `lighttpd` to *vCPU* 1 and pinning other threads to *vCPU* 0. The machine I am running this on has two *vCPUs*

User level commands run are:
- `taskset -pc 0 55` - pins the process of pid 55, which is a kernel thread, to vCPU 0
- `sudo taskset -cp 1 1100` - pins the `lighttpd` process to vCPU 1. 
- `echo 0 | sudo tee /proc/sys/kernel/timer_migration` Disable timer migration
- `echo -1 | sudo tee /proc/sys/kernel/sched_rt_runtime_us` Disable the CPU time limit for real-time cores
- `echo 0 > /proc/sys/kernel/numa_balancing` Disable NUMA balancing


Connection rate: 10.0 conn/s (99.8 ms/conn, <=15 concurrent connections)
Connection time [ms]: 
```
min 0.5 avg 1.7 max 21.5 median 1.5 stddev 1.3
```
Connection time [ms]: connect 0.8
Connection length [replies/conn]: 1.000

Request rate: 10.0 req/s (99.8 ms/req)
Request size [B]: 66.0

Reply rate [replies/s]: min 10.0 avg 10.0 max 10.0 stddev 0.0 (9 samples)
Reply time [ms]: response 0.9 transfer 0.0
Reply size [B]: header 215.0 content 402.0 footer 0.0 (total 617.0)
Reply status: 1xx=0 2xx=500 3xx=0 4xx=0 5xx=0

CPU time [s]: user 34.65 system 14.89 (user 69.4% system 29.8% total 99.3%)
Net I/O: 6.7 KB/s (0.1*10^6 bps)

Errors: total 0 client-timo 0 socket-timo 0 connrefused 0 connreset 0
Errors: fd-unavail 0 addrunavail 0 ftab-full 0 other 0
