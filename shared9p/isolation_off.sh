


sudo taskset -cp 0-3 $(pgrep -xo lighttpd)

for pid in $(pgrep -f '^kswapd[0-9]*$'); do
  sudo taskset -pc 0-3 "$pid"
done


echo 1 | sudo tee /proc/sys/kernel/timer_migration

echo 950000 | sudo tee /proc/sys/kernel/sched_rt_runtime_us


echo 1 | sudo tee /proc/sys/kernel/numa_balancing

