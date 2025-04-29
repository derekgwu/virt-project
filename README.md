# virt-project


NOT NEEDED:
`sudo qemu-system-x86_64 -enable-kvm -m 4096 -smp 4 -hda ./qemuimg -boot d -netdev user,id=net0 -device e1000,netdev=net0 -vga virtio -display sdl`

## How To Move Files From Host to VM
- add file to `shared9p` folder (on WSL)
- navigate to `~/Desktop/wsl` on the VM
- `sudo mount -t 9p -o trans=virtio hostshare /mnt/hostshare`

- run `cp /mnt/hostshare/<filename> ./` to copy the file to the directory

## Testing Notes

` httperf --server 192.168.122.3 --port 80 --num-conns 100 --rate 10 --timeout 1` 
