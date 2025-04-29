
sudo taskset -cp 1 $(pgrep -xo lighttpd)


for pid in $(pgrep -f '^kswapd[0-9]*$'); do
  sudo taskset -pc 0 "$pid"
done

echo 0 | sudo tee /proc/sys/kernel/timer_migration

echo -1 | sudo tee /proc/sys/kernel/sched_rt_runtime_us

echo 0 | sudo tee /proc/sys/kernel/numa_balancing

echo "Done"
