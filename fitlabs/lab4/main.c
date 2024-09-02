#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "stack.h"

#define MAX_STRING_LENGTH 1001
#define TOKENS_ALPHABET 26

int powint(int a, int n) {
    int res = 1;
    while (n) {
        if (n & 1) {
            res *= a;
            n--;
        }
        a *= a;
        n >>= 1;
    }
    return res;
}

int log2int(int x) {
    int res = 0;
    while (x >> 1) {
        x >>= 1;
        res++;
    }
    return res;
}

int logint(int x, int y) {
    return (log2int(y) / log2int(x));
}

int is_number(char c) {
    return ((c >= '0') && (c <= '9')) ? 1 : 0;
}

int is_operand(char c) {
    return ((c >= 'A') && (c <= 'Z')) ? 1 : 0;
}

int is_operator(char c) {
    if (c == '+' || c == '-')
        return 1;
    if (c == '*' || c == '/')
        return 1;
    if (c == '^' || c == 'l')
        return 1;
    return 0;
}

int check_symbol(char c) {
    return (is_number(c) || is_operator(c) || c == '(' || c == ')') ? 1 : 0;
}

int check_symbols(char *infix, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (!check_symbol(infix[i]))
            return 0;
    }
    return 1;
}

//Checks the nearest bracket elements
int check_bracket(char *infix, size_t len, size_t i) {
    if (infix[i] == '(') {
        if (i > 0 && (is_number(infix[i-1]) || infix[i-1] == ')'))
            return 0;
        if (i < len - 1 && (is_operator(infix[i+1]) || infix[i+1] == ')'))
            return 0;
    }
    if (infix[i] == ')') {
        if (i > 0 && (is_operator(infix[i-1]) || infix[i-1] == '('))
            return 0;
        if (i < len - 1 && (is_number(infix[i+1]) || infix[i+1] == '('))
            return 0;
    }
    return 1;
}

int check_brackets(char *infix, size_t len) {
    Stack brackets;
    init_stack(&brackets);
    for (size_t i = 0; i < len; i++) {
        char x = infix[i];
        if (x == '(') {
            if (!check_bracket(infix, len, i)) {
                delete_stack(&brackets);
                return 0;
            }
            push(&brackets, x);
        }
        if (x == ')') {
            if (!check_bracket(infix, len, i)) {
                delete_stack(&brackets);
                return 0;
            }
            if (!is_empty(brackets)) {
                pop(&brackets);
            }
            else {
                delete_stack(&brackets);
                return 0;
            }
        }
    }
    if (!is_empty(brackets)) {
        delete_stack(&brackets);
        return 0;
    }
    return 1;
}

int check_operations(char *infix, size_t len) {
    if (is_operator(infix[0]) || is_operator(infix[len-1]))
        return 0;
    for (size_t i = 0; i < len - 1; i++) {
        if (is_operator(infix[i]) && is_operator(infix[i+1]))
            return 0;
    }
    return 1;
}

int check_string(char *infix, size_t len) {
    if (!check_symbols(infix, len))
        return 0;
    if (!check_operations(infix, len))
        return 0;
    if (!check_brackets(infix, len))
        return 0;
    return 1;
}

int get_priority(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    if (op == '^' || op == 'l')
        return 3;
    return 0;
}

size_t infix_to_postfix(char *infix, size_t len, char *postfix) {
    Stack operators;
    init_stack(&operators);
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        char x = infix[i];
        if (is_operand(x)) {
            postfix[j] = x;
            j++;
        }
        if (x == '(') {
            push(&operators, x);
        }
        if (x == ')') {
            while (get_top(operators) != '(') {
                postfix[j] = pop(&operators);
                j++;
            }
            pop(&operators);
        }
        if (is_operator(x) && x != '(' && x != ')') {
            while (!is_empty(operators) && get_priority(get_top(operators)) >= get_priority(x)) {
                postfix[j] = pop(&operators);
                j++;
            }
            push(&operators, x);
        }
    }
    while (!is_empty(operators)) {
        postfix[j] = pop(&operators);
        j++;
    }
    return j;
}

int do_operation(int a, int b, char op) {
    if (op == '+')
        return a + b;
    if (op == '-')
        return a - b;
    if (op == '*')
        return a * b;
    if (op == '/')
        return b ? a / b : INT_MIN;
    if (op == '^')
        return powint(a, b);
    if (op == 'l')
        return (b >> 1) ? logint(b, a) : INT_MAX;
    return 0;
}

int calculate(char *postfix, size_t len, int *tokens) {
    Stack operands;
    init_stack(&operands);
    for (size_t i = 0; i < len; i++) {
        char x = postfix[i];
        if (is_operand(x)) {
            push(&operands, tokens[x - 'A']);
        }
        if (is_operator(x)) {

            int b = pop(&operands);
            int a = pop(&operands);
            int result = do_operation(a, b, x);

            if (result == INT_MIN || result == INT_MAX) {
                delete_stack(&operands);
                return result;
            }
            else push(&operands, result);
        }
    }
    return pop(&operands);
}

char set_token(int *tokens, size_t *tokens_length, int n) {
    for (size_t i = 0; i < *tokens_length; i++) {
        if (tokens[i] == n) {
            return ('A' + i);
        }
    }
    if (*tokens_length + 1 <= TOKENS_ALPHABET) {
        tokens[(*tokens_length)++] = n;
        return ('A' + *tokens_length - 1);
    }
    else return 0;
}

void set_tokens(char *infix, size_t *infix_length, int *tokens, size_t *tokens_length) {
    char result[MAX_STRING_LENGTH];
    size_t j = 0;
    for (size_t i = 0; i < *infix_length; i++) {
        if (!is_number(infix[i]))
            result[j++] = infix[i];
        else {
            int n = 0;
            while (is_number(infix[i]))
                n = 10 * n + (infix[i++] - '0');
            i--;

            char token = set_token(tokens, tokens_length, n);
            if (token != 0)
                result[j++] = token;
            else {
                *tokens_length = 0;
                return;
            }
        }
    }
    strcpy(infix, result);
    *infix_length = j;
}

int main(void) {
    char infix[MAX_STRING_LENGTH] = "";
    if (fgets(infix, MAX_STRING_LENGTH, stdin) != NULL) {
        size_t infix_length = strlen(infix);
        if (infix[infix_length - 1] == '\n')
            infix[--infix_length] = '\0';

        if (infix_length > 0) {
            if (check_string(infix, infix_length)) {

                int tokens[TOKENS_ALPHABET];
                size_t tokens_length = 0;
                set_tokens(infix, &infix_length, tokens, &tokens_length);

                if (tokens_length != 0) {
                    char postfix[MAX_STRING_LENGTH] = "";
                    size_t postfix_length = infix_to_postfix(infix, infix_length, postfix);

                    int result = calculate(postfix, postfix_length, tokens);
                    if (result == INT_MIN)
                        puts("division by zero");
                    else if (result == INT_MAX)
                        puts("wrong logarithm base");
                    else printf("%d\n", result);
                }
                else puts("too much arguments");
            }
            else puts("syntax error");
        }
        else puts("syntax error");
    }
    else puts("syntax error");
    return EXIT_SUCCESS;
}
