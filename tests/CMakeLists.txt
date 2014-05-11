cmake_minimum_required(VERSION 2.8)

project(Scheme)

set(BUILD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)

set(TEST_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(TEST_BINARY cmd_test)

set(TEST_SOURCES
    ${TEST_SOURCE_PATH}/cmd_test.c)

macro(exec_test name input output)
	add_test(NAME ${name}
		     COMMAND "${TEST_BINARY}"  $<TARGET_FILE_DIR:${BINARY}>/$<TARGET_FILE_NAME:${BINARY}> ${input} ${output})
endmacro(exec_test)

exec_test(dot_pair_test "'(a . (b . (c . d)))" "(a b c . d)")

exec_test(not_test_1 "(not #t)" "#f")
exec_test(not_test_2 "(not 3)" "#f")
exec_test(not_test_3 "(not (list 3))" "#f")
exec_test(not_test_4 "(not #f)" "#t")
exec_test(not_test_5 "(not '())" "#f")
exec_test(not_test_6 "(not (list))" "#f")
exec_test(not_test_7 "(not 'nil)" "#f")

exec_test(pairq_test_1 "(pair? '(a . b))" "#t")
exec_test(pairq_test_2 "(pair? '(a b b))" "#t")
exec_test(pairq_test_3 "(pair? '())" "#f")
##exec_test(pairq_test_4 "(pair? '#(a b))" "#f")

exec_test(cons_test_1 "(cons 'a '())" "(a)")
exec_test(cons_test_2 "(cons '(a) '(b c d))" "((a) b c d)")
##exec_test(cons_test_3 "(cons "a" '(b c))" "("a" b c)")
exec_test(cons_test_4 "(cons 'a 3)" "(a . 3)")
exec_test(cons_test_5 "(cons '(a b) 'c)" "((a b) . c)")

exec_test(car_test_1 "(car '(a b c))" "a")
exec_test(car_test_2 "(car '((a) b c d))" "(a)")
exec_test(car_test_3 "(car '(1 . 2))" "1")
##exec_test(car_test_3 "(car '())" "")

exec_test(cdr_test_1 "(cdr '((a) b c d))" "(b c d)")
exec_test(cdr_test_2 "(cdr '(1 . 2))" "2")
##exec_test(cdr_test_3 "(cdr '())" "")

exec_test(list_test_1 "(list 'a (+ 3 4) 'c)" "(a 7 c)")
exec_test(list_test_2 "(list)" "()")

add_executable(${TEST_BINARY} ${TEST_SOURCES})
