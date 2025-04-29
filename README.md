# Virtualization Project


**VIDEO DEMO**: https://drive.google.com/file/d/1PLR8ojbRz-5lLU8dncElHBFEq3i39ibl/view?usp=sharing

## Setup

### Setting up the virtual machine


The virtual machine is a QEMU/KVM machine run by `virt-manager`. To setup `virt-manager`, follow these directions.
- Download an Ubuntu `.iso` file from [here](https://ubuntu.com/download/desktop#how-to-install-NobleNumbat).
- Follow this [YouTube](https://www.youtube.com/watch?v=zAI-OWkavAw) tutorial to set up the necessary daemons and services for `virt-manager`.
- After installing Ubuntu, reboot the machine and remove the installation medium. 


### How To Move Files From Host to VM
- add file to `shared9p` folder (on WSL)
- navigate to `~/Desktop/wsl` on the VM
- `sudo mount -t 9p -o trans=virtio hostshare /mnt/hostshare`

- run `cp /mnt/hostshare/<filename> ./` to copy the file to the directory

# Testing Notes

To run a test, run `httperf --server <vm-ip> --port 80 --num-conns 100 --rate 10 --timeout 1`


### No Isolation 

### *100 connections + 10 connections per second*


Connection rate: 10.0 conn/s (99.8 ms/conn, <=15 concurrent connections)
Connection time [ms]:
```
min 0.6 avg 1.7 max 7.2 median 1.5 stddev 1.1
```
Connection time [ms]: connect 0.8
Connection length [replies/conn]: 1.000

Request rate: 10.0 req/s (99.8 ms/req)
Request size [B]: 66.0

Reply rate [replies/s]: 
```
min 7.2 avg 10.0 max 13.0 stddev 1.5 (9 samples)
```
Reply time [ms]: response 0.8 transfer 0.0
Reply size [B]: header 215.0 content 402.0 footer 0.0 (total 617.0)
Reply status: 1xx=0 2xx=500 3xx=0 4xx=0 5xx=0

CPU time [s]: user 35.58 system 13.83 (user 71.3% system 27.7% total 99.0%)
Net I/O: 6.7 KB/s (0.1*10^6 bps)

Errors: total 0 client-timo 0 socket-timo 0 connrefused 0 connreset 0
Errors: fd-unavail 0 addrunavail 0 ftab-full 0 other 0

### *500 connections 10 per second*
Total: connections 500 requests 500 replies 500 test-duration 49.902 s

Connection rate: 10.0 conn/s (99.8 ms/conn, <=14 concurrent connections)
Connection time [ms]: min 0.5 avg 3.9 max 1314.4 median 1.5 stddev 58.7
Connection time [ms]: connect 3.2
Connection length [replies/conn]: 1.000

Request rate: 10.0 req/s (99.8 ms/req)
Request size [B]: 66.0

Reply rate [replies/s]: min 10.0 avg 10.0 max 10.0 stddev 0.0 (9 samples)
Reply time [ms]: response 0.7 transfer 0.0
Reply size [B]: header 215.0 content 402.0 footer 0.0 (total 617.0)
Reply status: 1xx=0 2xx=500 3xx=0 4xx=0 5xx=0

CPU time [s]: user 34.04 system 13.89 (user 68.2% system 27.8% total 96.1%)
Net I/O: 6.7 KB/s (0.1*10^6 bps)

Errors: total 0 client-timo 0 socket-timo 0 connrefused 0 connreset 0
Errors: fd-unavail 0 addrunavail 0 ftab-full 0 other 0
dchen36@Derek:~$ httperf --server 192.168.122.3 --port 80 --num-conns 500 --rate 10 --timeout 1
httperf --timeout=1 --client=0/1 --server=192.168.122.3 --port=80 --uri=/ --rate=10 --send-buffer=4096 --recv-buffer=16384 --num-conns=500 --num-calls=1
Maximum connect burst length: 18

Total: connections 500 requests 500 replies 500 test-duration 49.903 s

Connection rate: 10.0 conn/s (99.8 ms/conn, <=18 concurrent connections)
Connection time [ms]: 
```
min 0.6 avg 1.2 max 4.4 median 0.5 stddev 0.6
```
Connection time [ms]: connect 0.6
Connection length [replies/conn]: 1.000

Request rate: 10.0 req/s (99.8 ms/req)
Request size [B]: 66.0

Reply rate [replies/s]: 
```
min 6.6 avg 10.0 max 13.6 stddev 1.8 (9 samples)
```
Reply time [ms]: response 0.6 transfer 0.0
Reply size [B]: header 215.0 content 402.0 footer 0.0 (total 617.0)
Reply status: 1xx=0 2xx=500 3xx=0 4xx=0 5xx=0

CPU time [s]: user 34.69 system 16.08 (user 69.5% system 32.2% total 101.7%)
Net I/O: 6.7 KB/s (0.1*10^6 bps)

Errors: total 0 client-timo 0 socket-timo 0 connrefused 0 connreset 0
Errors: fd-unavail 0 addrunavail 0 ftab-full 0 other 0

### Kernel + User-Level Isolation 

### *100 connections + 10 connections per second*

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

Alternatively, you can use the `isolation_on.sh` shell script to run all of these commands. 


Connection rate: 10.0 conn/s (99.8 ms/conn, <=15 concurrent connections)
Connection time [ms]: 
```
min 0.5 avg 1.7 max 21.5 median 1.5 stddev 1.3
```
Connection time [ms]: connect 0.8
Connection length [replies/conn]: 1.000

Request rate: 10.0 req/s (99.8 ms/req)
Request size [B]: 66.0

Reply rate [replies/s]: 
```
min 10.0 avg 10.0 max 10.0 stddev 0.0 (9 samples)
```
Reply time [ms]: response 0.9 transfer 0.0
Reply size [B]: header 215.0 content 402.0 footer 0.0 (total 617.0)
Reply status: 1xx=0 2xx=500 3xx=0 4xx=0 5xx=0

CPU time [s]: user 34.65 system 14.89 (user 69.4% system 29.8% total 99.3%)
Net I/O: 6.7 KB/s (0.1*10^6 bps)

Errors: total 0 client-timo 0 socket-timo 0 connrefused 0 connreset 0
Errors: fd-unavail 0 addrunavail 0 ftab-full 0 other 0

### *500 connections 10 per second*
Total: connections 500 requests 500 replies 500 test-duration 49.902 s

Connection rate: 10.0 conn/s (99.8 ms/conn, <=14 concurrent connections)
Connection time [ms]: 
```
min 0.5 avg 3.9 max 1314.4 median 1.5 stddev 58.7
```
Connection time [ms]: connect 3.2
Connection length [replies/conn]: 1.000

Request rate: 10.0 req/s (99.8 ms/req)
Request size [B]: 66.0

Reply rate [replies/s]: 
```
min 10.0 avg 10.0 max 10.0 stddev 0.0 (9 samples)
```
Reply time [ms]: response 0.7 transfer 0.0
Reply size [B]: header 215.0 content 402.0 footer 0.0 (total 617.0)
Reply status: 1xx=0 2xx=500 3xx=0 4xx=0 5xx=0

CPU time [s]: user 34.04 system 13.89 (user 68.2% system 27.8% total 96.1%)
Net I/O: 6.7 KB/s (0.1*10^6 bps)

Errors: total 0 client-timo 0 socket-timo 0 connrefused 0 connreset 0
Errors: fd-unavail 0 addrunavail 0 ftab-full 0 other 0


## Interpreting the Tests

For no isolation, there is a roughly 2 m/s deviation between connection and reply times. This implies that not all connections are taking the same amount of time and there is likely context switching happening that prevents the web server from being active the entire time to receieve connections and send responses. This is the same for the 100 and 500 connection tests.

For kernel and user isolation, both 100 and 500 tests reported a 0 standard deviation for the reply time. This implies that the server was always avaliable to send responses because it was always on the isolated core. There is still deviation for connections, which is likely the case because connecting requires kernel operations beyond the web server.

