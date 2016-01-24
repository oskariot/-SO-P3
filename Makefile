compile: compile_prog compile_test
compile_prog: riverCrossing.c
	gcc -std=c99 -pthread riverCrossing.c -o riverCrossing -lm
compile_test: tester.c
	gcc -std=c99 tester.c -o tester
run: ./riverCrossing
	./riverCrossing ${ARGS}
# testy
test1: run_with_test_params1 run_tester clean_output
test2: run_with_test_params2 run_tester clean_output
test3: run_with_test_params3 run_tester clean_output
test4: run_with_test_params4 run_tester clean_output
test5: run_with_test_params5 run_tester clean_output

# wywołania z parametrami testowymi
run_with_test_params1: ./riverCrossing
	./riverCrossing -s 0 -p 199 -t 1 > /dev/null
run_with_test_params2: ./riverCrossing
	./riverCrossing -s 0 -p 200 -t 1 > /dev/null
run_with_test_params3: ./riverCrossing
	./riverCrossing -s 0 -p 201 -t 1 > /dev/null
run_with_test_params4: ./riverCrossing
	./riverCrossing -s 0 -p 202 -t 1 > /dev/null
run_with_test_params5: ./riverCrossing
	./riverCrossing -s 0 -p 1015 -t 1 > /dev/null

run_tester: ./tester -lm
	./tester
clean: clean_prog clean_tester clean_output
clean_prog:
	rm ./riverCrossing
clean_tester:
	rm ./tester
clean_output:
	rm -rf ./output.txt
