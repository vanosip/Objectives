build:
	gcc main.c Graph.c dijkstra.c MaxFlow.c Tarjan.c -o final
clean:
	rm -f final *.o test*_output.txt
test: test-rpo test-dij test-flow test-tar

test-rpo: build test1 test2 test3
test1:
	@./final < Tests/test_rpo1.txt > testRpo1_output.txt
	@diff -w Expended/exp_rpo1.txt testRpo1_output.txt && echo "Test RPO1 was successful" || (echo "Test RPO1 failed"; exit 1)
test2:
	@./final < Tests/test_rpo2.txt > testRpo2_output.txt
	@diff -w Expended/exp_rpo2.txt testRpo2_output.txt && echo "Test RPO2 was successful" || (echo "Test RPO2 failed"; exit 1)
test3:
	@./final < Tests/test_rpo3.txt > testRpo3_output.txt
	@diff -w Expended/exp_rpo3.txt testRpo3_output.txt && echo "Test RPO3 was successful" || (echo "Test RPO3 failed"; exit 1)


test-dij: build test4 test5
test4:
	@./final < Tests/test_dij1.txt > testdij1_output.txt
	@diff -w Expended/exp_dij1.txt testdij1_output.txt && echo "Test Dijkstra1 was successful" || (echo "Test Dijkstra1 failed"; exit 1)
test5:
	@./final < Tests/test_dij2.txt > testdij2_output.txt
	@diff -w Expended/exp_dij2.txt testdij2_output.txt && echo "Test Dijkstra2 was successful" || (echo "Test Dijkstra2 failed"; exit 1)


test-flow: build test6 test7
test6:
	@./final < Tests/test_flow1.txt > testflow1_output.txt
	@diff -w Expended/exp_flow1.txt testflow1_output.txt && echo "Test flow1 was successful" || (echo "Test flow1 failed"; exit 1)
test7:
	@./final < Tests/test_flow2.txt > testflow2_output.txt
	@diff -w Expended/exp_flow2.txt testflow2_output.txt && echo "Test flow2 was successful" || (echo "Test flow2 failed"; exit 1)

test-tar: build test8 test9
test8:
	@./final < Tests/test_tar1.txt > testtar1_output.txt
	@diff -w Expended/exp_tar1.txt testtar1_output.txt && echo "Test Tarjan1 was successful" || (echo "Test Tarjan1 failed"; exit 1)
test9:
	@./final < Tests/test_tar2.txt > testtar2_output.txt
	@diff -w Expended/exp_tar2.txt testtar2_output.txt && echo "Test Tarjan2 was successful" || (echo "Test Tarjan2 failed"; exit 1)