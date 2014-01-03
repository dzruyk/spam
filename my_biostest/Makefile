all:	img

img:	boot
	./pybuild boot

boot:	simple_img.o
	ld -o boot --oformat=binary -Ttext=0x7c00 $?

simple_img.o: simple_img.S
	gcc -c $?

qemu-gdb:
	qemu -s -S -cpu 486 -fda ./img

qemu:
	qemu -cpu 486 -fda ./img


clean:
	rm boot img *.o
