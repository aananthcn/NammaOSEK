gcc test_fifo.c ../kernel/os_fifo.c -o test_fifo.bin -I ../include/ -I ../tools/src/ -I ../kernel/

./test_fifo.bin

rm test_fifo.bin *.o