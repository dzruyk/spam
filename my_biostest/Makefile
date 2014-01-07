all:	img

img:	boot
	./pybuild boot

boot:	simple_img.o
	ld -o boot --oformat=binary -Ttext=0x0000 $?

simple_img.o: simple_img.S
	gcc -c $?

qemu-gdb: img
	qemu -s -S -cpu 486 -fda ./img

qemu: img
	qemu -cpu 486 -fda ./img
gdb: img
	gdb -x .gdbinit ./img

clean:
	rm boot img *.o
