shell: main.o  Functions.o insert_command.o pwds.o echos.o insert.o changedir.o removes.o jobs.o fg.o kjob.o killallbg.o
	gcc $^ -o  $@

main.o: Functions.h
main.o: main.c 
	gcc $< -c -o $@


Functions.o: Functions.h
Functions.o: Functions.c 
	gcc $< -c -o $@

insert_command.o: Functions.h
insert_command.o: insert_command.c 
	gcc $< -c -o $@

pwds.o: Functions.h
pwds.o: pwds.c 
	gcc $< -c -o $@



echos.o: Functions.h
echos.o: echos.c 
	gcc $< -c -o $@	



insert.o: Functions.h
insert.o: insert.c 
	gcc $< -c -o $@


changedir.o: Functions.h
changedir.o: changedir.c 
	gcc $< -c -o $@	


removes.o: Functions.h
removes.o: removes.c 
	gcc $< -c -o $@	


jobs.o : Functions.h
jobs.o :jobs.c
	gcc $< -c -o $@	

fg.o : Functions.h
fg.o : fg.c
	gcc $< -c -o $@	

kjob.o : Functions.h
kjob.o : kjob.c
	gcc $< -c -o $@	

killallbg.o : Functions.h
killallbg.o : killallbg.c
	gcc $< -c -o $@