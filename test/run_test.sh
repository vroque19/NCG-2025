echo "=== Running All Tests ==="
echo "\n1. Running Tower Stack Tests..."
gcc -I ../include ../src/stack.c test_api.c test_stack.c -o stack_tests.out && ./stack_tests.out
echo "\n2. Running History Stack Tests..."
gcc -I ../include ../src/history.c test_api.c test_history.c -o history_tests.out && ./history_tests.out
# echo "\n3. Running Auto Solve Logic Tests..."
# gcc -I ../include ../src/mode_auto.c test_api.c test_auto.c -o auto_tests.out && ./auto_tests.out
echo "\n3. Running Game Logic Tests..."
gcc -I ../include ../src/game_logic.c ../src/stack.c ../src/queue.c ../src/set.c \
  ../src/history.c test_api.c test_game_logic.c -o \
  game_logic_tests.out && ./game_logic_tests.out
# echo "\n=== All Tests Complete ==="
echo "\n4. Running Queue Logic Tests..."

gcc -I ../include ../src/game_logic.c ../src/stack.c ../src/history.c ../src/queue.c ../src/set.c \
  test_api.c test_queue.c -o queue_tests.out && ./queue_tests.out

rm -f *.out
